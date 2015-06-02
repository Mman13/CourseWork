#include "Enemy.h"

Enemy::Enemy()
{
		
		position = sf::Vector2f(sf::VideoMode::getDesktopMode().width + rand()%500,  rand() % sf::VideoMode::getDesktopMode().height);
		dimensions = sf::Vector2f(45, 45);
		velocity = sf::Vector2f(0, 0);
	
		setSize(dimensions);
		setPosition(position);
		//setFillColor(sf::Color::Red);
	
		if(texture.loadFromFile("Sprites/Enemy.png"))
		{	
		}

		setTexture(&texture);
		setTextureRect(sf::IntRect(0,0, 45, 45));

		hit = false;
		minEnemy = 1;
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{
	setTexture(&texture);
	setTextureRect(sf::IntRect(0,0, 45, 45));

	position.x += velocity.x;
	position.y += velocity.y;
	setPosition(position);
}

sf::Vector2f Enemy::GetPosition()
{
	return position;
}

sf::Vector2f Enemy::GetDimensions()
{
	return dimensions;
}

sf::Vector2f Enemy::GetVelocity()
{
	return velocity;
}

int Enemy::GetMinEnemy()
{
	return minEnemy;
}

bool Enemy::GetHit()
{
	return hit;
}


void Enemy::SetPosition(sf::Vector2f position)
{
	setPosition(position);
}

void Enemy::SetDimensions(sf::Vector2f i)
{
	dimensions = i;
}

void Enemy::SetVelocity(sf::Vector2f i)
{
	velocity = i;
}

void Enemy::SetMinEnemy(int i)
{
	minEnemy = i;
}

void Enemy::SetHit(bool i)
{
	hit = i;
}


void Enemy::ChangeVelocity(sf::Vector2f i)
{
	velocity += i;
}

void Enemy::ChangePosition(sf::Vector2f i)
{
	position += i;
}

void Enemy::ChangeMinEnemy(int i)
{
	minEnemy += i;
}