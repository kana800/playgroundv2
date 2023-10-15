// Generalize the signal pattern in sync.c so that it works 
// both ways. Thread A has to wait for Thread B and vice versa
// THREAD A | THREAD B 
// 	  a1 	| 	 b1 
// 	  a2 	|    b2

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

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
