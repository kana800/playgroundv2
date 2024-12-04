#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <unistd.h>

void printarr(int* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) 
{

    if (argc < 2) return -1;

    FILE* fp = fopen(argv[1], "r");

    int arr1[1000] = {0};

    const unsigned int len = 265;
    char buffer[len];
    char* tok;
    int count = 0;
    const char * delim = " ";

    int prev, flip, flop = 0; 
    int diff, amount = 0;
    int safe = 0;
    int bad = 0;
    int bad_level = 0;
    while (fgets(buffer, len, fp))
    {
        tok = strtok(buffer, delim); 
        while (tok != NULL)
        {
            arr1[count] = atoi(tok);
            tok = strtok(NULL, delim);
            count++;
        }
        for (int i = 1; i < count; ++i)
        {
            diff = abs(arr1[i] - arr1[i - 1]);
            if ((diff < 1) || (diff >= 3))
            {
                bad += 1;
                bad_level = i;
                break;
            }
            if (arr1[i] > arr1[i - 1])
            {
                flip = 1;
            } else if (arr1[i] < arr1[i - 1])
            {
                flop = 1;
            }
            if ((flip == 1) && (flop == 1))
            {
                bad += 1;
                bad_level = i - 1;
                break;
            }
        }
        if (bad == 0)
        {
            safe += 1;
        }
        else if (bad == 1)
        {
            int* arr = malloc(sizeof(int) * (count - 1));
            for (int i = 0; i < bad_level; i++)
                arr[i] = arr1[i];
            for (int i = bad_level + 1; i < count ; ++i)
                arr[i - 1] = arr1[i];
            //printarr(arr1, count);
            //printarr(arr, count - 1);
            bad = 0, flip = 0, flop = 0;
            for (int i = 1; i < (count - 1); ++i)
            {
                diff = abs(arr[i] - arr[i - 1]);
                if ((diff < 1) || (diff >= 3))
                {
                    bad += 1;
                    break;
                }
                if (arr[i] > arr[i - 1])
                {
                    flip = 1;
                } else if (arr[i] < arr[i - 1])
                {
                    flop = 1;
                }
                if ((flip == 1) && (flop == 1))
                {
                    bad += 1;
                    break;
                }
            }
            if (bad == 0){
                safe += 1;
            }
           
            free(arr);
        }
        bad = 0, bad_level = 0, count = 0, flip = 0, flop = 0;
    }

    printf("safe reports %d\n", safe);

    fclose(fp);
    
    return 0;
}
