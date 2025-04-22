#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://gist.github.com/mmozeiko/1561361cd4105749f80bb0b9223e9db8
#include "pcg64.h"
#include "hav_formula.h"

#define MIN_Y -90
#define MAX_Y 90
#define MAX_X 180
#define MIN_X -180

typedef uint64_t u64;

f64 generateXcoord(pcg64* rng)
{
	f64 rd =  (pcg64_nextd(rng) * 360) - 180;
	return rd;
}

f64 generateYcoord(pcg64* rng)
{
	f64 rd =  (pcg64_nextd(rng) * 180) - 90;
	return rd;
}


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stdout, "usage: hav_gen.out seed iterations\n");
		return 0;
	}

	u64 seed = atoi(argv[1]);
	if ((seed == 0) && (strcmp(argv[1],"0") != 0))
	{
		fprintf(stderr, "seed should be a number\n");
		return -1;
	}

	u64 itr = atoi(argv[2]);
	if ((itr == 0) && (strcmp(argv[2], "0") != 0))
	{
		fprintf(stderr, "itr should be greater than zero\n");
		return -1;
	}

	fprintf(stdout, "seed->%ld iterations->%ld\n", seed, itr);

	pcg64 rng;
	pcg64_seed(&rng, seed, seed);

	FILE* fptr = fopen("output.json", "w");
	fprintf(fptr, "{\n\"pair\":[\n");
	
	for (int i = 0; i < itr; i++)
	{
		f64 x0 = generateXcoord(&rng);
		f64 y0 = generateYcoord(&rng);
		f64 x1 = generateXcoord(&rng);
		f64 y1 = generateYcoord(&rng);

		fprintf(fptr, "\t{'x0':%lf,'y0':%lf,'x1':%lf,'y1':%lf},\n",
				x0,y0,x1,y1);
	}

	f64 x0 = generateXcoord(&rng);
	f64 y0 = generateYcoord(&rng);
	f64 x1 = generateXcoord(&rng);
	f64 y1 = generateYcoord(&rng);

	fprintf(fptr, "\t{'x0':%lf,'y0':%lf,'x1':%lf,'y1':%lf}\n",
			x0,y0,x1,y1);
	fprintf(fptr,"]\n}");
	return 0;
}
