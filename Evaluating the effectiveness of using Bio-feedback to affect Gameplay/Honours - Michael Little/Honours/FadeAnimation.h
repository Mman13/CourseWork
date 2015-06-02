#ifndef FADEANIMATION_H
#define FADEANIMATION_H

#include "Animation.h"
#include <time.h>

class FadeAnimation : public Animation
{
public:
	FadeAnimation();
	~FadeAnimation();

	void LoadContent(sf::String string, sf::Texture &texture, sf::Vector2f position);
	void UnloadContent();
	void Update(sf::RenderWindow &Window, sf::Clock clock);
	void Draw(sf::RenderWindow &Window, sf::Clock clock);

	void SetAlpha(float value);

protected:
private:
	bool increase;
	float fadeSpeed;
};

#endif