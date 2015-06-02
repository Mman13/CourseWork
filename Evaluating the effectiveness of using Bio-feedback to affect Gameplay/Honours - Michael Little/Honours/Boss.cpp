#include "Boss.h"

Boss::Boss()
{
	position = sf::Vector2f(sf::VideoMode::getDesktopMode().width + 500, 0);
	dimensions = sf::Vector2f(300, sf::VideoMode::getDesktopMode().height);
	velocity = sf::Vector2f(-1, 0);

	//setRadius(500);
	setSize(dimensions);
	setPosition(position);
	//setFillColor(sf::Color::Magenta);

	health = 200;
	spawnTime = 50.0f;

	active = false;
	attack = false;
	invincible = true;

	if(texture.loadFromFile("Sprites/MasterControler.png"))
	{
	}

	setTexture(&texture);
	setTextureRect(sf::IntRect(0,-200, 300, 1000));
	
}

Boss::~Boss()
{

}

void Boss::Update()
{

	if(position.x < sf::VideoMode::getDesktopMode().width - 300)
	{
		SetVelocity(sf::Vector2f(0.0f, 0.0f));
		active = true;
	}

	if(active == true)
	{
		invincible = false;
		attack = true;
	}


	position.x += velocity.x;
	position.y += velocity.y;
	setPosition(position);
}

sf::Vector2f Boss::GetPosition()
{
	return position;
}

sf::Vector2f Boss::GetDimensions()
{
	return dimensions;
}

sf::Vector2f Boss::GetVelocity()
{
	return velocity;
}

int Boss::GetHealth()
{
	return health;
}

float Boss::GetSpawnTime()
{
	return spawnTime;
}

bool Boss::GetHit()
{
	return hit;
}

bool Boss::GetAttack()
{
	return attack;
}

bool Boss::GetInvincible()
{
	return invincible;
}

bool Boss::GetActive()
{
	return active;
}


void Boss::SetPosition(sf::Vector2f position)
{
	setPosition(position);
}

void Boss::SetDimensions(sf::Vector2f i)
{
	dimensions = i;
}

void Boss::SetVelocity(sf::Vector2f i)
{
	velocity = i;
}

void Boss::SetHealth(int i)
{
	health = i;
}

void Boss::SetSpawnTime(float i)
{
	spawnTime = i;
}

void Boss::SetHit(bool i)
{
	hit = i;
}

void Boss::SetAttack(bool i)
{
	attack = i;
}

void Boss::SetInvincible(bool i)
{
	invincible = i;
}

void Boss::SetActive(bool i)
{
	active = i;
}


void Boss::ChangeVelocity(sf::Vector2f i)
{
	velocity += i;
}

void Boss::ChangePosition(sf::Vector2f i)
{
	position += i;
}

void Boss::ChangeHealth(int i)
{
	health += i;
}