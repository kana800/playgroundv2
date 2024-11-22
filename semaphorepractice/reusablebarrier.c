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


//**Phase 1:**

//1. Threads arrive at the barrier and attempt to lock `mutex1`. The first thread to arrive successfully locks `mutex1`.
//
//2. All other arriving threads block on the locked `mutex1` and cannot proceed.
//
//3. When the first thread (the one that locked `mutex1`) reaches the second phase, it unlocks `mutex1`. This action allows all blocked threads to proceed, including the last thread, which will go on to the second phase of the barrier.
//
//**Phase 2:**
//
//1. All threads, including the last one, enter the second phase of the barrier and attempt to lock `mutex2`, which is initially locked.
//
//2. The last thread to arrive at `mutex2` successfully locks it. Then, it unlocks `mutex2`, which permits `(NUM_THREADS - 1)` threads that are blocked on `mutex2` to proceed to the critical point.
//
//3. After all threads have passed through the critical point, `mutex2` is locked again.
//
//In summary, the barrier ensures that all threads reach the first phase and that only the first thread unlocks `mutex1`, allowing all threads to proceed. In the second phase, the last thread locks and unlocks `mutex2`, enabling `(NUM_THREADS - 1)` blocked threads to proceed. This ensures proper synchronization and relocks `mutex2` after all threads have passed through the barrier, making it ready for future synchronizations.
//THIS SOLUTION IS CALLED A TWO PHASE BARRIER

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define UPPERLIMIT 5

int count = 0;
pthread_mutex_t mutexlock1;
pthread_mutex_t mutexlock2;

pthread_t threadarr[UPPERLIMIT];


void* genericThread(void* parg)
{	
	pthread_mutex_lock(&mutexlock1);
	count = count + 1;

	if (count == UPPERLIMIT)
	{
		for (int i = 0; i < UPPERLIMIT - 1; i++)
		{
			printf("%d Thread : Rendezvous Completed\n", i);
			pthread_mutex_unlock(&mutexlock2);
		}
	}
	else 
	{
		pthread_mutex_unlock(&mutexlock1);
		pthread_mutex_lock(&mutexlock2);
	}
	printf("%ld Thread is At Critical Section\n", (long)parg);
	pthread_mutex_unlock(&mutexlock2);
	pthread_exit(0);
}

int main(int argc, char* argv[])
{
	if (pthread_mutex_init(&mutexlock1, NULL) != 0)
	{
		fprintf(stderr, "Mutex Initialization Failed\n");
		return 1;
	}

	if (pthread_mutex_init(&mutexlock2, NULL) != 0)
	{
		fprintf(stderr, "Mutex Initialization Failed\n");
		return 1;
	}
	
	for (long i = 0; i < UPPERLIMIT; i++)
	{
		pthread_create(&(threadarr[i]), NULL, genericThread, (void*) i);
	}

	for (int i = 0; i < UPPERLIMIT; i++)
	{
		pthread_join(threadarr[i], NULL);
	}

	pthread_mutex_destroy(&mutexlock1);
	pthread_mutex_destroy(&mutexlock2);

	return 0;
}
