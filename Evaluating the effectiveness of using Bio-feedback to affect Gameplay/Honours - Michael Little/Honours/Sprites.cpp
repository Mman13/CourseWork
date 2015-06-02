#include "Sprites.h"

Sprites::Sprites()
{

}
Sprites::~Sprites()
{

}

int Sprites::GetTNF()
{
	return total_number_frame;
}

int Sprites::GetFrame()
{
	return frame;
}

int Sprites::GetSet()
{
	return set;
}

bool Sprites::GetExist()
{
	return exist;
}

int Sprites::GetLives()
{
	return lives;
}

bool Sprites::GetGameOver()
{
	return GAME_OVER;
}

sf::Vector2f Sprites::GetPosition()
{
	return position;
}

sf::Vector2f Sprites::Get_M_Position()
{
	return m_position;
}

sf::Vector2f Sprites::GetDimensions()
{
	return dimensions;
}

sf::Vector2f Sprites::GetVelocity()
{
	return velocity;
}

HBITMAP Sprites::GetBitmap()
{
	return bitmap;
}


void Sprites::SetTNF(int i)
{
	total_number_frame = i;
}

void Sprites::SetFrame(int i)
{
	frame = i;
}

void Sprites::SetExist(bool i)
{
	exist = i;
}

void Sprites::SetLives(int i)
{
	lives = i;
}

void Sprites::SetSet(int i)
{
	set = i;
}

void Sprites::SetGameOver(bool i)
{
	GAME_OVER = i;
}

void Sprites::SetPosition(sf::Vector2f i)
{
	position = i;
}

void Sprites::Set_M_Position(sf::Vector2f i)
{
	m_position = i;
}

void Sprites::SetDimensions(sf::Vector2f i)
{

	dimensions = i;

}

void Sprites::SetVelocity(sf::Vector2f i)
{

	velocity = i;

}

void Sprites::SetBitmap(HBITMAP tempBitmap)
{
	bitmap = tempBitmap;
}


void Sprites::ChangeLives(int i)
{
	lives + i;
}

void Sprites::ChangeVelocity(sf::Vector2f i)
{
	velocity + i;
}

void Sprites::ChangePosition(sf::Vector2f i)
{
	position + i;
}