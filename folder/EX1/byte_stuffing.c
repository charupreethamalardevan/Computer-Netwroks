#include <stdio.h>
#include <string.h>

int main()
{
    char data[100], stuffed[200], framed[250], destuffed[100];
    int i, j = 0, k = 0;

    printf("Enter Data: ");
    scanf("%s", data);

    printf("\nOriginal Data : %s\n", data);



    for(i = 0; data[i] != '\0'; i++)
    {
        if(data[i] == 'F' || data[i] == 'E')
        {
            stuffed[j++] = 'E';
        }

        stuffed[j++] = data[i];
    }

    stuffed[j] = '\0';

    printf("Stuffed Data  : %s\n", stuffed);



    framed[k++] = 'F';

    for(i = 0; stuffed[i] != '\0'; i++)
        framed[k++] = stuffed[i];

    framed[k++] = 'F';
    framed[k] = '\0';

    printf("Framed Data   : %s\n", framed);



    j = 0;

    for(i = 1; i < k - 1; i++)
    {
        if(framed[i] == 'E')
        {
            i++;
        }

        destuffed[j++] = framed[i];
    }

    destuffed[j] = '\0';

    printf("Destuffed Data: %s\n", destuffed);



    if(strcmp(data, destuffed) == 0)
        printf("\nDestuffing Successful\n");
    else
        printf("\nData Mismatch\n");

    return 0;
}
