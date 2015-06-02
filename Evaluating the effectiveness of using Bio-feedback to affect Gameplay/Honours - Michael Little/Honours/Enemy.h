#ifndef ENEMY_H
#define ENEMY_H

#include "SpriteAnimation.h"
#include "LoadBitmap.h"
#include "SFML/Graphics.hpp"

class Enemy : public sf::RectangleShape
{
public:
	Enemy();
	~Enemy();

	void Update();
	
	sf::Vector2f GetPosition();
	sf::Vector2f GetDimensions();
	sf::Vector2f GetVelocity();
	int GetMinEnemy();
	bool GetHit();

	void SetPosition(sf::Vector2f i);
	void SetDimensions(sf::Vector2f i);
	void SetVelocity(sf::Vector2f i);
	void SetMinEnemy(int i);
	void SetHit(bool i);

	void ChangeVelocity(sf::Vector2f i);
	void ChangePosition(sf::Vector2f i);
	void ChangeMinEnemy(int i);
	
	

private:

	sf::Vector2f position;
	sf::Vector2f dimensions;
	sf::Vector2f velocity;

	sf::Texture texture;

	int minEnemy;
	bool hit;

};

#endif