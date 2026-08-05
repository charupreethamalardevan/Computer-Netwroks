#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compute_checksum(char **data, int n, int k, char *checksum) {
    int sum = 0;
    int max_val = (1 << k) - 1;
    for (int i = 0; i < n; i++) {
        int val = 0;
        for (int j = 0; j < k; j++) {
            val = (val << 1) + (data[i][j] - '0');
        }
        sum += val;
    }
    while (sum > max_val) {
        int carry = sum >> k;
        sum = (sum & max_val) + carry;
    }
    int check_val = ~sum & max_val;
    for (int i = k - 1; i >= 0; i--) {
        checksum[i] = (check_val & 1) ? '1' : '0';
        check_val >>= 1;
    }
    checksum[k] = '\0';
}

int verify_checksum(char **data, int n, int k, char *checksum, int *final_sum, int *final_result) {
    int sum = 0;
    int max_val = (1 << k) - 1;
    for (int i = 0; i < n; i++) {
        int val = 0;
        for (int j = 0; j < k; j++) {
            val = (val << 1) + (data[i][j] - '0');
        }
        sum += val;
    }
    int check_val = 0;
    for (int j = 0; j < k; j++) {
        check_val = (check_val << 1) + (checksum[j] - '0');
    }
    sum += check_val;
    while (sum > max_val) {
        int carry = sum >> k;
        sum = (sum & max_val) + carry;
    }
    *final_sum = sum;
    *final_result = ~sum & max_val;
    return (*final_result == 0);
}

int main() {
    int k;
    printf("Enter segment size (n): ");
    scanf("%d", &k);

    char input[1000] = "";
    char text_input[200] = "";
    int num_segments = 0, choice, error_count, piece_num, bit_idx, input_type;
    int rx_sum = 0, rx_result = 0;

    printf("Select Input Type:\n1. Binary String\n2. Text Characters\nEnter choice: ");
    scanf("%d", &input_type);
    getchar();

    if (input_type == 2) {
        printf("Enter text data: ");
        fgets(text_input, sizeof(text_input), stdin);
        text_input[strcspn(text_input, "\n")] = '\0';
        for (int i = 0; text_input[i] != '\0'; i++) {
            char ch = text_input[i];
            char *bin_char = (char *)malloc((k + 1) * sizeof(char));
            for (int j = k - 1; j >= 0; j--) {
                bin_char[j] = (ch & 1) ? '1' : '0';
                ch >>= 1;
            }
            bin_char[k] = '\0';
            strcat(input, bin_char);
            free(bin_char);
        }
    } else {
        printf("Enter binary data: ");
        scanf("%s", input);
    }

    int len = strlen(input);
    int pad = (k - (len % k)) % k;
    int total_len = len + pad;
    char padded[1000] = "";
    for (int i = 0; i < pad; i++) {
        strcat(padded, "0");
    }
    strcat(padded, input);

    num_segments = total_len / k;

    char **segments = (char **)malloc(num_segments * sizeof(char *));
    char **rx_segments = (char **)malloc(num_segments * sizeof(char *));
    for (int i = 0; i < num_segments; i++) {
        segments[i] = (char *)malloc((k + 1) * sizeof(char));
        rx_segments[i] = (char *)malloc((k + 1) * sizeof(char));
        for (int j = 0; j < k; j++) {
            segments[i][j] = padded[i * k + j];
        }
        segments[i][k] = '\0';
    }

    char *tx_checksum = (char *)malloc((k + 1) * sizeof(char));
    char *rx_checksum = (char *)malloc((k + 1) * sizeof(char));

    compute_checksum(segments, num_segments, k, tx_checksum);

    printf("\n--- SENDER SIDE ---\n");
    for (int i = 0; i < num_segments; i++) {
        printf("Segment %d: %s\n", i + 1, segments[i]);
    }
    printf("Computed Checksum: %s\n", tx_checksum);

    for (int i = 0; i < num_segments; i++) {
        strcpy(rx_segments[i], segments[i]);
    }
    strcpy(rx_checksum, tx_checksum);

    int **flipped_tracker = (int **)malloc((num_segments + 2) * sizeof(int *));
    for (int i = 0; i < num_segments + 2; i++) {
        flipped_tracker[i] = (int *)calloc(k, sizeof(int));
    }

    printf("\n--- ERROR INJECTION MENU ---\n");
    printf("1. No Error (Clean Transmission)\n");
    printf("2. Single-bit Error\n");
    printf("3. Multi-bit Error\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 2) {
        printf("Enter Segment number to alter (Enter %d for Checksum): ", num_segments + 1);
        scanf("%d", &piece_num);
        printf("Enter bit position to flip (1 to %d): ", k);
        scanf("%d", &bit_idx);
        bit_idx--;
        if (piece_num <= num_segments) {
            rx_segments[piece_num - 1][bit_idx] = (rx_segments[piece_num - 1][bit_idx] == '0') ? '1' : '0';
        } else {
            rx_checksum[bit_idx] = (rx_checksum[bit_idx] == '0') ? '1' : '0';
        }
    } else if (choice == 3) {
        printf("Enter total number of error bits to inject: ");
        scanf("%d", &error_count);
        for (int e = 0; e < error_count; e++) {
            int valid_input = 0;
            while (valid_input == 0) {
                printf("Error bit %d -> Enter Segment number (Enter %d for Checksum): ", e + 1, num_segments + 1);
                scanf("%d", &piece_num);
                printf("Error bit %d -> Enter bit position to flip (1 to %d): ", e + 1, k);
                scanf("%d", &bit_idx);
                int internal_bit_idx = bit_idx - 1;
                if (flipped_tracker[piece_num][internal_bit_idx] == 1) {
                    printf("Invalid: This exact bit position has already been altered. Try another.\n");
                } else {
                    flipped_tracker[piece_num][internal_bit_idx] = 1;
                    valid_input = 1;
                    if (piece_num <= num_segments) {
                        rx_segments[piece_num - 1][internal_bit_idx] = (rx_segments[piece_num - 1][internal_bit_idx] == '0') ? '1' : '0';
                    } else {
                        rx_checksum[internal_bit_idx] = (rx_checksum[internal_bit_idx] == '0') ? '1' : '0';
                    }
                }
            }
        }
    } else {
        printf("No Error Induced.\n");
    }

    printf("\n--- RECEIVER SIDE ---\n");
    for (int i = 0; i < num_segments; i++) {
        printf("Received Segment %d: %s\n", i + 1, rx_segments[i]);
    }
    printf("Received Checksum: %s\n", rx_checksum);

    int is_valid = verify_checksum(rx_segments, num_segments, k, rx_checksum, &rx_sum, &rx_result);

    char *rx_sum_bin = (char *)malloc((k + 1) * sizeof(char));
    char *rx_result_bin = (char *)malloc((k + 1) * sizeof(char));
    int temp_sum = rx_sum;
    int temp_res = rx_result;
    for (int j = k - 1; j >= 0; j--) {
        rx_sum_bin[j] = (temp_sum & 1) ? '1' : '0';
        temp_sum >>= 1;
        rx_result_bin[j] = (temp_res & 1) ? '1' : '0';
        temp_res >>= 1;
    }
    rx_sum_bin[k] = '\0';
    rx_result_bin[k] = '\0';

    printf("Receiver Sum 1s Complement: %s \n", rx_sum_bin);
    printf("Receiver Checksum Value : %s \n", rx_result_bin);

    if (is_valid) {
        printf("Transmission Successful.\n");
        if (input_type == 2) {
            printf("Decoded Text: ");
            for (int i = 0; i < num_segments; i++) {
                char ch = 0;
                for (int j = 0; j < k; j++) {
                    ch = (ch << 1) + (rx_segments[i][j] - '0');
                }
                if (ch >= 32 && ch <= 126) {
                    printf("%c", ch);
                }
            }
            printf("\n");
        }
    } else {
        printf("Transmission Failed. Error Detected\n");
    }

    for (int i = 0; i < num_segments; i++) {
        free(segments[i]);
        free(rx_segments[i]);
    }
    for (int i = 0; i < num_segments + 2; i++) {
        free(flipped_tracker[i]);
    }
    free(segments);
    free(rx_segments);
    free(flipped_tracker);
    free(tx_checksum);
    free(rx_checksum);
    free(rx_sum_bin);
    free(rx_result_bin);

    return 0;
}
