#include "Animation.h"

Animation::Animation()
{

}

Animation::~Animation()
{

}

void Animation::LoadContent(sf::String string, sf::Texture &texture, sf::Vector2f position)
{
	if(!font.loadFromFile("../Fonts/Dead Kansas.ttf"))
	{
		// error...
	}
	text.setFont(font);

	this->text = text;
	this->string = string;
	this->texture = texture;
	this->position = position;

	alpha = 1.0f;
	textColour = sf::Color(114, 66, 161);
	if(texture.getSize().y > 0);
	{
		sprite.setTexture(this->texture);
	}
	this->text = text;
	this->string = string;
	active = false;
}

void Animation::UnloadContent()
{
	
}

void Animation::Update(sf::RenderWindow &Window, sf::Clock clock)
{
	
}

void Animation::Draw(sf::RenderWindow &Window, sf::Clock clock)
{
	//std::string str = text.getString();
	//std::string str = string;
	//sprite.setTextureRect(sourceRect);
	text.setString(string);

	//if(sprite.getTexture()!=NULL)
		//{
		sprite.setColor(sf::Color(255, 255, 255, 255 * alpha));
		sprite.setPosition(position);
		sprite.setScale(scale, scale);
		Window.draw(sprite);

	//}
	//if(str != "")
	//{	
		text.setColor(sf::Color(255, 255, 255, 255 * alpha));
		text.setPosition(position);
		text.setScale(scale, scale);
		Window.draw(text);
	//}

}

float &Animation::GetAlpha()
{
	return alpha;
}

bool &Animation::GetActive()
{
	return active;
}

float &Animation::GetScale()
{
	return scale;
}

sf::Vector2f &Animation::GetPosition()
{
	return position;
}

sf::String &Animation::GetString()
{
	return string;
}
sf::Text &Animation::GetText()
{
	return text;
}

void Animation::SetActive(bool value)
{
	active = value;
}

void Animation::SetAlpha(float value)
{
	alpha = value;
}

