#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::Bullet(Player* playerpointer)
{
	player = playerpointer;
	//position = sf::Vector2f(player.GetPosition().x +3, player.GetPosition().y);
	dimensions = sf::Vector2f(15, 5);
	velocity = sf::Vector2f(25, 0);

	drawn = false;

	setSize(dimensions);
	//setFillColor(sf::Color::Green);

	if(texture.loadFromFile("Sprites/Laser.png"))
	{
	}

	setTexture(&texture);
	setTextureRect(sf::IntRect(0,0, 50, 20));
}

Bullet::~Bullet()
{

}

void Bullet::Update()
{
	setTexture(&texture);
	setTextureRect(sf::IntRect(0,0, 50, 20));

	if (drawn == false)
	{
		position = sf::Vector2f(player->GetPosition().x +70, player->GetPosition().y + 30);
		drawn = true;
	}

	position.x += velocity.x;
	position.y += velocity.y;
	setPosition(position);
}

sf::Vector2f Bullet::GetPosition()
{
	return position;
}

sf::Vector2f Bullet::GetDimensions()
{
	return dimensions;
}

sf::Vector2f Bullet::GetVelocity()
{
	return velocity;
}


void Bullet::SetPosition(sf::Vector2f i)
{
	sf::RectangleShape::setPosition(i);
}

void Bullet::SetDimensions(sf::Vector2f i)
{
	dimensions = i;
}

void Bullet::SetVelocity(sf::Vector2f i)
{
	velocity = i;
}


void Bullet::ChangeVelocity(sf::Vector2f i)
{
	velocity += i;
}

void Bullet::ChangePosition(sf::Vector2f i)
{
	position + i;
}