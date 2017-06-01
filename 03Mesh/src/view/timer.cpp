
#include <stdlib.h>
#include "timer.h"

Timer::Timer()
{
#if defined(_WIN32) || defined(_WIN64)
    QueryPerformanceFrequency(&frequency);
    startCount.QuadPart = 0;
    endCount.QuadPart = 0;
#else
    startCount.tv_sec = startCount.tv_usec = 0;
    endCount.tv_sec = endCount.tv_usec = 0;
#endif

    stopped = 0;
    startTimeInMicroSec = 0;
    endTimeInMicroSec = 0;
}


Timer::~Timer()
{
}


void Timer::start()
{
    stopped = 0; // reset stop flag
#if defined(_WIN32) || defined(_WIN64)
    QueryPerformanceCounter(&startCount);
#else
    gettimeofday(&startCount, NULL);
#endif
}

void Timer::stop()
{
    stopped = 1; // set timer stopped flag

#if defined(_WIN32) || defined(_WIN64)
    QueryPerformanceCounter(&endCount);
#else
    gettimeofday(&endCount, NULL);
#endif
}

double Timer::getElapsedTimeInMicroSec()
{
#if defined(_WIN32) || defined(_WIN64)
    if(!stopped)
        QueryPerformanceCounter(&endCount);

    startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
    endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
    if(!stopped)
        gettimeofday(&endCount, NULL);

    startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
    endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif

    return endTimeInMicroSec - startTimeInMicroSec;
}


double Timer::getElapsedTimeInMilliSec()
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}


double Timer::getElapsedTimeInSec()
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}


double Timer::getElapsedTime()
{
    return this->getElapsedTimeInSec();
}

double getTimeInMillSecond()
{
#if defined(_WIN32) || defined(_WIN64)
	LARGE_INTEGER startCount;
	LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startCount);
	return (startCount.QuadPart * (1000.0 / frequency.QuadPart));
#else
	timeval startCount;
	gettimeofday(&startCount, NULL);
    return (startCount.tv_sec * 1000.0 + startCount.tv_usec/1000.0);
#endif
}

double getTimeInMicroSecond()
{
#if defined(_WIN32) || defined(_WIN64)
	LARGE_INTEGER startCount;
	LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startCount);
	return (startCount.QuadPart * (1000000.0 / frequency.QuadPart));
#else
	timeval startCount;
	gettimeofday(&startCount, NULL);
    return (startCount.tv_sec * 1000000.0 + startCount.tv_usec);
#endif
}

int getTicks()
{
	double t = getTimeInMicroSecond();
	int b = 0x7FFFFFF;
	t -= b * (double)(int)(t/b);
	return (int)t;
}

