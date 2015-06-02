#ifndef EMOTIVSHOOTERSCREEN_H
#define EMOTIVSHOOTERSCREEN_H

#include "GameScreen.h"
#include "ScreenManager.h"
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include "Boss.h"
#include "EnemyFire.h"

#include "GlobalVariables.h"

#include<SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include<SFML/System.hpp>
#include<iostream>

using namespace std;
#include <fstream>


#include <Windows.h>
#include <stdio.h>
#include <MMSystem.h>
#include <math.h>
#include <vector>

//#include <future>

class EmotivShooterScreen : public GameScreen
{
public:
	EmotivShooterScreen();
	//EmotivShooterScreen(GlobalVariables* gvPointer);
	~EmotivShooterScreen();

	

	void LoadContent();
	void UnloadContent();
	void UpdateEvent(sf::Event event, sf::RenderWindow &Window);
	void Update(sf::RenderWindow &Window, sf::Clock clock);
	void Draw(sf::RenderWindow &Window, sf::Clock clock);

	void GetEmotiv();


	Player player; // creates an instance of player
	Boss boss;
	std :: vector <Enemy> enemy; // creates the enemy array
	std :: vector <Bullet> bullet; // creates an array for bullets
	std :: vector <EnemyFire> enemyFire;

protected:
private:

	sf::Text text;
	sf::Font font;
	sf::Clock clock;
	sf::Clock clock2;

	sf::Texture bgTex;

	sf::Vector2f mousePos;

	sf::Time timer;

	sf::Mutex shooterMutex;

	int score;
	int killCount;
	int timesHit;

	int minEnemies;


	float	enemySpeed;
	float	fireRate;
	float	bossFireRate;
	float	delayTime;
	float	bossDelayTime;

	bool	paused;
	bool	GameStart;
	bool	GameOver;
	bool	collision;
	bool	enemyHit;
	bool	victory; 
	bool	mouseClicked;

	sf::RectangleShape Background;
	sf::RectangleShape MsgBox; 
	sf::RectangleShape YesBtn; 
	sf::RectangleShape NoBtn; 
	sf::RectangleShape EndBtn; 

///////////////////////////////////
// Emotiv //

	GlobalVariables globalVariables;

	double frustrationScore;
	double excitementScore;
	double meditationScore;
	double boredomScore;

	int updateRate;
	
};

#endif