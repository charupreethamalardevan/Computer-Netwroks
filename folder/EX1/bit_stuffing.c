#include <stdio.h>

#define MAX 200

void printBits(int arr[], int n)
{
    for(int i=0;i<n;i++)
        printf("%d",arr[i]);
    printf("\n");
}

int main()
{
    int data[MAX], stuffed[MAX], framed[MAX], destuffed[MAX];
    int flag[8]={0,1,1,1,1,1,1,0};

    int n,i,j=0,ones=0;

    printf("Enter number of bits: ");
    scanf("%d",&n);

    printf("Enter data bits (0/1):\n");
    for(i=0;i<n;i++)
        scanf("%d",&data[i]);

    printf("\nOriginal Data : ");
    printBits(data,n);



    ones=0;
    j=0;

    for(i=0;i<n;i++)
    {
        stuffed[j++]=data[i];

        if(data[i]==1)
            ones++;
        else
            ones=0;

        if(ones==5)
        {
            stuffed[j++]=0;
            ones=0;
        }
    }

    int stuffedLen=j;

    printf("Stuffed Data  : ");
    printBits(stuffed,stuffedLen);



    int k=0;

    for(i=0;i<8;i++)
        framed[k++]=flag[i];

    for(i=0;i<stuffedLen;i++)
        framed[k++]=stuffed[i];

    for(i=0;i<8;i++)
        framed[k++]=flag[i];

    int frameLen=k;

    printf("Framed Data   : ");
    printBits(framed,frameLen);



    j=0;
    ones=0;

    for(i=8;i<frameLen-8;i++)
    {
        if(framed[i]==1)
        {
            destuffed[j++]=1;
            ones++;

            if(ones==5)
            {
                i++;
                ones=0;
            }
        }
        else
        {
            destuffed[j++]=0;
            ones=0;
        }
    }

    int destuffLen=j;

    printf("Destuffed Data: ");
    printBits(destuffed,destuffLen);



    int match=1;

    if(destuffLen!=n)
        match=0;
    else
    {
        for(i=0;i<n;i++)
        {
            if(data[i]!=destuffed[i])
            {
                match=0;
                break;
            }
        }
    }

    if(match)
        printf("\nDestuffing Successful\n");
    else
        printf("\nData Mismatch\n");

    return 0;
}
