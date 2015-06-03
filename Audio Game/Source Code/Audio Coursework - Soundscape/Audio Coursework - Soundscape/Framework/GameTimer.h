//=======================================================================================
// Stuart Milne
// Based on GameTimer.h by Frank Luna (C) 2008 All Rights Reserved
//=======================================================================================

// This high resolution timer uses the high resolution performance counter on the CPU
// This is accessed using the QueryPerformanceCounter/Frequency windows api calls.
// Note that some hardware (VERY unlikely in x86) may not have a performance counter onboard, in this case the timer will fail
// There is no fallback in this case
// See the comments in the constructor for how to begin addressing this limitation if it is a problem

// The performance counter operates as an incremental counter, that is it increments by 1 uniformly a specific
// number of times per second. By querying this frequency we can determine the time-step for a single increment
// Timing is then a case of determining the how many counts have passed in a period and multiplying this by the time-step
// Note all internal calculations are performed with double precision, downcasting to float is only performed on client code queries
// and does not affect internal timekeeping.

#ifndef GAMETIMER_H
#define GAMETIMER_H


class GameTimer
{
public:
	GameTimer();

	// GetElapsed returns the time since the last call to Reset()
	float GetElapsedTime() const;  // in seconds
	// GetDelta methods return time since last call to Tick()
	inline float GetDeltaTime() const { return static_cast<float>(mDeltaTime); };			// in seconds
	inline float GetDeltaTimeMS() const { return static_cast<float>(mDeltaTime * 1000); }	// in ms.

	void Reset(); // Call before message loop.
	void Start(); // Call to unpause a paused timer.
	void Stop();  // Call to pause timer
	void Tick();  // Call each frame in game loop

private:
	// Double precision internally
	double mSecondsPerCount;
	double mDeltaTime;

	// Internals for performance counter which is a 64bit integer
	__int64 mBaseCount;
	__int64 mPausedCount;
	__int64 mStopCount;
	__int64 mPreviousCount;
	__int64 mCurrentCount;

	bool mIsStopped;
};

#endif // GAMETIMER_H