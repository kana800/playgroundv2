// Generalize the rendezvour soltuion; Every thread should run the 
// following code : 
// 		rendezvous
// 		critical point
// The synchronization requirement is that no thread executes critical point 
// until after all threads have executed rendezvous. You can assume that there 
// are n threads and that this value is stored in a variable n that is accessible
// from all threads.
// When the first n-1 threads arrrive they should block until the nth thread arrives 
// at which point all threads may proceed.

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define UPPERLIMIT 5

int count = 0;
pthread_mutex_t cntlock;
pthread_t threadarr[UPPERLIMIT];

sem_t rendezvous;

void* genericThread(void* parg)
{	
	pthread_mutex_lock(&cntlock);
	count = count + 1;

	if (count == UPPERLIMIT)
	{
		for (int i = 0; i < UPPERLIMIT - 1; i++)
		{
			printf("%d Thread : Rendezvous Completed\n", i);
			sem_post(&rendezvous);
		}
	}

	printf("%ld Thread is At Critical Section\n", (long)parg);
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
	
	sem_init(&rendezvous, 0 ,0);
	
	for (long i = 0; i < UPPERLIMIT; i++)
	{
		pthread_create(&(threadarr[i]), NULL, genericThread, (void*) i);
	}

	for (int i = 0; i < UPPERLIMIT; i++)
	{
		pthread_join(threadarr[i], NULL);
	}

	pthread_mutex_destroy(&cntlock);
	sem_destroy(&rendezvous);

	return 0;
}
