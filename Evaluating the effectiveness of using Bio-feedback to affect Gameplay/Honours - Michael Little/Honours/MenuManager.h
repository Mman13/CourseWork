#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include<SFML/Graphics.hpp>
#include"Animation.h"
#include"FadeAnimation.h"
#include"InputManager.h"
#include"FileManager.h"


class MenuManager
{
public:
	MenuManager();
	~MenuManager();

	void LoadContent(std::string menuID);
	void UnloadContent();
	void UpdateEvent(InputManager input);
	void Update(sf::RenderWindow &Window, sf::Clock clock);
	void Draw(sf::RenderWindow &Window, sf::Clock clock);

	int GetItemNumber();

protected:
private:
	std::vector<std::vector<std::string>> attributes;
	std::vector<std::vector<std::string>> contents;

	FileManager file;

	std::vector<std::vector<Animation*>> animation;
	std::vector<Animation*> tempAnimation;

	sf::Clock clock;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Text text;
	sf::String string;

	std::vector<sf::Texture> textures;
	std::vector<sf::Texture> menuImages;

	std::vector<sf::String> menuItems;

	sf::Vector2f position;
	int axis;
	std::string align;
	std::vector<std::string> animationTypes;

	void EqualizeMenuItems();
	void SetAnimations();
	void SetPositions();

	sf::Texture null;

	int itemNumber;
};

#endif