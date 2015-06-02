#include "EnemyFire.h"

EnemyFire::EnemyFire()
{
}

EnemyFire::EnemyFire(Boss* bosspointer)
{
	boss = bosspointer;
	dimensions = sf::Vector2f(15, 15);
	velocity = sf::Vector2f(-10, -5 + rand()% 10);

	drawn = false;

	setSize(dimensions);
	//setFillColor(sf::Color::Blue);
	
	if(texture.loadFromFile("Sprites/EnergyBall.png"))
	{
	}

	setTexture(&texture);
	setTextureRect(sf::IntRect(0,0, 50, 50));
}

EnemyFire::~EnemyFire()
{

}

void EnemyFire::Update()
{
	setTexture(&texture);
	setTextureRect(sf::IntRect(0,0, 50, 50));
	if (drawn == false)
	{
		position = sf::Vector2f(boss->GetPosition().x + 50, boss->GetPosition().y + (sf::VideoMode::getDesktopMode().height / 2) + (5 + rand()% 90));
		drawn = true;
	}

	position.x += velocity.x;
	position.y += velocity.y;
	setPosition(position);
}

sf::Vector2f EnemyFire::GetPosition()
{
	return position;
}

sf::Vector2f EnemyFire::GetDimensions()
{
	return dimensions;
}

sf::Vector2f EnemyFire::GetVelocity()
{
	return velocity;
}


void EnemyFire::SetPosition(sf::Vector2f i)
{
	sf::RectangleShape::setPosition(i);
}

void EnemyFire::SetDimensions(sf::Vector2f i)
{
	dimensions = i;
}

void EnemyFire::SetVelocity(sf::Vector2f i)
{
	velocity = i;
}


void EnemyFire::ChangeVelocity(sf::Vector2f i)
{
	velocity += i;
}

void EnemyFire::ChangePosition(sf::Vector2f i)
{
	position + i;
}