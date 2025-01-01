/* Given a list of numbers and a number k
 * return whether any two numbers from the list add up to k. 
 * For example, given [10, 15, 3, 7] and k of 17, 
 * return true since 10 + 7 is 17.*/
#include <stdio.h>

#define arrlen(arr) (sizeof((arr))/sizeof(arr[0]))


int main(int argc, char* argv[])
{
    // iterating through a for loop
    int t1[] = {10,15,3,7};
    int t2[] = {10,15,3,7,8};

    // O(n2) solution
    for (int i = 0; i < arrlen(t1); i++)
    {
        for ( int j = 0; i < arrlen(t1); i++) 
        {
            if ((t1[i]+t1[j]) == 17) {
                printf("%d+%d=%d\n",t1[i],t1[j],17);
                break;
            }
        }

    }

    return 0;
}
