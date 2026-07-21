
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 100
#define MAX_BINARY 2000
#define MAX_STUFFED 3000

int isBinary(char str[])
{
    int i;

    if(str[0] == '\0')
        return 0;

    for(i = 0; str[i] != '\0'; i++)
    {
        if(str[i] == '\n')
            continue;

        if(str[i] != '0' && str[i] != '1')
            return 0;
    }

    return 1;
}

void textToBinary(char text[], char binary[])
{
    int i, j;
    binary[0] = '\0';

    for(i = 0; text[i] != '\0'; i++)
    {
        for(j = 7; j >= 0; j--)
        {
            int len = strlen(binary);

            if(len < MAX_BINARY - 1)
            {
                binary[len] = ((text[i] >> j) & 1) + '0';
                binary[len + 1] = '\0';
            }
        }
    }
}

void binaryToText(char binary[], char text[])
{
    int len = strlen(binary);
    int i, j, k = 0;

    for(i = 0; i + 7 < len; i += 8)
    {
        char ch = 0;

        for(j = 0; j < 8; j++)
        {
            ch = (ch << 1) | (binary[i + j] - '0');
        }

        text[k++] = ch;
    }

    text[k] = '\0';
}

int main()
{
    char input[MAX_INPUT];
    char binary[MAX_BINARY];
    char stuffed[MAX_STUFFED];
    char destuffed[MAX_BINARY];
    char receivedText[MAX_INPUT];

    int inputWasBinary;
    int i, j, count;
    int pos, len;
    char choice;

    printf("Enter Data : ");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    inputWasBinary = isBinary(input);

    if(inputWasBinary)
    {
        strcpy(binary, input);
        printf("\nInput Type       : Binary");
    }
    else
    {
        printf("\nInput Type       : Text");
        textToBinary(input, binary);
    }

    // ---------- Bit Stuffing ----------
    j = 0;
    count = 0;

    for(i = 0; binary[i] != '\0'; i++)
    {
        stuffed[j++] = binary[i];

        if(binary[i] == '1')
        {
            count++;

            if(count == 5)
            {
                stuffed[j++] = '0';
                count = 0;
            }
        }
        else
        {
            count = 0;
        }
    }

    stuffed[j] = '\0';

    printf("\nBinary Data      : %s", binary);
    printf("\nStuffed Data     : %s", stuffed);

    printf("\n\nTransmitted Frame : ");
    printf("01111110 %s 01111110\n", stuffed);

    // ---------- Error Simulation ----------
    printf("\nDo you want to induce an error? (Y/N) : ");
    scanf(" %c", &choice);

    if(choice == 'Y' || choice == 'y')
    {
        len = strlen(stuffed);

        printf("Enter Bit Position (1-%d) : ", len);
        scanf("%d", &pos);

        if(pos >= 1 && pos <= len)
        {
            char oldBit = stuffed[pos - 1];

            stuffed[pos - 1] =
                (stuffed[pos - 1] == '0') ? '1' : '0';

            printf("\nBit at Position %d changed from %c to %c",
                   pos,
                   oldBit,
                   stuffed[pos - 1]);

            printf("\n\nFrame After Error : %s", stuffed);

            printf("\n\nTransmission Error Detected");
            printf("\nFrame Discarded");
            printf("\nDe-Stuffing Not Performed\n");
        }
        else
        {
            printf("\nInvalid Bit Position.\n");
        }
    }
    else
    {
        printf("\nNo Error Introduced.");

        // ---------- De-Stuffing ----------
        count = 0;
        j = 0;

        for(i = 0; stuffed[i] != '\0'; i++)
        {
            destuffed[j++] = stuffed[i];

            if(stuffed[i] == '1')
            {
                count++;

                if(count == 5)
                {
                    if(stuffed[i + 1] == '0')
                        i++;   // Skip stuffed bit

                    count = 0;
                }
            }
            else
            {
                count = 0;
            }
        }

        destuffed[j] = '\0';

        printf("\n\nReceiver Binary Output : %s",
               destuffed);

        if(!inputWasBinary)
        {
            binaryToText(destuffed, receivedText);

            printf("\nReceiver Text Output   : %s",
                   receivedText);
        }

        printf("\n");
    }

    return 0;
}
