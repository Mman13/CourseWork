//System.cpp created 1/4/15
//Author Michael Little - 1103677

#include "System.h"

System::System()
{
	//initialise variables
	minObstacle_ = 0;
	obstacleSpeed_ = 0;
	targetSpeed_ = 0;
	timer_ = 0;
	score_ = 0;
	collided_ = false;
	GameOver_ = false;
	paused_ = false;
	started_ = false;
}


System::~System()
{

}

bool System::SphereCollision( GameObject* objectpointer, Camera* camerapointer) //used for collision detection between camera and objects
{
	abfw::Sphere object_sphere = objectpointer->bounding_sphere().Transform(objectpointer->GetTransform());
	abfw::Sphere camera_sphere = camerapointer->bounding_sphere().Transform(camerapointer->GetTransform());

	float Distance = (object_sphere.position() - camera_sphere.position()).LengthSqr();
	float Radius = objectpointer->bounding_sphere().radius();
	float Radius2 = camerapointer->bounding_sphere().radius();
	//30463.2718...

	if (Distance < (Radius + Radius2) * (Radius + Radius2))
	{
		return true;
	}
	else
	{
		return false; 
	}
}

int System::GetMinObstacle() //used to return the minimum number of obstacles
{
	return minObstacle_;
}

float System::GetObstacleSpeed() //used to return an obstacles speed / velocity
{
	return obstacleSpeed_;
}

float System::GetTargetSpeed() //used to return a targets speed / velocity
{
	return targetSpeed_;
}

float System::GetScore() //used to return the current score
{
	return score_;
}

float System::GetTimer() // used to return the current timer
{
	return timer_;
}

bool System::GetCollided() // used to stop colision detection spamming
{
	return collided_;
}

bool System::GetGameOver() // used to return whether or not its game over
{
	return GameOver_;
}

bool System::GetPaused() // used to return wether or no the game is paused
{
	return paused_;
}

bool System::GetStarted() // used to return whether or not the game has started
{
	return started_;
}


void System::SetMinObstacle(int minimum) // used to set the minimum number of obstacles
{
	minObstacle_ = minimum;
}

void System::SetObstacleSpeed(float OBspeed)// used to set the obstacles speed/velocity
{
	obstacleSpeed_ = OBspeed;
}

void System::SetTargetSpeed(float TARspeed)// used to set the targets speed/velocity
{
	targetSpeed_ = TARspeed;
}

void System::SetScore(float score)// used to set the players score
{
	score_ = score;
}

void System::SetTimer(float timer)//used to set the current timer
{
	timer_ = timer;
}

void System::SetCollided(bool collided)// used to set wehther or not collision has already taken place
{
	collided_ = collided;
}

void System::SetGameOver(bool gameOver)// used to set gameover to true or false
{
	GameOver_ = gameOver;
}

void System::SetPaused(bool paused)// used to pause or unpause the game
{
	paused_ = paused;
}

void System::SetStarted(bool started)// used to start the game
{
	started_ = started;
}


void System::ChangeScore(float change)// used to change the current score
{
	score_ += change;
}

void System::ChangeTimer(float change)//used to change the current timer
{
	timer_ += change;
}

void System::Update()//used to check whether or not time has run out and increase the difficulty
{
	// increasing difficulting
	if(score_ >= 100)
	{
		obstacleSpeed_ = 7;
		targetSpeed_ = 7.5;
		minObstacle_ = 5;
	}
	else if(score_ >= 90)
	{
		obstacleSpeed_ = 6.5;
		targetSpeed_ = 7;
	}
	else if(score_ >= 80)
	{
		obstacleSpeed_ = 6;
		targetSpeed_ = 6.5;
	}
	else if(score_ >= 70)
	{
		obstacleSpeed_ = 5.5;
		targetSpeed_ = 6;
		minObstacle_ = 4;
	}
	else if(score_ >= 60)
	{
		obstacleSpeed_ = 5;
		targetSpeed_ = 5.5;
	}
	else if(score_ >= 50)
	{
		obstacleSpeed_ = 4.5;
		targetSpeed_ = 5;
	}
	else if(score_ >= 40)
	{
		obstacleSpeed_ = 4;
		targetSpeed_ = 4.5;
		minObstacle_ = 3;
	}
	else if(score_ >= 30)
	{
		obstacleSpeed_ = 3.5;
		targetSpeed_ = 4;
	}
	else if(score_ >= 20)
	{
		obstacleSpeed_ = 3;
		targetSpeed_ = 3.5;
		minObstacle_ = 2;
	}
	else if(score_ >= 10)
	{
		obstacleSpeed_ = 2.5;
		targetSpeed_ = 3;
	}
	else
	{
		obstacleSpeed_ = 2;
		targetSpeed_ = 2.5;
		minObstacle_ = 1;
	}

	if(timer_ <= 0)// checks if the timer has run out
	{
		GameOver_ = true;
	}
}