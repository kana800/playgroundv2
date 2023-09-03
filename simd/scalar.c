#include <stdio.h>

void printarr(int* arr, int s)
{
	for (int i = 0; i < s; i++)
	{
		printf("%d %d \n", i, arr[i]);
	}
	printf("\n");
}


int main(int argc, char * argv[])
{
	int a[4] = { 1, 3, 5, 7 };
	int b[4] = { 2, 4, 6, 8 };
	int c[4];

	c[0] = a[0] + b[0];     // 1 + 2
	c[1] = a[1] + b[1];     // 3 + 4
	c[2] = a[2] + b[2];     // 5 + 6
	c[3] = a[3] + a[3];     // 7 + 8
	
	printarr(c, 4);
}
