#ifndef BOSS_H
#define BOSS_H

#include "SpriteAnimation.h"
#include "LoadBitmap.h"
#include "SFML/Graphics.hpp"

class Boss : public sf::RectangleShape
{
public:
	Boss();
	~Boss();

	void Update();

	sf::Vector2f GetPosition();
	sf::Vector2f GetDimensions();
	sf::Vector2f GetVelocity();
	int		GetHealth();
	float	GetSpawnTime();
	bool	GetHit();
	bool	GetAttack();
	bool	GetInvincible();
	bool	GetActive();

	void	SetPosition(sf::Vector2f i);
	void	SetDimensions(sf::Vector2f i);
	void	SetVelocity(sf::Vector2f i);
	void	SetHealth(int i);
	void	SetSpawnTime(float i);
	void	SetHit(bool i);
	void	SetAttack(bool i);
	void	SetInvincible(bool i);
	void	SetActive(bool i);

	void	ChangeVelocity(sf::Vector2f i);
	void	ChangePosition(sf::Vector2f i);
	void	ChangeHealth(int i);

private:

	sf::Vector2f position;
	sf::Vector2f dimensions;
	sf::Vector2f velocity;

	int health;

	float spawnTime;

	bool hit;
	bool attack;
	bool invincible;
	bool active;

	sf::Texture texture;

};

#endif