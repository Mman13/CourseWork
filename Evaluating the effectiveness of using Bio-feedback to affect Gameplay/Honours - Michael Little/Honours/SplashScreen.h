#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "GameScreen.h"
#include "TitleScreen.h"
#include "ScreenManager.h"
#include "FadeAnimation.h"

#include<SFML/Graphics.hpp>
#include<iostream>


class SplashScreen : public GameScreen
{
public:
	SplashScreen();
	~SplashScreen();

	void LoadContent();
	void UnloadContent();
	void UpdateEvent(sf::Event event, sf::RenderWindow &Window);
	void Update(sf::RenderWindow &Window, sf::Clock clock);
	void Draw(sf::RenderWindow &Window, sf::Clock clock);
protected:
private:
	sf::Text text;
	sf::String string;
	sf::Font font;
	std::vector<int> keys; 

	sf::Texture texture;
	std::vector<FadeAnimation*> fade;

	FileManager file;

	int imageNumber;
};

#endif