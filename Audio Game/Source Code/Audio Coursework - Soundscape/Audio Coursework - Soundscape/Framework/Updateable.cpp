/*
	file:	Updateable.cpp
	Version:	2.2
	Date:	14th May 2013; 2012.
	Author:	Allan C. Milne.

	Namespace:	AllanMilne.
	Exposes:	implementation of Updateable.

	Description:
	this is the implementation file for the Updateable class.
	* see Updateable.hpp for details of its behaviour.

*/
#include "Updateable.hpp"

namespace AllanMilne {

//--- Returns true if OnUpdate() should be called, false otherwise.
//--- See .hpp header comments for how frame rate and time measures interact in getting the return value.
bool Updateable::IsTimeToUpdate () 
{
	// if both measures are 0 then return indicating that we should update.
	if (mFrameRate<=0 && mFrameTime<=0.0f) return true;
	// Check if frame rate is to be used and if it has been reached.
	if (mFrameRate>0 && mFrameCount>=mFrameRate) return true;
	// check if frame time is to be used an dif it has been reached.
	if (mFrameTime>0.0f && mTimeElapsed>=mFrameTime) return true;
	// return indicating that neither of the measures have been reached and no updating should be performed.
	return false;
} // end IsTimeToUpdate function.

//--- Call every frame in the client application to control updating for the concrete subclass.
void Updateable::Update (const float deltaTime)
{
	// Only applied if this object is active for updating.
	if (!IsUpdateActive() ) return;
	
	// increment internal measures.
	++mFrameCount;
	mTimeElapsed += deltaTime;
	if (IsTimeToUpdate()) 
	{
		// parameter is the time since OnUpdate was last called, not the time since this function was last called.
		OnUpdate(mTimeElapsed);
		// Reset measure counts.
		mFrameCount = 0;
		mTimeElapsed = 0.0f;
	}
} // end Update function.

//--- Controlling the active status of the update.
//--- Call appropriate event function, but only if the active status has changed.

void Updateable::ActivateUpdate ()
{
	if (mActive) return;
	mActive = true;
	OnUpdateActivation();
	// Set frame measures to force OnUpdate at the next frame.
	mFrameCount = mFrameRate;
	mTimeElapsed = mFrameTime;
} // end ActivateUpdate function.

void Updateable::DeactivateUpdate ()
{
	if (!mActive) return;
	mActive = false;
	OnUpdateDeactivation();
} // end DeactivateUpdate function.

void Updateable::ToggleUpdate ()
{
	if (mActive)
		DeactivateUpdate();
	else ActivateUpdate();
} // end ToggleUpdate function.

//--- Setting update measures; don't allow invalid values.
void Updateable::SetUpdateFrameRate (const int aRate)
{
	if (aRate >= 0)
		mFrameRate = aRate;
} // end SetUpdateFrameRate function.

void Updateable::SetUpdateFrameTime (const float aTime)
{
	if (aTime >= 0.0f)
		mFrameTime = aTime;
} // end SetUpdateFrametime function.

} // end AllanMilne namespace.