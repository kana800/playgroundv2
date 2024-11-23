#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int min(int x, int y) {
    return (x < y) ? x : y;
}

int getSurfaceArea(int l, int w, int h)
{
    return 2*l*w + 2*w*h + 2*h*l;
}

int getSmallerSide(int l, int w, int h)
{
    int a = l*w;
    int b = w*h;
    int c = h*l;
    return min(min(a,b),c);
}

int getPerimeter(int l, int w, int h)
{
    int a = l+l+w+w;
    int b = w+w+h+h;
    int c = h+h+l+l;
    return min(min(a,b),c);
}

int getVolume(int l, int w, int h)
{
    return l*w*h;
}

int main(int argc, char **argv) 
{

    if (argc < 2) return -1;

    FILE* fp = fopen(argv[1], "r");
    const unsigned int len = 256;
    char buffer[len];
    char* token;
    int total;
    while (fgets(buffer, len, fp))
    {
        int arr[3];
        token = strtok(buffer, "x");
        int i = 0;
        while (token != NULL)
        {
            arr[i] = atoi(token);
            token = strtok(NULL, "x");
            i++;
        }
        // PART 1
        int surfacearea = getSurfaceArea(arr[0], arr[1], arr[2]);
        int smallestsize = getSmallerSide(arr[0], arr[1], arr[2]);
        //total += surfacearea + smallestsize;
        // PART 2
        int ribbonvolume = getVolume(arr[0], arr[1], arr[2]);
        int perimeter = getPerimeter(arr[0], arr[1], arr[2]);
        total += ribbonvolume + perimeter;
    }
    fprintf(stdout, "total sq feet: %d", total);
    fclose(fp);
    return 0;
}
