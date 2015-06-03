/*
	file:	Map.cpp
	Version:	1.3
	Date:	21th april 2014.
	Author:	Michael Little based on Room.cpp by Allan c. Milne.

	Requires: Player, XACore, XASound, stu::v3f.

	Expects: Sounds/air_raid.wav

	Desription:
	This is the implementation of the class members (see Map.hpp for the class definition and further information on the application functionality)

	also contains code to randomly spawn the emmitter after they play their sound.

*/

//--- system includes.
#include <Windows.h>
#include <math.h>
#include <XAudio2.h>
#include <X3DAudio.h>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
using std::unique_ptr;
using std::string;

//--- Framework includes.
#include "Framework\XACore.hpp"
#include "Framework\XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;

//--- Application includes.
#include "Map.hpp"
#include "Camera.hpp"
#include "StuVector/StuVector3.hpp"

namespace
{
	//// --- map size
	const stu::v3f kMapSize = { 200.0f, 0.0f, 100.0f };
	//// -- map base; the bottom left of the room
	const stu::v3f kMapBase = { -100.0f, 0.0f, -400.0f };
	//position of the goal
	const stu::v3f kGoalPosition = { 100, 0.0f, -390 };
	// position of the goal sound
	const stu::v3f kGoalSoundPosition = { kGoalPosition.x, 10.0f, kGoalPosition.z - 30.0f};
	// Goal Sound
	const string kGoalFile = "Sounds/air_raid.wav";
} // end namespace

bool Map::Initialize(HWND aWindow)
{
	//loads namespace variables into modifiable values
	MapSizeX = kMapSize.x;
	MapSizeY = kMapSize.y;
	MapSizeZ = kMapSize.z;
	MapBaseX = kMapBase.x;
	MapBaseY = kMapBase.y;
	MapBaseZ = kMapBase.z;

	GoalPosX = kGoalPosition.x;
	GoalPosY = kGoalPosition.y;
	GoalPosZ = kGoalPosition.z;
	GoalSndX = kGoalSoundPosition.x;
	GoalSndY = kGoalSoundPosition.y;
	GoalSndZ = kGoalSoundPosition.z;

	return true; // returns if everything was initialised ok
}

// --- default constructor
Map::Map (X3DAUDIO_LISTENER* listener)
	: mGoalSound(new XASound(kGoalFile, true))
{
	//sets up the goal sound and sound cone
	if (!mGoalSound->IsValid()) return;
	mGoalSound->SetLooped(true);
	mListener = listener;
	//initialise emitter and sound cone
	SecureZeroMemory (&mEmitter, sizeof(X3DAUDIO_EMITTER));
	XAUDIO2_VOICE_DETAILS details;
	mGoalSound->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	mEmitter.Position.x = kGoalPosition.x;
	mEmitter.Position.y = kGoalPosition.y;
	mEmitter.Position.z = kGoalPosition.z;

	mEmitter.OrientTop = listener->OrientTop;	// So we are in the same plane as the listener.
	const float goalToSound = 2;
	mEmitter.OrientFront.x = 0;
	mEmitter.OrientFront.y = 0;
	mEmitter.OrientFront.z = 1;
	SecureZeroMemory (&mCone, sizeof(X3DAUDIO_CONE));
	mCone.InnerAngle = 8.0f * atan((0.5f) / goalToSound);
	mCone.InnerVolume = 25.0f;
	mCone.OuterAngle = mCone.InnerAngle + 8.0f;
	mCone.OuterVolume = 20.0f;
	mEmitter.pCone = &mCone;
} // end map constructor.

//--- Destructor should stop the goal sound playing and free the sound cone.
Map::~Map()
{
	if (mGoalSound->IsValid()) 
	{
		mGoalSound->Stop();
		delete mEmitter.pCone;
	}
} // end Goal destructor.

void Map::Reset()
{
	mEmitter.pCone = &mCone;
	Update3DSound();
	mGoalSound->Play();
} // end Reset function.

void Map::Stop()
{
	mGoalSound->Stop();
} // end Stop function.

void Map::Update3DSound()
{
	if (!mGoalSound->IsValid()) return;
	XACore::GetInstance()->Apply3D(mGoalSound->GetSourceVoice(), &mEmitter, mListener, X3DAUDIO_CALCULATE_MATRIX);
}


//--- Getters and Setters
D3DXVECTOR3 Map::GetMapSize()
{
	return D3DXVECTOR3 (MapSizeX, MapSizeY, MapSizeZ);
}

D3DXVECTOR3 Map::GetMapBase()
{
	return D3DXVECTOR3 (MapBaseX, MapBaseY, MapBaseZ);
}

D3DXVECTOR3 Map::GetGoalPosition()
{
	return D3DXVECTOR3 (GoalPosX, GoalPosY, GoalPosZ);
}

D3DXVECTOR3 Map::GetGoalSoundPosition()
{
	return D3DXVECTOR3 (GoalSndX, GoalSndY, GoalSndZ);
}

void Map::SetBase(float x, float y, float z)
{
	MapBaseX = x;
	MapBaseY = y;
	MapBaseZ = z;
}

void Map::SetSize(float x, float y, float z)
{
	MapSizeX = x;
	MapSizeY = y;
	MapSizeZ = z;
}

void Map::SetGoalPosition(float x, float y, float z)
{
	GoalPosX = x;
	GoalPosY = y;
	GoalPosZ = z;
}

void Map::SetGoalSoundPosition(float x, float y, float z)
{
	GoalSndX = x;
	GoalSndY = y;
	GoalSndZ = z;
}