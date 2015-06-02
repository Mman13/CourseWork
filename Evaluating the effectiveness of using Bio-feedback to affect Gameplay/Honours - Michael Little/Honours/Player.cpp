#include "Player.h"

Player::Player()
{
	position = sf::Vector2f(100, 500);
	dimensions = sf::Vector2f(100, 50);
	velocity = sf::Vector2f(0, 0);

	setSize(dimensions);
	setPosition(position);
	//setFillColor(sf::Color::Blue);
	//player.setTexture(spriteAni.spritesClass.GetBitmap());
	
	if(texture.loadFromFile("Sprites/SpaceShip.png"))
	{
	}

	setTexture(&texture);
	setTextureRect(sf::IntRect(0,0, 100, 50));

	playerSpeed = 5.0f;
	lives = 3;
	attack = false;
}

Player::~Player()
{

}

void Player::Update()
{
		if(GetVelocity().x > 0)// slows player down when not moving
		{
			ChangeVelocity(sf::Vector2f(-1.0f, 0.0f));
		}
		if(GetVelocity().x < 0)// slows player down when not moving
		{
			ChangeVelocity(sf::Vector2f(1.0f, 0.0f));
		}
		if(GetVelocity().y > 0)// slows player down when not moving
		{
			ChangeVelocity(sf::Vector2f(0.0f, -1.0f));
		}
		if(GetVelocity().y < 0)// slows player down when not moving
		{
			ChangeVelocity(sf::Vector2f(0.0f, 1.0f));
		}

		if(getPosition().x > sf::VideoMode::getDesktopMode().width / 3) // player boundries
		{
			ChangeVelocity(sf::Vector2f(-playerSpeed - 9, 0.0f));
		}
		if(getPosition().x < 0)
		{
			ChangeVelocity(sf::Vector2f(playerSpeed + 9, 0.0f));
		}
		if(getPosition().y > sf::VideoMode::getDesktopMode().height - 50)
		{
			ChangeVelocity(sf::Vector2f(0.0f, -playerSpeed - 9));
		}
		if(getPosition().y < 0)
		{
			ChangeVelocity(sf::Vector2f(0.0f, playerSpeed + 9));
		}



	position.x += velocity.x;
	position.y += velocity.y;
	setPosition(position);
}

sf::Vector2f Player::GetPosition()
{
	return position;
	//return getPosition();
}

sf::Vector2f Player::GetDimensions()
{
	return dimensions;
}

sf::Vector2f Player::GetVelocity()
{
	return velocity;
}

int Player::GetLives()
{
	return lives;
}

float Player::GetPlayerSpeed()
{
	return playerSpeed;
}

bool Player::GetAttack()
{
	return attack;
}

bool Player::GetInvincible()
{
	return invincible;
}


void Player::SetPosition(sf::Vector2f position)
{
	setPosition(position);
}

void Player::SetDimensions(sf::Vector2f i)
{
	dimensions = i;

}

void Player::SetVelocity(sf::Vector2f i)
{
	velocity = i;
}

void Player::SetAttack(bool i)
{
	attack = i;
}

void Player::SetInvincible(bool i)
{
	invincible = i;
}


void Player::ChangeLives(int i)
{
	lives += i;
}

void Player::ChangePlayerSpeed(float i)
{
	playerSpeed += i;
}

void Player::ChangeVelocity(sf::Vector2f i)
{
	velocity += i;
}

void Player::ChangePosition(sf::Vector2f i)
{
	position += i;
}