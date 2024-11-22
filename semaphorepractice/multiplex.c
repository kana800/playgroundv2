// Generalize the previous solution so that
// it allows multiple threads to run in the 
// critical section at the same time, but it
// enforces an upper limit on the number of 
// concurrent threads.
// Thread A -> count = count + 1;
// Thread B -> count = count + 1;

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define UPPERLIMIT 5

int count;
pthread_mutex_t cntlock;
pthread_t threadarr[UPPERLIMIT];

void* genericThread()
{
	pthread_mutex_lock(&cntlock);
	count = count + 1;
	printf("Inside Thread -> count %d\n", count);
	pthread_mutex_unlock(&cntlock);
	pthread_exit(0);
}


int main(int argc, char* argv[])
{
	if (pthread_mutex_init(&cntlock, NULL) != 0)
	{
		fprintf(stderr, "Mutex Initialization Failed\n");
		return 1;
	}

	count = 0;

	for (int i = 0; i < UPPERLIMIT; i++)
	{
		pthread_create(&(threadarr[i]), NULL, genericThread, NULL);
	}

	for (int i = 0; i < UPPERLIMIT; i++)
	{
		pthread_join(threadarr[i], NULL);
	}

	pthread_mutex_destroy(&cntlock);
	return 0;
}
