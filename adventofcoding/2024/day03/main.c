#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <unistd.h>


int main(int argc, char **argv) 
{

    if (argc < 2) return -1;

    FILE* fp = fopen(argv[1], "r");

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
    
    return 0;
}
