#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <unistd.h>


int main(int argc, char **argv) 
{

    if (argc < 2) return -1;

    FILE* fp = fopen(argv[1], "r");


    int arr1[1000] = {0};
    int arr2[1000] = {0};
    int part1[1000] = {0};

    const unsigned int len = 265;
    char buffer[len];
    char* tok;
    int linecount = 0;
    const char * delim = " ";

    while (fgets(buffer, len, fp))
    {
        tok = strtok(buffer, delim); 
        if (tok != NULL)
        {
            arr1[linecount] = atoi(tok);
            tok = strtok(NULL, delim);
        }
        if (tok != NULL) arr2[linecount] = atoi(tok);
        linecount++;
    }

    fclose(fp);
    
    // PRINTING FOR CHECKING
    for (int i = 0; i < 1000; i++)
    {
        printf("(%d) %d %d\n", i, arr1[i], arr2[i]);
    }

    // SORTING ARRAYS ( BUBBLE SORTING DONT KILL ME PLEASE )
    int i, j, temp;
    for(i = 0; i < 1000-1; i++) {
        for(j = 0; j < 1000-i-1; j++) {
            if(arr1[j] > arr1[j+1]) {
                temp = arr1[j];
                arr1[j] = arr1[j+1];
                arr1[j+1] = temp;
            }
            if(arr2[j] > arr2[j+1]) {
                temp = arr2[j];
                arr2[j] = arr2[j+1];
                arr2[j+1] = temp;
            }
        }
    }

    int total_distance = 0;
    // PRINTING FOR CHECKING
    for (int i = 0; i < 1000; i++)
    {
        part1[i] = abs(arr1[i] - arr2[i]);
        total_distance += part1[i];
    }

    printf("(P1) total distance %d\n", total_distance);

    int similarity_score = 0;
    int count = 0;
    for ( int i = 0; i < 1000; i++ )
    {
        for ( int j = 0; j < 1000; j++)
        {
            if (arr2[j] > arr1[i]) break;
            if (arr2[j] == arr1[i]) count++;
        }
        similarity_score += ( count * arr1[i] );
        count = 0;
    }

    printf("(P2) similarity score %d\n", similarity_score);
    return 0;
}
