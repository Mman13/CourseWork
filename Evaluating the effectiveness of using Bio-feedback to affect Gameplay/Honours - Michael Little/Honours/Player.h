#ifndef PLAYER_H
#define PLAYER_H

#include "SpriteAnimation.h"
#include "LoadBitmap.h"
#include "SFML/Graphics.hpp"
//#include <RectangleShape>

class Player :  public sf::RectangleShape
{
public:
	Player();
	~Player();

	void Update();

	sf::Vector2f GetPosition();
	sf::Vector2f GetDimensions();
	sf::Vector2f GetVelocity();

	int GetLives();

	float GetPlayerSpeed();

	bool GetInvincible();
	bool GetAttack();

	void SetAttack(bool i);
	void SetInvincible(bool i);
	void SetLives(int i);
	void SetPosition(sf::Vector2f i);
	void SetDimensions(sf::Vector2f i);
	void SetVelocity(sf::Vector2f i);

	void ChangeLives(int i);
	void ChangePlayerSpeed(float i);
	void ChangeVelocity(sf::Vector2f i);
	void ChangePosition(sf::Vector2f i);

private:
	int		lives;

	float	playerSpeed;

	bool	attack;
	bool	invincible;

	sf::Vector2f position;
	sf::Vector2f dimensions;
	sf::Vector2f velocity;
	sf::Texture texture;

	//sf::RectangleShape player;
};

#endif