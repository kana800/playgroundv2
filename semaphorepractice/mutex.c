// Add semaphore to the following example to enfore mutual exclusion 
// to the shared variable count
// Thread A -> count = count + 1;
// Thread B -> count = count + 1;

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int count;
pthread_mutex_t cntlock;

void* threadA()
{
	pthread_mutex_lock(&cntlock);
	count = count + 1;
	printf("Inside Thread A -> count %d\n", count);
	pthread_mutex_unlock(&cntlock);
}

void* threadB()
{
	pthread_mutex_lock(&cntlock);
	count = count + 1;
	printf("Inside Thread B -> count %d\n", count);
	pthread_mutex_unlock(&cntlock);
}


int main(int argc, char* argv[])
{
	if (pthread_mutex_init(&cntlock, NULL) != 0)
	{
		fprintf(stderr, "Mutex Initialization Failed\n");
		return 1;
	}

	count  = 0;

	pthread_t t1;
	pthread_t t2;

	pthread_create(&t1, NULL, threadA, NULL);
	pthread_create(&t2, NULL, threadA, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	pthread_mutex_destroy(&cntlock);
	return 0;

}
