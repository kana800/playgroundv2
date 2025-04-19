#ifndef METRICS_H
#define METRICS_H

#include <x86intrin.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef uint64_t u64;
typedef double f64;

struct t_timedata
{
	char name[20];
	int idx;
	u64 OSStart;
	u64 OSEnd;
	u64 OSElapsed;
	u64 CPUStart;
	u64 CPUEnd;
	u64 CPUElapsed;
};

static int s_starttimeidx = 0;
static int s_endtimeidx = 0;
static struct t_timedata a_timedatamem[100];
static struct t_timedata s_time;

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

static void BeginTimer(char* name)
{
	struct t_timedata* temp = &a_timedatamem[s_starttimeidx];
	temp->OSStart = ReadOSTimer();
	temp->CPUStart = ReadCPUTimer();
	temp->idx = s_starttimeidx;
	int slen = strlen(name);
	memcpy(temp->name,name,slen);
	temp->name[slen + 1] = '\0';
	s_starttimeidx += 1;
}

static void EndTimer()
{
	assert(s_starttimeidx != 0);
	int idx = s_starttimeidx - s_endtimeidx;
	if (s_starttimeidx == 1) idx = 0;
	struct t_timedata* temp = &a_timedatamem[idx];
	temp->OSEnd = ReadOSTimer();
	temp->CPUEnd = ReadCPUTimer();
	temp->OSElapsed = temp->OSEnd - temp->OSStart;
	temp->CPUElapsed = temp->CPUEnd - temp->CPUStart;
	printf("%d(%s) Elapsed Time %llu %llu\n",
		idx, temp->name, 
		temp->OSElapsed, temp->CPUElapsed);
	s_endtimeidx += 1;
}


#endif // METRICS_H
