#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "GameScreen.h"
#include "SplashScreen.h"
#include "ShooterScreen.h"
#include "EmotivShooterScreen.h"
#include "ScreenManager.h"
#include "MenuManager.h"

#include<SFML/Graphics.hpp>
#include<iostream>


class TitleScreen : public GameScreen
{
public:
	TitleScreen();
	~TitleScreen();

	void LoadContent();
	void UnloadContent();
	void UpdateEvent(sf::Event event, sf::RenderWindow &Window);
	void Update(sf::RenderWindow &Window, sf::Clock clock);
	void Draw(sf::RenderWindow &Window, sf::Clock clock);

protected:
private:
	sf::Text text;
	sf::Font font;
	sf::Clock clock;

	sf::Texture menuTex;

	MenuManager menu;

	sf::RectangleShape Background; 
};


#endif