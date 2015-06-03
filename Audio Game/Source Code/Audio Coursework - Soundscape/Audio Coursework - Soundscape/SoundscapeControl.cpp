/*
	File name:		SoundscapeControl.cpp
	Version:		1.3
	Date:			24/03/14
	Author:			Michael Little (Based on and utilises code from WalkingControl by Allan C. Milne)

	Exposes:		Implementation of the SoundscapeControl class.
	Requires:		XASound, Updateable, Sounds, camera, map
	Expects:		Sounds/Rain.wav, explosion.wav, cannon.wav, gunshots.wav, gunshot_echo.wav, plane.wav, 
					scream.wav, footsteps.wav, turning.wav, WrongWay.wav, StartSound.wav, WinSound.wav
		
	
	Description:
	This is the implementation of the class members (see SoundscapeControl.hpp for the class definition and further information on the application functionality)
*/

// system includes
#include <windows.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

// framework includes.
#include "Framework\XACore.hpp"
#include "Framework\XASound.hpp"
#include "Framework\Updateable.hpp"
using namespace AllanMilne::Audio;
using AllanMilne::Updateable;

// application includes
#include "SoundscapeControl.hpp"
#include "Sounds.hpp"


// Anonymous namespace used for defining constants and helper functions used in the implementation.
namespace 
{

	// .wav files used for 3D sounds refrenced here
	const int numberOf3DSounds = 6; //total number of 3D sounds
	typedef struct fileInfoStruct
	{
		string name;
		X3DAUDIO_VECTOR position;
	} fileInfoStruct;
	const fileInfoStruct cFileInfo[] = {
		{ "Sounds/explosion.wav", {50, 0.0f, 10} },
		{ "Sounds/cannon.wav", {-50.0f, 0.0f, -150.0f} },
		{ "Sounds/gunshots.wav", {150.0f, 0.0f, -100.0f} },
		{ "Sounds/gunshot_echo.wav", {-150, 0.0f, -200} },
		{ "Sounds/plane.wav", {20, 0.0f, 20} },
		{ "Sounds/scream.wav", {-80, 0.0f, -300} },
	};

	// The 8 cardinal compass directions used for turning the listener.
	// Directions are from North clockwise, vectors are normalised.
	const float cOneOverSqrt2= 1 / sqrt(2.0f);
	const X3DAUDIO_VECTOR cCardinalPoints[] = {
		{0.0f, 0.0f, 1.0f}, {cOneOverSqrt2, 0.0f, cOneOverSqrt2}, {1.0f, 0.0f, 0.0f}, {cOneOverSqrt2, 0.0f, -cOneOverSqrt2},
		{0.0f, 0.0f, -1.0f}, {-cOneOverSqrt2, 0.0f, -cOneOverSqrt2}, {-1.0f, 0.0f, 0.0f}, {-cOneOverSqrt2, 0.0f, cOneOverSqrt2}
	};
	
	// Display instructions on using this application.
	void Instructions () 
	{
		stringstream msg;
		msg<< "You need to get to the bomb shelter, listen for the Air Raid siren!" <<endl;
		msg<<endl <<" controls: " <<endl <<endl;
		msg <<" W \t = move in forward direction" <<endl;
		msg <<" S \t = move Backwards" <<endl;
		msg <<" A \t = move Left" <<endl;
		msg <<" D \t = move Right" <<endl;
		msg <<" J \t = turn head anti-clockwise. " <<endl;
		msg <<" L \t = turn head clockwise. " <<endl;
		msg <<" I \t = display these instructions. " <<endl;
		msg <<" R \t = Reset the game. " <<endl;
		msg <<" escape \t = end the application. " <<endl;
		const string msgStr = msg.str();
		MessageBox (NULL, msgStr.c_str(), TEXT("Instructions for Soundscape"), MB_OK | MB_ICONINFORMATION);
	} // end Instructions function.

	void GameFinished()
	{
		stringstream msg;
		msg<< "You get to live another day Soldier" <<endl;
		msg <<" hit ok to restart the game " <<endl;
		const string msgStr = msg.str();
		MessageBox (NULL, msgStr.c_str(), TEXT("Congratulations"), MB_OK | MB_ICONINFORMATION);
	}

} // end anonymous namespace.


/////////////////////////////////////////////////////////////////////
// Interface Implementation //
/////////////////////////////////////////////////////////////////////
// initializes the required sounds
bool SoundscapeControl::Initialize (HWND aWindow)
{
//////////////////
// Static Sound //
////////////////////////////////////////////////////////

	// guard against uninitialized or invalid XACore.
	if (XACore::GetStatus() != XACore::OK) return false;

	// movement sounds
	mFootsteps.reset(new XASound ("Sounds/footsteps.wav"));
	if (!mFootsteps->IsValid()) {
		MessageBox (NULL, "Error loading footsteps.wav", TEXT ("Initialize() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mTurning.reset(new XASound ("Sounds/turning.wav"));
	if (!mFootsteps->IsValid()) {
		MessageBox (NULL, "Error loading turning.wav", TEXT ("Initialize() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}

	//Voice work
	mHitBoundry.reset(new XASound ("Sounds/WrongWay.wav"));
	if (!mHitBoundry->IsValid()) {
		MessageBox (NULL, "Error loading WrongWay.wav", TEXT ("Initialize() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mStartSound.reset(new XASound ("Sounds/StartSound.wav"));
	if (!mStartSound->IsValid()) {
		MessageBox (NULL, "Error loading StartSound.wav", TEXT ("Initialize() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mGoalReached.reset(new XASound ("Sounds/WinSound.wav"));
	if (!mGoalReached->IsValid()) {
		MessageBox (NULL, "Error loading WinSound.wav", TEXT ("Initialize() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}

	// Background Sounds
	mRain.reset(new XASound ("Sounds/rain.wav"));
	if (!mRain->IsValid()) {
		MessageBox (NULL, "Error loading Rain.wav", TEXT ("Initialize() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mRain->SetLooped(true);			//loops the sound
	mRain->AdjustVolume (-12.0f);	//sets the volume

	//adjusts the volume of the individual sounds
	mGoalReached->AdjustVolume (-5.0f);
	mFootsteps->AdjustVolume (-10.0f);
	mTurning->AdjustVolume (-10.0f);
	////////////////////////////////////////////////////////

	// Create the 3D sound objects and add to the updateable component list.
	m3DSounds.reserve(numberOf3DSounds);
	for (int i=0; i<numberOf3DSounds; i++)
	{
		m3DSounds.emplace_back(new Sounds(cFileInfo[i].name, cFileInfo[i].position.x, cFileInfo[i].position.y, cFileInfo[i].position.z, &mListener));
		if (!m3DSounds[i]->IsValid()) 
		{
			stringstream errorMsg;
			errorMsg <<"Error loading file '";
			errorMsg <<cFileInfo[i].name <<"'";
			MessageBox (NULL, errorMsg.str().c_str(), TEXT ("Initialize() - FAILED"), MB_OK | MB_ICONERROR );
			return false;
		}
		// Deactivate so starts playing on setup.
		m3DSounds[i]->DeactivateUpdate();
	} // end for each 3D file name.

	////////////
	// Camera //
	////////////
	// Create the camera object.
	m_Camera = new Camera;
	if(!m_Camera)
	{
		return false;
	}
	
	//sets up the player/listener/camera spawning position
	float SndPos = -100 + (rand() % 190);
	m_Camera->SetPosition(SndPos, 0.0f, 15.0f);
	m_Camera->SetRotation(0.f, 0.0f, 0.0f);
	m_Camera->Render();
	mListener.Position = m_Camera->GetPosition();
	mListener.OrientFront = m_Camera->GetForward();

	/////////
	// Map //
	/////////
	// creates a new instance of the map
	m_Map = new Map(&mListener);
	if (!m_Map) 
	{
		MessageBox (NULL, "Error creating the map.", TEXT ("Initialize() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}

	//sets up the map values
	m_Map->SetBase(-100.0f, 0.0f, -400.0f);
	m_Map->SetSize(200.0f, 0.0f, 100.0f);
	m_Map->SetGoalPosition(100.0f, 0.0f, -390.0f);
	m_Map->SetGoalSoundPosition(100.0f, 10.0f, -420.0f);
	return true; // returns if everything was initialised ok
} // Initialize end


// called to start the sounds
bool SoundscapeControl::Setup ()
{
	// loads the instructions
	Instructions();
	// Play the static sounds and activate the updating.
	mStartSound->Play();
	mRain->Play();
	for (auto iter =m3DSounds.begin(); iter!=m3DSounds.end(); ++ iter)
	{
		(*iter)->ActivateUpdate();
	}
	m_Map->Reset();
	return true; // setup completed without error.
} // Setup end

// Detects if the player hits the map boundries and responds apropriatly
void SoundscapeControl::DetectCollision()
{
	if (m_Camera->GetPosition().x >= m_Map->GetMapSize().x)
	{
		if(!mHitBoundry->IsPlaying())
		{
			mHitBoundry->Play();
		}
		m_Camera->MoveRIGHT();
	}
	if (m_Camera->GetPosition().x <= m_Map->GetMapBase().x)
	{
		if(!mHitBoundry->IsPlaying())
		{
			mHitBoundry->Play();
		}
		m_Camera->MoveLEFT();
	}
	if (m_Camera->GetPosition().z >= m_Map->GetMapSize().z)
	{
		if(!mHitBoundry->IsPlaying())
		{
			mHitBoundry->Play();
		}
		m_Camera->MoveFWRD();
	}
	if (m_Camera->GetPosition().z <= m_Map->GetMapBase().z)
	{
		if(!mHitBoundry->IsPlaying())
		{
			mHitBoundry->Play();
		}
		m_Camera-> MoveBKWRD();
	}

	if (m_Camera->GetPosition().x == m_Map->GetGoalPosition().x && m_Camera->GetPosition().z <= m_Map->GetGoalPosition().z)
	{
		m_Map->Stop();
		mGoalReached->Play();
		GameFinished();
		Reset();
	}

}

// checks for keypresses and changes in the listener position and calls Update()
bool SoundscapeControl::ProcessFrame (const float deltaTime)
{
	DetectCollision();

	////////////////////////////////////
	// Player controls implementation //
	///////////////////////////////////////////////////////
	IsMoving = false;
	
	// move forward.
	if (GetAsyncKeyState ('W') & 0x0001) 
	{
		m_Camera-> MoveFWRD();
		IsMoving = true;
		mFootsteps->Play();
	}
	//move backwards.
	if (GetAsyncKeyState ('S') & 0x0001) 
	{
		m_Camera-> MoveBKWRD();
		mFootsteps->Play();
	}
	//move left.
	if (GetAsyncKeyState ('A') & 0x0001) 
	{
		m_Camera-> MoveLEFT();
		mFootsteps->Play();
	}
	//move right.
	if (GetAsyncKeyState ('D') & 0x0001) 
	{
		m_Camera-> MoveRIGHT();
		mFootsteps->Play();
	}

	//turn camera anti-clockwise.
	if (GetAsyncKeyState ('J') & 0x0001) 
	{
		m_Camera-> RotateLEFT();
		mTurning->Play();
	}
	//turn head clockwise.
	if (GetAsyncKeyState ('L') & 0x0001) 
	{
		m_Camera-> RotateRIGHT();
		mTurning->Play();
	}

	if (GetAsyncKeyState ('R') & 0x0001) 
	{
		Reset();
	}

	// updates the camera and keeps the listener in the same place
	m_Camera->Render();
	mListener.Position = m_Camera->GetPosition();
	mListener.OrientFront = m_Camera->GetForward();
	m_Map->Update3DSound();
	//brings up the instructions
	if (GetAsyncKeyState ('I') & 0x0001) 
	{
		Instructions();
	}
////////////////////////////////////////////////////////////

	// go through the list of 3D sounds calling their Update method.
	for (auto iter=m3DSounds.begin(); iter!=m3DSounds.end(); ++iter)
	{
		(*iter)->Update(deltaTime);
	}


	return true;
} //  Processframe end

// -- resets the game when called
void SoundscapeControl:: Reset()
{
	float SndPos = -100 + (rand() % 190);
	m_Camera->SetPosition(SndPos, 0.0f, 15.0f);
	m_Camera->SetRotation(0.f, 0.0f, 0.0f);
	m_Camera->Render();
	mListener.Position = m_Camera->GetPosition();
	mListener.OrientFront = m_Camera->GetForward();
	Instructions();
	m_Map->Reset();
}

//stops the sounds and resets the listner
void SoundscapeControl::Cleanup ()
{
	if (mRain->IsValid()) {
		mRain->Stop();
	}
	for (auto iter=m3DSounds.begin(); iter!=m3DSounds.end(); ++iter)
	{
		(*iter)->DeactivateUpdate();
	}
	m_Map->Stop();
	mCleanedUp = true;
} // cleanup end

////////////////////////////
// Constructor/Destructor //
//////////////////////////////////////////////////////////////
// constructor
SoundscapeControl::SoundscapeControl()
	: mRain(nullptr), mCleanedUp(false), mOrientation(0), m_Map(nullptr)
{
		SecureZeroMemory (&mListener, sizeof(X3DAUDIO_LISTENER));
	// Start at the origin.
	mListener.Position.x = mListener.Position.y = mListener.Position.z = 0.0f;
	// start facing North.
	mListener.OrientFront = cCardinalPoints[mOrientation];
	// Will always be upright; no pitch or roll.
	mListener.OrientTop.x = 0.0f;
	mListener.OrientTop.y = 1.0f;
	mListener.OrientTop.z = 0.0f;
}// constructor end  

// destructor
SoundscapeControl::~SoundscapeControl()
{
	if (!mCleanedUp) Cleanup();
	m3DSounds.clear();
	m_Map->Stop();
} // destructor end  
//////////////////////////////////////////////////////////////

// end of code