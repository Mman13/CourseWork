#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include<SFML/Graphics.hpp>
#include "InputManager.h"
#include "FileManager.h"

class GameScreen
{
public:
	GameScreen();
	~GameScreen();

	virtual void LoadContent();
	virtual void UnloadContent();
	virtual void UpdateEvent(sf::Event event, sf::RenderWindow &Window);
	virtual void Update(sf::RenderWindow &Window, sf::Clock clock);
	virtual void Draw(sf::RenderWindow &Window, sf::Clock clock);
protected:
	InputManager input;
	std::vector<int> keys;

	FileManager file;
	std::vector<std::vector<std::string>> attributes;
	std::vector<std::vector<std::string>> contents;
private:

};
#endif