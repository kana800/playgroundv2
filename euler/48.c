#include <stdio.h>
#include <math.h>
#include "external/gmp-6.3.0/gmp.h"

int main(int argc, char* argv[])
{
//	unsigned long long sum = 0;
	mpz_t sum, mult;
	mpz_init(sum);
	mpz_init(mult);
	
	for(int i = 0; i <= 1000; i++)
	{
		mpz_ui_pow_ui(mult,i,i);
		mpz_add(sum,sum,mult);
		gmp_printf("%d^%d = %Zd\n",i,i,mult);
	}
	gmp_printf("sum = %Zd\n", sum);


	return 0;
}
