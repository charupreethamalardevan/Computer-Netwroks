#include <stdio.h>
#include <string.h>

int isBinary(char str[])
{
    int i;

    if(str[0] == '\0')
        return 0;

    for(i = 0; str[i] != '\0'; i++)
    {
        if(str[i] != '0' && str[i] != '1')
            return 0;
    }

    return 1;
}

int main()
{
    char input[100];
    char binary[1000] = "";
    char stuffed[1500];
    char destuffed[1000];
    char ch;

    int i, j, count;
    int pos, len;

    printf("Enter Data : ");
    scanf("%99s", input);

    
    if(isBinary(input))
    {
        strcpy(binary, input);
        printf("\nInput Type       : Binary");
    }
    else
    {
        printf("\nInput Type       : Text");

        for(i = 0; input[i] != '\0'; i++)
        {
            for(j = 7; j >= 0; j--)
            {
                int l = strlen(binary);
                binary[l] = ((input[i] >> j) & 1) + '0';
                binary[l + 1] = '\0';
            }
        }
    }

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

   
    printf("\nDo you want to induce an error? (Y/N) : ");
    scanf(" %c", &ch);

    if(ch == 'Y' || ch == 'y')
    {
        len = strlen(stuffed);

        printf("Enter Bit Position (1-%d) : ", len);
        scanf("%d", &pos);

        if(pos >= 1 && pos <= len)
        {
            char oldBit = stuffed[pos - 1];

            if(stuffed[pos - 1] == '0')
                stuffed[pos - 1] = '1';
            else
                stuffed[pos - 1] = '0';

            printf("\nBit at Position %d changed from %c to %c",
                   pos, oldBit, stuffed[pos - 1]);

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
                    i++;   // Skip stuffed 0
                    count = 0;
                }
            }
            else
            {
                count = 0;
            }
        }

        destuffed[j] = '\0';

        printf("\n\nReceiver Output  : %s\n", destuffed);
    }

    return 0;
}
