#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define STRINGSIZE 100

// line read is completed
sem_t a1done;

int count;


void* readfilethread(void* parg)
{
	// post a signal that 
	// semaphore read is completed
	count++;
	sem_post(&a1done);
	pthread_exit(0);
}

void* printfilethread(void* parg)
{
	// wait for the read 
	// semaphore to send 
	// signal
	sem_wait(&a1done);
	fprintf(stdout, "%d\n", count);
	pthread_exit(0);
}

int main(int argc, char* argv[])
{
	count = 1;

	sem_init(&a1done, 0, 0);

	pthread_t t1, t2;
	
	pthread_create(&t2, NULL, printfilethread, NULL);
	pthread_create(&t1, NULL, readfilethread, NULL);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	sem_destroy(&a1done);

	return 0;
}
