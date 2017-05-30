
#ifndef _____timer___h__________________________
#define _____timer___h__________________________

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else // Unix based system specific
#include <sys/time.h>
#endif

#include <vector>

// Timer measures time in micro-second in both Windows, Linux and Unix system 

class Timer
{
public:
    Timer();                                    // default constructor
    ~Timer();                                   // default destructor

    void   start();                             // start timer
    void   stop();                              // stop the timer
    double getElapsedTime();                    // get elapsed time in second
    double getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
    double getElapsedTimeInMilliSec();          // get elapsed time in milli-second
    double getElapsedTimeInMicroSec();          // get elapsed time in micro-second

protected:

private:
    double startTimeInMicroSec;                 // starting time in micro-second
    double endTimeInMicroSec;                   // ending time in micro-second
    int    stopped;                             // stop flag 
#if defined(_WIN32) || defined(_WIN64)
    LARGE_INTEGER frequency;                    // ticks per second
    LARGE_INTEGER startCount;                   //
    LARGE_INTEGER endCount;                     //
#else
    timeval startCount;                         //
    timeval endCount;                           //
#endif
};

class FPSCounter
{
public:
	Timer _timer;
	int   _fcurr;
	int   _ftotal;
	std::vector<double> _buffer;
public:
	void reset(int nframes)
	{
		_buffer.resize(nframes,0);
		_buffer[0] = _timer.getElapsedTime();
		_fcurr     = 0;
		_ftotal    = 0;
	}
	void update()
	{
		_fcurr = (_fcurr+1) % _buffer.size();
		_buffer[_fcurr] = _timer.getElapsedTime();
		_ftotal++;
	}

	double getFPS() 
	{
		int n = _buffer.size();
		if( _ftotal>=n )
			return (n-1)/(_buffer[_fcurr] - _buffer[(_fcurr+n-1)%n]);
		else if( _fcurr>0 )
			return _fcurr/ (_buffer[_fcurr] - _buffer[0]);
		return 0;
	}

};

#endif // _____timer___h__________________________
