// coded triangle number; problem 42
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int istriangle(int sum)
{
	// sum = 0.5n(n + 1);
	// 2 * sum = n(n + 1);
	// 2 * sum = n^2 + n
	// n^2 + n - 2 * sum = 0
	// x = ( -b + sqrt ( b^2 - 4ac )) / 2 * a;
	// x1 = ( -1 + sqrt ( 1^2 - 8 * sum)) / 2; 
	// x2 = ( -1 - sqrt ( 1^2 - 8 * sum)) / 2; 
	float x1 = ( -1 + sqrt( 1 - (-2 * 4 * sum))) / 2; 
	float x2 = ( -1 - sqrt( 1 - (-2 * 4 * sum))) / 2; 
	
//	if ((x1 % 1 == 0) || (x2 % 1 == 0))
//	{
//		return 1;
//	}
	
	if ((x1 < 0) && (x2 < 0))
	{
		return -1;
	}

	if ((x1 == (int)x1) || (x2 == (int)x2))
	{
		return 1;
	}
	return -1;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stdout, "usage: 'exp.out <filename>'\n");
		return -1;
	}

	FILE* fptr = fopen(argv[1], "rb");

	if (!fptr) {
		fprintf(stdout, "usage: 'sim.out <filename>'\n");
		return -1;
	}

	// getting the file size
	fseek(fptr, 0L, SEEK_END);
	int len = ftell(fptr);

	// making the buffer
	unsigned char* buffer = malloc(sizeof(unsigned char) * len);
	fseek(fptr, 0L, SEEK_SET);

	size_t bytesread_f = fread(buffer, sizeof(unsigned char), len, fptr);

	if (bytesread_f != len)
	{
		fprintf(stderr, 
			"short read of '%s': expected %d bytes\n", argv[1], len);
		return -1;
	}

	const char* delim = ",";
	char* token = strtok(buffer, delim);
	int i = 0;
	int tri = 0;
	while (token != NULL)
	{
		int tokenlength = strlen(token);
		int sum = 0;
		for (int i = 1; i < tokenlength - 1; i++)
		{
//			printf("\t%c %d", token[i], (int) token[i]);
			sum += (int)token[i] - 64;
		}
		int t = istriangle(sum);
		if (t == 1) tri += 1;
//		printf("%d %s -> %d %d\n", i, token, sum, t);
		token = strtok(NULL, delim);
		i++;
	}
	
	printf("total triangular num = %d\n", tri);

	assert(istriangle(55) == 1);
	assert(istriangle(1) == 1);
	assert(istriangle(6) == 1);
	assert(istriangle(10) == 1);
	assert(istriangle(12) == -1);

}
