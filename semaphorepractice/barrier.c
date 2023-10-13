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

// line read is completed
sem_t a1done;
sem_t b1done;

void* threadA(void* parg)
{
	printf("statement a1\n");
	sem_post(&a1done);
	sem_wait(&b1done);
	printf("statement a2\n");
}

void* threadB(void* parg)
{
	printf("statement b1\n");
	sem_post(&b1done);
	sem_wait(&a1done);
	printf("statement b2\n");
}

int main(int argc, char* argv[])
{

	sem_init(&a1done, 0, 0);
	sem_init(&b1done, 0, 0);

	pthread_t t1, t2;
	
	pthread_create(&t2, NULL, threadA, NULL);
	pthread_create(&t1, NULL, threadB, NULL);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	sem_destroy(&a1done);
	sem_destroy(&b1done);

	return 0;
}
