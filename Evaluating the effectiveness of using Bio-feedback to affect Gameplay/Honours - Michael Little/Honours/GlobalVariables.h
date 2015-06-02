#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include<SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include<SFML/System.hpp>

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace std;

class GlobalVariables
{
public:
	GlobalVariables();
	~GlobalVariables();

	double GetGlobalExcitement();
	double GetGlobalBoredom();
	double GetGlobalFrustration();
	double GetGlobalMeditation();
	string GetDetectedEmotion();
	bool GetGameStart();
	bool GetBossSpawned();
	int GetEnemiesKilled();
	int GetTimesHit();
	int GetScore();
	int GetBossHealth();

	void SetGlobalExcitement(double i);
	void SetGlobalBoredom(double i);
	void SetGlobalFrustration(double i);
	void SetGlobalMeditation(double i);
	void SetDetectedEmotion(string i);
	void SetGameStart(bool i);
	void SetBossSpawned(bool i);
	void SetEnemiesKilled(int i);
	void SetTimesHit(int i);
	void SetScore(int i);
	void SetBossHealth(int i);


	string currentDateTime();


private:
	sf::Mutex globalMutex;
};
#endif