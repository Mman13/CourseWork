/*
	file:	Map.hpp
	Version:	1.3
	Date:	21th april 2014.
	Author:	Michael Little based on Room.hpp by Allan c. Milne.

	requires: Camera, Sounds, XASound

	Description:
	Creates a virtual map for the player to walk around.
	Stores the position of two opposite cornors and uses these to create the boundries for the player.
	Also contains the goal, which uses a sound cone to lead the player towards it.

*/

#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <Windows.h>
#include <X3DAudio.h>
#include <memory>
using std::unique_ptr;

//--- Framework includes.
#include "framework\XASound.hpp"
using AllanMilne::Audio::XASound;

//--- Application includes.
#include "Camera.hpp"
#include "Sounds.hpp"

class Map
{
public:
	// --- Default Contructor
	Map();
	Map(const Map&);
	Map(X3DAUDIO_LISTENER*);
	// --- Destructor
	~Map();
	bool Initialize(HWND aWindow);

	//--- Returns true if the player and sounds have been set up correctly, false otherwise.
	inline bool IsValid() const {}

	D3DXVECTOR3 GetMapSize();
	D3DXVECTOR3 GetMapBase();
	D3DXVECTOR3 GetGoalPosition();
	D3DXVECTOR3 GetGoalSoundPosition();

	void SetSize(float, float, float);
	void SetBase(float, float, float);
	void SetGoalPosition(float, float, float);
	void SetGoalSoundPosition(float, float, float);

	void Update3DSound ();

	// --- resets the player to the start
	void Reset();

	//--- Stop playing the Goal sound.
	void Stop();

private:

	// --- variables for seting up map size and base
	float MapSizeX, MapSizeY, MapSizeZ;
	float MapBaseX, MapBaseY, MapBaseZ;
	float GoalPosX, GoalPosY, GoalPosZ;
	float GoalSndX, GoalSndY, GoalSndZ;

	//--- the sound effect of walking into the boundries.
	unique_ptr<XASound> mHitBoundrySound;

	//--- The sound from the goal, include emitter to use 3D calculations with a sound cone.
	unique_ptr<XASound> mGoalSound;
	X3DAUDIO_EMITTER mEmitter;
	X3DAUDIO_CONE mCone;

	//--- pointer to the listner
	X3DAUDIO_LISTENER* mListener;

	//--- returns true if player gets to the goal
	void DetectCollision();

}; // end Map class
#endif