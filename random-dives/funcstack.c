// tiny example for a function pointer array
#include <stdio.h>

int globalsnack = 0;

void add()
{
	globalsnack++;
}

void sub()
{
	globalsnack--;
}

void disp()
{
	printf("%d\n",globalsnack);
}

int main(int argc, char* argv[])
{
//	void* fstack[2] = { &add, &sub };
//	int (*func)(int, int) = fstack[1];
//	printf("%d\n", (*func)(1,2));

	void (*farray[5])();

	farray[0] = add;
	farray[1] = add;
	farray[2] = add;
	farray[3] = sub;
	farray[4] = disp;


	for (int i = 0; i < 5; i++)
	{
		(*farray[i])();
	}

	return 0;
}
