#ifndef METRICS_H
#define METRICS_H

#include <x86intrin.h>
#include <sys/time.h>
#include <stdint.h>
//#include <cstdint>

typedef uint64_t u64;
typedef double f64;

static u64 GetOSTimerFreq(void)
{
	return 1000000;
}

static u64 ReadOSTimer(void)
{
	// NOTE(casey): The "struct" keyword is not necessary here when compiling in C++,
	// but just in case anyone is using this file from C, I include it.
	struct timeval Value;
	gettimeofday(&Value, 0);
	
	u64 Result = GetOSTimerFreq()*(u64)Value.tv_sec + (u64)Value.tv_usec;
	return Result;
}


/* NOTE(casey): This does not need to be "inline", it could just be "static"
   because compilers will inline it anyway. But compilers will warn about 
   static functions that aren't used. So "inline" is just the simplest way 
   to tell them to stop complaining about that. */
static inline u64 ReadCPUTimer(void)
{
	// NOTE(casey): If you were on ARM, you would need to replace __rdtsc
	// with one of their performance counter read instructions, depending
	// on which ones are available on your platform.
	
	return __rdtsc();
}

static u64 guessCPUFrequency(long mstowait)
{
	u64 OSFreq = GetOSTimerFreq();
	u64 CPUStart = ReadCPUTimer();
	u64 OSStart = ReadOSTimer();

	u64 OSEnd, OSElapsed = 0;
	u64 OSWaitTime = OSFreq* mstowait / 1000;
	
	while (OSElapsed < OSWaitTime)
	{
		OSEnd = ReadOSTimer();
		OSElapsed = OSEnd - OSStart;
	}

	u64 CPUEnd = ReadCPUTimer();
	u64 CPUElapsed = CPUEnd - CPUStart;
	u64 CPUFreq = 0;

	if (OSElapsed)
	{
		CPUFreq = OSFreq * CPUElapsed / OSElapsed;
	}

	return CPUFreq;
}


static :BeginTimer()



#endif // METRICS_H
