#ifndef ANIMATION_H
#define ANIMATION_H

#include<SFML/Graphics.hpp>
#include<string>
#include <time.h>

class Animation
{
public:
	Animation();
	~Animation();

	virtual void LoadContent(sf::String string, sf::Texture &texture, sf::Vector2f position);
	virtual void UnloadContent();
	virtual void Update(sf::RenderWindow &Window, sf::Clock clock);
	virtual void Draw(sf::RenderWindow &Window, sf::Clock clock);

	virtual void SetAlpha(float value);
	virtual void SetActive(bool value);

	float &GetAlpha();
	bool &GetActive();
	float &GetScale();
	sf::String &GetString();
	sf::Text &GetText();
	sf::Vector2f &GetPosition();


	template <typename T>
	void SetValue(T &variable, T value)
	{
		variable = value;
	}

	

protected:
	float alpha;
	std::string preText;
	sf::Text text;
	sf::Font font;
	sf::String string;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f position;
	sf::Color textColour;
	sf::Clock clock;

	sf::IntRect sourceRect;
	bool active;

	float scale;

private:

};

#endif