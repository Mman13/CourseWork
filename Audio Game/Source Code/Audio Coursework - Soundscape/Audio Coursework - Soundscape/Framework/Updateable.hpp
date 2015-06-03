/*
	file:	Updateable.hpp
	Version:	2.2
	Date:	14th May 2013; 2012.
	Author:	Allan C. Milne.

	Namespace:	AllanMilne.
	Exposes:	Updateable.

	Description:
	this defines an abstract class exposing behaviour for implementing model updating on a frame by frame basis.
	It is inherited by a concrete client subclass that must implement the abstract OnUpdate(...) function
	- this defines the frame processing required by the client model.
	The client subclass may also optionally override the virtual OnUpdateActivation() and OnUpdateDeactivation() functions
	- these provide stubs for including processing when the update state is changed;
	- if not overidden then their default action is to do nothing.

	Update() is available to be called from frame processing by the client on any object inheriting from Updateable;
	- this calls the OnUpdate(..) function implementing the processing relevant to the entity being modelled.
	While Update() is called every processing frame, the OnUpdate() method call can be controlled by
	- indicating whether this object is active or inactive for updating;
	- defining a target frame rate and/or frame time specifying that it should be called only every few frames.
	- NOTE: Update() -must- be called by the calling loop every frame to ensure correct behaviour - timing and 
	frame counting will only be valid if Update has been called each frame.

	The OnUpdateActivation() and OnUpdateDeactivation() functions can be defined in the client subclass
	to perform processing as the active state changes.
	If these member functions are not overidden then the default action defined in this base class is to do nothing.

	The frame rate is the number of frames that should pass before OnUpdate() is called.
	The frame time is the time in seconds to elapse before OnUpdate() is called.
These work together as follows:
	both = 0	update every frame.
	one = 0		use the other measure.
	both != 0	use whichever is reached first.

*/

#ifndef __UPDATEABLE_HPP__
#define __UPDATEABLE_HPP__

namespace AllanMilne {

class Updateable
{
public:

	//--- Call every frame in the client application to control updating for the concrete subclass.
	void Update (const float deltaTime);
	
	//--- Abstract (pure virtual) function - MUST be defined in the concrete subclass.
	//--- Called from Update() when updating is active and the target frame rate or time is past.
	//--- Defines the client frame processing to be performed.
	//--- Parameter is the time in seconds since this function was last called.
	virtual void OnUpdate (const float deltaTime) = 0;

	//--- the following functions may be overidden in the client subclass to take relevant action on the active status changing.
	//--- these default implementations do nothing.
//--- Called when the active status of update is changed.
	virtual void OnUpdateActivation () { }
	virtual void OnUpdateDeactivation () { }

	//--- Controlling the active status of the update.
	inline bool IsUpdateActive () const { return mActive; }
	void ActivateUpdate ();
	void DeactivateUpdate ();
	void ToggleUpdate();

//--- Getter and setter for the target frame rate and time at which OnUpdate is to be called.
	inline int GetUpdateFrameRate () const { return mFrameRate; }
	void SetUpdateFrameRate (const int aRate);
	//--- time is in seconds.
	inline float GetUpdateFrameTime () const { return mFrameTime; }
	void SetUpdateFrameTime (const float aTime);

	//--- constructor initializes update to be active;
	//---measures are 0 so that updating will be every frame.
	Updateable ()
		: mActive(true), mFrameRate(0), mFrameCount(0), mFrameTime(0.0f), mTimeElapsed(0.0f)
	{ }
	//--- virtual destructor to allow inherited destructors to work correctly.
	virtual ~Updateable () {}


private:
	int mFrameRate;			// target frame rate.
	int mFrameCount;		// count current frames since last update.
	float mFrameTime;		// target time between updates.
	float mTimeElapsed;		// current time since last update.
	bool mActive;

	//--- Returns true if OnUpdate() should be called, false otherwise.
	//--- See header comments for how frame rate and time measures interact in getting the return value.
	bool IsTimeToUpdate ();

}; // end Updateable class.

} // end AllanMilne namespace.

#endif