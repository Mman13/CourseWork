/*
	File name:	Sound.cpp
	Version:	1.1
	Date:	24/03/14
	Author:	Michael Little (Based on and utilises code from My3DSound by Allan C. Milne)

	Requires:	XASound, Updateable, XACore.
	
	Description:
	Implementation of the Sound class (see Sound.hpp for details)	
	includes code to move the emmiters after they play their sound
*/

// System Includes
#include <Windows.h>
#include <X3DAudio.h>
#include <math.h>
#include <XAudio2.h>
#include <string>
#include <cmath>		
#include <memory>	
#include <iostream>
#include <sstream>
#include <time.h> 
using namespace std;

// Framework includes
#include "Framework\XASound.hpp"
#include "Framework\XACore.hpp"
#include "Framework\Updateable.hpp"
using AllanMilne::Audio::XASound;
using AllanMilne::Audio::XACore;
using AllanMilne::Updateable;

#include "Sounds.hpp"
#include "StuVector/StuVector3.hpp"
/////////////////////////////////////////////////////////////////////
// Interface Implementation //
/////////////////////////////////////////////////////////////////////
//used to stop the audio
void Sounds::OnUpdateDeactivation() 
{
	mSound->Stop();
}// OnUpdateDeactivation end

// used to start the audio
void Sounds::OnUpdateActivation()
{
	mSound->Play();
}// OnUpdateDeactivation end

//used to update the audio position ect.
void Sounds::OnUpdate(const float deltaTime)
{
	
	//RenderAudio();
	XACore::GetInstance()->Apply3D ( mSound->GetSourceVoice(), &mEmitter, mListener, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_LPF_DIRECT);

	//once the emmiter has finished playing its moved, and repeats the process
	if(!mSound->IsPlaying())
	{
		srand (time(NULL));
		float SndPos = -100 + (rand() % 190);
		float SndPos2 = -390 + (rand() % 90);
		SetPosition (SndPos, 0.0f, SndPos2);
		mSound->Play();
	}
	
} // OnUpdate end


//////////////////////////////
// Interface Implementation //
/////////////////////////////////////////////////////////////////////
// Constructor used to create XASound object
Sounds::Sounds (const string& aWavFile, const float x, const float y, const float z, X3DAUDIO_LISTENER* aListener)
{
	// Create the sound element.
	mSound = make_shared<XASound>(aWavFile, true);	// if true -> filters enabled.
	if (!mSound->IsValid()) return;		// Can do no more.
	mSound->SetLooped(false);
	mSound->AdjustVolume (+20.0f);
	// Set up the emitter struct.
	SecureZeroMemory (&mEmitter, sizeof(X3DAUDIO_EMITTER));
	XAUDIO2_VOICE_DETAILS details;
	mSound->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	mEmitter.Position.x = x;
	mEmitter.Position.y = y;
	mEmitter.Position.z = z;
	mListener = aListener;

	
}// Constructor end

// Destructor
Sounds::~Sounds ()
{
	if (mSound->IsValid()) 
	{
		mSound->Stop();
		delete mEmitter.pCone;
	}
}// Destructor end

void Sounds::RenderAudio ()
{

}  // end render audio

//used to move the position of the emmiter
void Sounds::SetPosition (const float x, const float y, const float z)
{
	mEmitter.Position.x = x;
	mEmitter.Position.y = y;
	mEmitter.Position.z = z;
} // end SetPosition function.


// end of Sounds class