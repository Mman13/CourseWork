#include "FadeAnimation.h"

FadeAnimation::FadeAnimation()
{

}

FadeAnimation::~FadeAnimation()
{

}

void FadeAnimation::LoadContent(sf::String string, sf::Texture &texture, sf::Vector2f position)
{
	if(!font.loadFromFile("../Fonts/Dead Kansas.ttf"))
	{
		// error...
	}
	text.setFont(font);

	Animation::LoadContent(string, texture, position);
	increase = false;
	fadeSpeed = 100.0f;
}

void FadeAnimation::UnloadContent()
{

}

void FadeAnimation::Update(sf::RenderWindow &Window, sf::Clock clock)
{ 

	if(active)
	{
		if(!increase)
		{			
			alpha -= clock.getElapsedTime().asSeconds() * fadeSpeed;
		//	alpha -= clock.restart().asSeconds() * fadeSpeed;
		}
		else
		{
			alpha -= clock.getElapsedTime().asSeconds() * fadeSpeed;
			//alpha += clock.restart().asSeconds() * fadeSpeed;
		}

		if(alpha >= 1.0f)
		{
			alpha = 1.0f;
			increase = false;
		}
		else if(alpha <= 0.0f)
		{
			alpha = 0.0f;
			increase = true;
		}
		//clock.restart();
	}
	else
	{
		alpha = 1.0f;
	}

	Animation::Update(Window, clock);
}

void FadeAnimation::Draw(sf::RenderWindow &Window, sf::Clock clock)
{
	Animation::Draw(Window, clock);
}

void FadeAnimation::SetAlpha(float value)
{
	alpha = value;

	if(alpha == 0.0f)
	{
		increase = true;
	}
	else
	{
		increase = false;
	}

}