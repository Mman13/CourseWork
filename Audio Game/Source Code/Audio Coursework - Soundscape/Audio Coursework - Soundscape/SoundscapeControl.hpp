/*
	File name:		SoundscapeControl.hpp
	Version:	1.3
	Date:		24/03/14
	Author:		Michael Little (Based on and utilises code from WalkingControl by Allan C. Milne)

	Requires:	IState, XASound, Sounds, camera, map
	
	Description:

	This class is an implementation of the IState interface
	The class called from the context WinCore class can be found in IState.hpp

	The application will create a soundscape made up of a variety of sound elements
	and allows the user to walk around this soundscape

	Static sounds properties won't change 
	3D sounds will appear to change as the user moves
	the listener is modelled by an X3DAudio listener struct
	
	This class uses the Camera class to allow the user to walk and turning their head.

	I also uses the Map class to create an instance of the virtual map.

	it also incorporates a flag to indicate whether the object resources have been cleaned up
	this is to allow both the explicit Cleanup and destructor to be used.

*/

#ifndef __SoundscapeControl_HPP__
#define __SoundscapeControl_HPP__

// system includes
#include <memory>	
#include<vector>
using namespace std;

//--- Framework includes
#include "framework\IState.hpp"
#include "framework\XASound.hpp"
using AllanMilne::IState;
using AllanMilne::Audio::XASound;

//--- Class includes
#include "Camera.hpp"
#include "Map.hpp"

//--- forward declaration
class Sounds;

class SoundscapeControl : public IState {
public:
	/////////////////////////////////
	// Implementation of interface //
	/////////////////////////////////
	//--- initializes the required sounds
	bool Initialize (HWND aWindow);	
	//--- called to start the sounds
	bool Setup ();
	//--- checks for keypresses and changes in the listener position and calls Update()
	bool ProcessFrame (const float deltaTime);
	//---stops the sounds and resets the listner

	bool IsMoving;

	void Cleanup ();

	void DetectCollision();
	//--- Resets the Scene
	void Reset();

	///////////////////////
	// control functions //
	///////////////////////
	void frwd();
	void bkwrd();
	void left();
	void right();
	void up();
	void down();
	void rotL();
	void rotR();
	void rotD();
	void rotU();

	////////////////////////////
	// Constructor/Destructor //
	////////////////////////////
	//--- Constructor
	SoundscapeControl ();
	//--- Destructor
	~SoundscapeControl ();
	
private:

	//--- sets up the instances of the map and camera class
	Camera* m_Camera;
	Map* m_Map;

	//--- The listener.
	//std::shared_ptr<X3DAUDIO_LISTENER> mListener;
	X3DAUDIO_LISTENER mListener;
	//--- used to determine the direction the listner is facing (0= North, 7= North west ect.)
	int mOrientation;

	//--- used to indicate whether resources have been cleaned up
	bool mCleanedUp;

	////////////
	// sounds //
	////////////
	//--- Static Sounds
	//--- Background noise
	unique_ptr<XASound> mRain;

	//--- Sounds used for moving and turning.
	unique_ptr<XASound> mFootsteps;
	unique_ptr<XASound> mTurning;
	unique_ptr<XASound> mHitBoundry;
	unique_ptr<XASound> mStartSound;
	unique_ptr<XASound> mGoalReached;
	
	//--- updateable 3D sounds 
	vector<unique_ptr<Sounds>> m3DSounds;

}; // SoundscapeControl class

#endif