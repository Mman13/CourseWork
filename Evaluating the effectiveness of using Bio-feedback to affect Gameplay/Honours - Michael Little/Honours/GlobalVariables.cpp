#include "GlobalVariables.h"

namespace
{
	static double globalExcitement = 0;
	static double globalBoredom = 0;
	static double globalFrustration = 0;
	static double globalMeditation = 0;

	static string detectedEmotion;

	static bool gameStart = false;
	static bool bossSpawned = false;

	static int enemiesKilled = 0;
	static int timesHit = 0;
	static int score = 0;
	static int bossHealth = 0;

}

GlobalVariables::GlobalVariables()
{

}

GlobalVariables::~GlobalVariables()
{

}

double GlobalVariables::GetGlobalExcitement()
{
	globalMutex.lock();
	return globalExcitement;
	globalMutex.unlock();
}

double GlobalVariables::GetGlobalBoredom()
{
	globalMutex.lock();
	return globalBoredom;
	globalMutex.unlock();
}

double GlobalVariables::GetGlobalFrustration()
{
	globalMutex.lock();
	return globalFrustration;
	globalMutex.unlock();
}

double GlobalVariables::GetGlobalMeditation()
{
	globalMutex.lock();
	return globalMeditation;
	globalMutex.unlock();
}

string GlobalVariables::GetDetectedEmotion()
{
	globalMutex.lock();
	return detectedEmotion;
	globalMutex.unlock();
}

bool GlobalVariables::GetGameStart()
{
	globalMutex.lock();
	return gameStart;
	globalMutex.unlock();
}

bool GlobalVariables::GetBossSpawned()
{
	globalMutex.lock();
	return bossSpawned;
	globalMutex.unlock();
}

int GlobalVariables::GetEnemiesKilled()
{
	globalMutex.lock();
	return enemiesKilled;
	globalMutex.unlock();
}

int GlobalVariables::GetTimesHit()
{
	globalMutex.lock();
	return timesHit;
	globalMutex.unlock();
}

int GlobalVariables::GetScore()
{
	globalMutex.lock();
	return score;
	globalMutex.unlock();
}

int GlobalVariables::GetBossHealth()
{
	globalMutex.lock();
	return bossHealth;
	globalMutex.unlock();
}


void GlobalVariables::SetGlobalExcitement(double i)
{
	globalMutex.lock();
	globalExcitement = i;
	globalMutex.unlock();
}

void GlobalVariables::SetGlobalBoredom(double i)
{
	globalMutex.lock();
	globalBoredom = i;
	globalMutex.unlock();
}

void GlobalVariables::SetGlobalFrustration(double i)
{
	globalMutex.lock();
	globalFrustration = i;
	globalMutex.unlock();
}

void GlobalVariables::SetGlobalMeditation(double i)
{
	globalMutex.lock();
	globalMeditation = i;
	globalMutex.unlock();
}

void GlobalVariables::SetDetectedEmotion(string i)
{
	globalMutex.lock();
	detectedEmotion = i;
	globalMutex.unlock();
}

void GlobalVariables::SetGameStart(bool i)
{
	globalMutex.lock();
	gameStart = i;
	globalMutex.unlock();
}

void GlobalVariables::SetBossSpawned(bool i)
{
	globalMutex.lock();
	bossSpawned = i;
	globalMutex.unlock();
}

void GlobalVariables::SetEnemiesKilled(int i)
{
	globalMutex.lock();
	enemiesKilled = i;
	globalMutex.unlock();
}

void GlobalVariables::SetTimesHit(int i)
{
	globalMutex.lock();
	timesHit = i;
	globalMutex.unlock();
}

void GlobalVariables::SetScore(int i)
{
	globalMutex.lock();
	score = i;
	globalMutex.unlock();
}

void GlobalVariables::SetBossHealth(int i)
{
	globalMutex.lock();
	bossHealth = i;
	globalMutex.unlock();
}


string GlobalVariables::currentDateTime()
{
	time_t		now = time(0);
	struct tm	tstruct;
	char		buf[80];
	tstruct =   *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}