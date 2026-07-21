#include <stdio.h>
#include <string.h>

#define FLAG 126
#define ESC 163

void printAsBinary(int value)
{
    for(int i=7;i>=0;i--)
        printf("%d",(value>>i)&1);
    printf(" ");
}

int main()
{
    char text[100];
    int data[100];
    int frame[200];
    int result[100];

    char errChoice;
    int errType;
    int bytePos, bitPos;

    printf("Enter text (Use 'F' for FLAG and 'E' for ESC): ");
    scanf("%s",text);

    int dataLen=strlen(text);

    for(int i=0;i<dataLen;i++)
    {
        if(text[i]=='F')
            data[i]=FLAG;
        else if(text[i]=='E')
            data[i]=ESC;
        else
            data[i]=text[i];
    }

    printf("\n=== TRANSMITTER SIDE ===\n");
    printf("Original Text: %s\n\n",text);

    printf("--- Text to ASCII to Binary Conversion ---\n");
    for(int i=0;i<dataLen;i++)
    {
        printf("Character '%c' -> ASCII Decimal: %3d -> Binary: ",text[i],data[i]);
        printAsBinary(data[i]);
        printf("\n");
    }

    int fIdx=0;

    frame[fIdx++]=FLAG;

    for(int i=0;i<dataLen;i++)
    {
        if(data[i]==FLAG)
        {
            frame[fIdx++]=ESC;
            frame[fIdx++]=FLAG;
        }
        else if(data[i]==ESC)
        {
            frame[fIdx++]=ESC;
            frame[fIdx++]=ESC;
        }
        else
            frame[fIdx++]=data[i];
    }

    frame[fIdx++]=FLAG;

    printf("\n--- Final Framed Packet Output ---\n");

    printf("Framed Characters   : ");
    for(int i=0;i<fIdx;i++)
    {
        if(frame[i]==FLAG)
            printf("F        ");
        else if(frame[i]==ESC)
            printf("E        ");
        else
            printf("%c        ",frame[i]);
    }

    printf("\nFramed Packet ASCII : ");
    for(int i=0;i<fIdx;i++)
        printf("%-8d ",frame[i]);

    printf("\nFramed Packet Binary: ");
    for(int i=0;i<fIdx;i++)
        printAsBinary(frame[i]);
    printf("\n");

    /* -------- ERROR INDUCTION -------- */

    printf("\nDo you want to induce an error? (Y/N): ");
    scanf(" %c",&errChoice);

    if(errChoice=='Y' || errChoice=='y')
    {
        printf("\n1. Bit Level Error");
        printf("\n2. Byte Level Error");
        printf("\nEnter your choice: ");
        scanf("%d",&errType);

        if(errType==1)
        {
            printf("Enter Byte Position (1-%d): ",fIdx);
            scanf("%d",&bytePos);

            printf("Enter Bit Position (1-8): ");
            scanf("%d",&bitPos);

            if(bytePos>=1 && bytePos<=fIdx && bitPos>=1 && bitPos<=8)
            {
                frame[bytePos-1]^=(1<<(8-bitPos));

                printf("\nBit Level Error Induced Successfully.\n");
            }
            else
            {
                printf("\nInvalid Position.\n");
                return 0;
            }
        }
        else if(errType==2)
        {
            printf("Enter Byte Position (1-%d): ",fIdx);
            scanf("%d",&bytePos);

            if(bytePos>=1 && bytePos<=fIdx)
            {
                frame[bytePos-1]^=0xFF;

                printf("\nByte Level Error Induced Successfully.\n");
            }
            else
            {
                printf("\nInvalid Position.\n");
                return 0;
            }
        }
        else
        {
            printf("\nInvalid Choice.\n");
            return 0;
        }

        printf("\n=== FRAME AFTER ERROR ===\n");

        printf("Framed Characters   : ");
        for(int i=0;i<fIdx;i++)
        {
            if(frame[i]==FLAG)
                printf("F        ");
            else if(frame[i]==ESC)
                printf("E        ");
            else if(frame[i]>=32 && frame[i]<=126)
                printf("%c        ",frame[i]);
            else
                printf("?        ");
        }

        printf("\nFramed Packet ASCII : ");
        for(int i=0;i<fIdx;i++)
            printf("%-8d ",frame[i]);

        printf("\nFramed Packet Binary: ");
        for(int i=0;i<fIdx;i++)
            printAsBinary(frame[i]);

        printf("\n");
        printf("\nTransmission Error Detected");
        printf("\nFrame Discarded");
        printf("\nDe-Stuffing Not Performed\n");

        return 0;
    }

    printf("\n=== RECEIVER SIDE ===\n");

    int rIdx=0;

    for(int i=1;i<fIdx-1;i++)
    {
        if(frame[i]==ESC)
        {
            i++;
            result[rIdx++]=frame[i];
        }
        else
            result[rIdx++]=frame[i];
    }

    printf("\n=== FINAL RESULT ===\n");

    printf("Correct Destuffed Ans: ");
    for(int i=0;i<rIdx;i++)
    {
        if(result[i]==FLAG)
            printf("F");
        else if(result[i]==ESC)
            printf("E");
        else
            printf("%c",result[i]);
    }

    printf("\n");

    printf("Final Clean ASCII    : ");
    for(int i=0;i<rIdx;i++)
        printf("%d ",result[i]);

    printf("\nFinal Clean Binary   : ");
    for(int i=0;i<rIdx;i++)
        printAsBinary(result[i]);

    printf("\n");

    return 0;
}
