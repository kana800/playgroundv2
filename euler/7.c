/*listing the first 6 prime number we can see 2,3,5,7,11 and 13. 
 * we can see 13th is 6th prime. What is the 10001st prime number*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "primes.h"

void sieveOfEratosthenes(int n)
{
    bool prime[n + 1];
    memset(prime, true, sizeof(prime));

    for (int p = 2; p * p <= n; p++) {
        if (prime[p] == true) {
            for (int i = p * p; i <= n; i += p)
                prime[i] = false;
        }
    }

    for ( int i = 2; i <= n ; i++)
        if (prime[i]) {
            printf("%d,", i);
        }
}

int main(int argc, char** argv)
{
    printf("%d", GETPRIME(10001));
}
