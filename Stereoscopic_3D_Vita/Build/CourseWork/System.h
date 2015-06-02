//System.h created 1/4/15
//Author Michael Little - 1103677

#ifndef SYSTEM_H
#define SYSTEM_H

#include "GameObject.h"
#include "Camera.h"

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector3.h>
#include <vector>
#include <graphics/mesh_instance.h>


class System
{
public:
	System();
	~System();
//////////////////////////////////////////////////
//colision detection
	bool SphereCollision(GameObject* objectpointer, Camera* camerapointer);
	
////////////////////////////////////////////////////
// Getters //
	int GetMinObstacle();
	float GetObstacleSpeed();
	float GetTargetSpeed();
	float GetScore();
	float GetTimer();
	bool GetCollided();
	bool GetGameOver();
	bool GetPaused();
	bool GetStarted();

/////////////////////////////////////////////////////
// Setters //
	void SetMinObstacle(int minimum);
	void SetObstacleSpeed(float OBspeed);
	void SetTargetSpeed(float TARspeed);
	void SetScore(float score);
	void SetTimer(float timer);
	void SetCollided(bool collided);
	void SetGameOver(bool gameOver);
	void SetPaused(bool paused);
	void SetStarted(bool started);

////////////////////////////////////////////////////
// Update //
	void ChangeScore(float change);
	void ChangeTimer(float change);

	void Update();

private:	
	abfw::MeshInstance Mesh1;
	
/////////////////////////////////////////
// Variables //
	int minObstacle_;

	float obstacleSpeed_;
	float targetSpeed_;
	float timer_;
	float score_;

	bool collided_;
	bool GameOver_;
	bool paused_;
	bool started_;
};

#endif