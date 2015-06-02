#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include<string>
#include<iostream>

#include "GameScreen.h"
#include "SplashScreen.h"
#include "TitleScreen.h"
#include "ShooterScreen.h"
#include "FadeAnimation.h"
#include "FileManager.h"

//#define ScreenWidth 800
//#define ScreenHeight 600

class ScreenManager
{
public:
	~ScreenManager();
	static ScreenManager &GetInstance();

	void Initialise();
	void LoadContent();
	void UnloadContent();
	void UpdateEvent(sf::Event event, sf::RenderWindow &Window);
	void Update(sf::RenderWindow &Window, sf::Clock clock);
	void ScreenManager::Draw(sf::RenderWindow &Window, sf::Clock clock);

	void AddScreen(GameScreen *screen);

	float GetAlpha();

protected:
private:
	//GameScreen *currentScreen, *newScreen;

	ScreenManager();
	ScreenManager(ScreenManager const&);
	void operator = (ScreenManager const&);

	void Transition(sf::RenderWindow &Window, sf::Clock clock);
	bool transition;

	FadeAnimation fade;
	GameScreen *newScreen;
	FileManager file;

	std::vector<std::vector<std::string>> attributes;
	std::vector<std::vector<std::string>> contents;
};

#endif