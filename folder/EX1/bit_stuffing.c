#include <stdio.h>
#include <string.h>

int main()
{
    char data[100], stuffed[150], destuffed[100];
    char ch;
    int i, j = 0, count = 0;
    int pos, len;

    printf("Enter Binary Data : ");
    scanf("%s", data);

    for(i = 0; data[i] != '\0'; i++)
    {
        stuffed[j++] = data[i];

        if(data[i] == '1')
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

    printf("\nOriginal Data      : %s", data);
    printf("\nStuffed Data       : %s", stuffed);

    printf("\n\nTransmitted Frame  : ");
    printf("01111110 %s 01111110\n", stuffed);

    printf("\nDo you want to induce an error? (Y/N) : ");
    scanf(" %c", &ch);

    if(ch == 'Y' || ch == 'y')
    {
        len = strlen(stuffed);

        printf("Enter Bit Position (1-%d) : ", len);
        scanf("%d", &pos);

        if(pos >= 1 && pos <= len)
        {
            if(stuffed[pos - 1] == '0')
            {
                stuffed[pos - 1] = '1';
                printf("\nBit at Position %d changed from 0 to 1", pos);
            }
            else
            {
                stuffed[pos - 1] = '0';
                printf("\nBit at Position %d changed from 1 to 0", pos);
            }

            printf("\n\nFrame After Error  : %s", stuffed);

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
                    i++;        // Skip stuffed 0
                    count = 0;
                }
            }
            else
            {
                count = 0;
            }
        }

        destuffed[j] = '\0';

        printf("\n\nReceiver Output    : %s\n", destuffed);
    }

    return 0;
}
