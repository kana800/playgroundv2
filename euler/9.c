// https://projecteuler.net/problem=9
#include <stdio.h>
	
int main(int argc, char **argv)
{
	for ( int a = 1; a <= 1000; a++)
	{
		for ( int b = 1; b <= 1000; b++)
		{
			for ( int c = 1; c <= 1000; c++)
			{
				if ((a*a) + (b*b) == (c*c))
				{
//					printf("%d^2 + %d^2 == %d^2\n", a,b,c);
					if ( a + b + c == 1000)
					printf("%d + %d == %d (%d)\n", a,b,c, a*b*c);

				}
			}
		}
	}

	return 0;
}

