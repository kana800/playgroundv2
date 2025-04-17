// https://projecteuler.net/problem=44
#include <stdio.h>
#include <math.h>
// #include "external/gmp-6.3.0/gmp.h"


long long int pentagonalNumber(int n)
{
	return (n * ((3 * n) - 1)) / 2;
}

int isPentagonal(long long int a)
{
	long double sq = sqrt(1 + (24 * a));
	if (sq < 0) return -1;
	long double x1 = (1 + sq) / 6;
	long double x2 = (1 - sq) / 6;
	if ((fmod(x1,1) != 0) && (fmod(x2,1) != 0)) return -1;
	return 1;
}

int main(int argc, char* argv[])
{
	long long int len = 100000;
//	mpz_t pn[len];
	long long int pn[len];

//	mpz_t tp, mult;
//
//	mpz_init(tp);
//	mpz_init(mult);
//	
//	mpz_t c;
//	mpz_init(c);

	for (int i = 1; i < len + 1 ; i++)
	{
		pn[i] = pentagonalNumber(i);
//		long long int tp = pentagonalNumber(i);
//		tp = i * ((3 * i) - 1) / 2;
//		mpz_init2(pn[i], tp);
//		gmp_printf("P%d = %Zd\n", i, pn[i]);
	}


	for (int i = 1; i < len + 1; i++)
	{
		for (int j = 1; j < len + 1; j++)
		{
			long long int s = pn[i] + pn[j];
			long long int d = pn[i] - pn[j];

			if ((isPentagonal(s) == 1) && isPentagonal(d) == 1)
			{
				printf("%ld + %ld = %ld are pentagonal\n", 
					s, d, pn[j] - pn[i]);
				break;
			}
		}
	}

	return 0;
}
