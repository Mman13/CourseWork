/*
	File name:	Sounds.hpp
	Version:	1.1
	Date:		24/03/2014
	Autor:		Michael Little (Based on and utilises code from My3DSound by Allan C. Milne)

	Requires: XASound, Updateable

	Desription:
	The Sounds class is used to create a sound element at specific location in the world
	and utilises an updateable interface to allow the sound to be started/stoped on command.
	It also uses an XASound object to encapsulate the sound and an X3DAudio emitter struct to 
	maintain the position of the sound(s) and listner
*/

#ifndef __Sounds_HPP__
#define __Sounds_HPP__

// system includes
#include <Windows.h>
#include <XAudio2.h>
#include <X3DAudio.h>
#include <memory>			
#include <string>
using namespace std;

// framework include
#include"Framework\Updateable.hpp"
using AllanMilne::Updateable;

// forward Declarations
class XASound;

class Sounds : public Updateable 
{
public:
	/////////////////////////////////
	// Implementation of interface //
	/////////////////////////////////
	//--- stop playing sound
	void OnUpdateDeactivation ();
	//--- reset position to start and begin playing
	void OnUpdateActivation ();		
	//--- real-time calculate position and apply to audio
	void OnUpdate (const float deltaTime);		

	//--- used to indicate if a valid object has been created
	inline bool IsValid() const { return mSound->IsValid(); }

	//--- encapsulates the underlying sound and returns it to XASound
	inline shared_ptr<XASound> GetXASound() const { return mSound; }

	//--- getter and setter for the sound position.
	inline X3DAUDIO_VECTOR GetPosition () const { return mEmitter.Position; }
	void SetPosition (const float x, const float y, const float z);

	//--- Allow the listener to be changed.
	inline void SetListener (X3DAUDIO_LISTENER* aListener) { mListener = aListener; }

	////////////////////////////
	// Constructor/Destructor //
	////////////////////////////
	//--- constructor
	Sounds (const string& aWavFile, const float x, const float y, const float z, X3DAUDIO_LISTENER* aListener); 
	//--- destructor
	virtual ~Sounds(); 

private:
	//--- the sound element to be played
	shared_ptr<XASound> mSound;	

	//--- the emitter used for defining the positions of sound elements
	X3DAUDIO_EMITTER mEmitter;
	X3DAUDIO_CONE mCone;

	//--- A pointer to the listener 
	X3DAUDIO_LISTENER* mListener;

protected:

	virtual void RenderAudio ();

}; // sounds class end

#endif