#include <stdio.h>

#ifdef __AVX__
  #include <immintrin.h>
#else
  #warning No AVX support - will not compile
#endif

void printarr(float* arr, int s)
{
	for (int i = 0; i < s; i++)
	{
		printf("%d %.2f \n", i, arr[i]);
	}
	printf("\n");
}

int main(int argc, char **argv)
{
    __m256 a = _mm256_set_ps(8.0, 7.0, 6.0, 5.0, 
                             4.0, 3.0, 2.0, 1.0);
    __m256 b = _mm256_set_ps(18.0, 17.0, 16.0, 15.0, 
                             14.0, 13.0, 12.0, 11.0);

    __m256 c = _mm256_add_ps(a, b);

    float d[8];
    _mm256_storeu_ps(d, c);

	printarr(d, 8);


    return 0;
}
