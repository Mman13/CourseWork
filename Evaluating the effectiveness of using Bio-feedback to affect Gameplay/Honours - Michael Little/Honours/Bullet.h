#ifndef Bullet_H
#define Bullet_H

#include "SFML/Graphics.hpp"
#include "Player.h"

class Bullet : public sf::RectangleShape
{
public:
	Bullet();
	Bullet(Player* playerpointer);
	~Bullet();

	void Update();

	sf::Vector2f GetPosition();
	sf::Vector2f GetDimensions();
	sf::Vector2f GetVelocity();

	
	void SetPosition(sf::Vector2f i);
	void SetDimensions(sf::Vector2f i);
	void SetVelocity(sf::Vector2f i);

	void ChangeVelocity(sf::Vector2f i);
	void ChangePosition(sf::Vector2f i);

private:

	Player* player;

	bool drawn;

	sf::Vector2f position;
	sf::Vector2f dimensions;
	sf::Vector2f velocity;

	sf::Texture texture;
};

#endif