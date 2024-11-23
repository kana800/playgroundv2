#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
 * ^ north
 * > east
 * < west
 * v south
 */

#define GRIDX 1000
#define GRIDY 1000

typedef struct __coord__ {
    int x;
    int y;
} coordinate_t ;

int main(int argc, char **argv) 
{

    if (argc < 2) return -1;

    FILE* fp = fopen(argv[1], "r");
    const unsigned int len = 256;
    char arrow;
    int arrgrid[GRIDX][GRIDY] = {{0}};

    coordinate_t santa = {GRIDX / 2, GRIDY / 2};
    coordinate_t robot = {GRIDX / 2, GRIDY / 2};

    int unique_grid = 1;
    arrgrid[santa.x][santa.y] = 1; 
    
    int count = 1;

    while ((arrow=fgetc(fp)) != EOF)
    {
        if (count % 2 == 0) {
            switch (arrow) 
            {
                case '^':
                    santa.y++;
                    break;
                case 'v':
                    santa.y--;
                    break;
                case '>':
                    santa.x++;
                    break;
                case '<':
                    santa.x--;
                    break;
            }
            if (santa.x >= 0 && santa.x < GRIDX && santa.y >= 0 && santa.y < GRIDY) {
                // house has not been visited
                if (arrgrid[santa.x][santa.y] == 0) {
                    unique_grid++;
                }
                // house as visited
                arrgrid[santa.x][santa.y]++;
            } 
        } 
        else 
        {
            switch (arrow) 
            {
                case '^':
                    robot.y++;
                    break;
                case 'v':
                    robot.y--;
                    break;
                case '>':
                    robot.x++;
                    break;
                case '<':
                    robot.x--;
                    break;
            }
            if (santa.x >= 0 && santa.x < GRIDX && santa.y >= 0 && santa.y < GRIDY) {
                // house has not been visited
                if (arrgrid[robot.x][robot.y] == 0) {
                    unique_grid++;
                }
                // house as visited
                arrgrid[robot.x][robot.y]++;
            } 
        }
        // total count
        count++;
    }

    fprintf(stdout, "santa coordinates (%d, %d)\n", santa.x, santa.y);
    fprintf(stdout, "unique houses %d\n", unique_grid);

    fclose(fp);
    return 0;
}
