#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include<SFML/Graphics.hpp>
#include<vector>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update(sf::Event event);
	bool KeyPressed(int key);
	bool KeyPressed(std::vector<int> keys);

	bool KeyReleased(int key);
	bool KeyReleased(std::vector<int> keys);

	//bool KeyDown(sf::RenderWindow &Window, sf::Key::Code key);
	//bool KeyDown(sf::RenderWindow &Window, std::vector<sf::Key::Code> keys);

protected:
private:
	sf::Event event;
};

#endif