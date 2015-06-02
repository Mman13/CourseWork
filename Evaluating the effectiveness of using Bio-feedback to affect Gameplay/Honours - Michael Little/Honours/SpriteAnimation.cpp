#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation()
{
	PFcounter = 0;
}
SpriteAnimation::~SpriteAnimation()
{

}

void SpriteAnimation::LoadContent(sf::String string, sf::Texture &texture, sf::Vector2f position)
{

}

void SpriteAnimation::UnloadContent()
{

}

void SpriteAnimation::Update()
{ 
	if(PFcounter >= 5)
	{
		frame++;
		set=1;

		if (frame >= spritesClass.GetTNF())
		{
			frame = 0; //resets the frame after walking animation
		}
	
		PFcounter = 0;

	}
	else
	{
		PFcounter++;
	}

}

void SpriteAnimation::jUpdate()
{
	set = 2;
}

void SpriteAnimation::aUpdate()
{
	if(PFcounter >= 5)
	{
		frame++;
		set=3;

		if (frame >= spritesClass.GetTNF())
		{
			frame = 0; //resets the frame after walking animation
		}
	
		PFcounter = 0;

	}
	else
	{
		PFcounter++;
	}
}

void SpriteAnimation::eUpdate()
{
	if(PFcounter >= 2)
	{
		frame++;

		if (frame >= spritesClass.GetTNF())
		{
			frame = 0; //resets the frame after walking animation
		}
	
		PFcounter = 0;

	}
	else
	{
		PFcounter++;
	}
}

void SpriteAnimation::Draw(sf::RenderWindow &Window, sf::Clock clock)
{
	//Animation::Draw(Window, clock);
}