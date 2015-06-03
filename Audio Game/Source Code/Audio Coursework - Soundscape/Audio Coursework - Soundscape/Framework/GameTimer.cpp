//=======================================================================================
// Stuart Milne.
// based on Game timer by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================
#include "GameTimer.h"
#include <windows.h>

GameTimer::GameTimer()
: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseCount(0), 
  mPausedCount(0), mPreviousCount(0), mCurrentCount(0), mIsStopped(false)
{
	// Find the granularity of the performance counter
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

	// Check for performance counter support, not implemented as we can assume modern x86 CPU's will support this
	// TODO - fall back if not supported, ie. GetTickCount or get_time
	//if((double)countsPerSec == 0) { PerformanceCounterNotSupportedOnHardwareNotify(); }
	
	// Calculate the time in seconds that each step of the performance counter represents
	mSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
}

// Returns the total time elapsed since reset() was called, NOT counting any
// time when the clock is stopped.
float GameTimer::GetElapsedTime() const
{
	// If we are stopped, do not count the time that has passed since we stopped.
	//
	// ----*---------------*------------------------------*------> time
	//  mBaseTime       mStopTime                      mCurrTime

	if( mIsStopped )
	{
		return static_cast<float>((mStopCount - mBaseCount) * mSecondsPerCount);
	}

	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from mCurrTime:  
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime
	
	else
	{
		return static_cast<float>( ( (mCurrentCount-mPausedCount)-mBaseCount ) * mSecondsPerCount );
	}
}

// Reset the timer, all times will start counting from 0
// Must be called at least once before use
void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseCount = currTime;
	mPreviousCount = currTime;
	mStopCount = 0;
	mIsStopped  = false;
}

// Only for starting a stopped timer
void GameTimer::Start()
{
	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     

	// Can only start a stopped timer
	if( mIsStopped )
	{
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
		// Keep track of how long we've been paused, cumulative since last call to Reset()
		mPausedCount += (startTime - mStopCount);	

		mPreviousCount = startTime;	// Begin counting again
		mStopCount = 0;
		mIsStopped  = false;
	}
}

void GameTimer::Stop()
{
	if( !mIsStopped )
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopCount = currTime;	// Start counting stop time
		mIsStopped  = true;
	}
}

void GameTimer::Tick()
{
	if( mIsStopped )
	{
		mDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrentCount = currTime;

	// We know how long a single count is, we know how many counts have passed since we last counted
	// Calculate time difference between this frame and the previous
	mDeltaTime = (mCurrentCount - mPreviousCount) * mSecondsPerCount;

	// Prepare for next frame.
	mPreviousCount = mCurrentCount;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if(mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}

