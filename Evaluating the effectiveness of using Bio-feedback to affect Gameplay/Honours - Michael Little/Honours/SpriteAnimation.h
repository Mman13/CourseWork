#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include<SFML/Graphics.hpp>
#include "Animation.h"
#include "Sprites.h"
#include <time.h>

class SpriteAnimation : public Animation, Sprites
{
public:
	SpriteAnimation();
	~SpriteAnimation();
	
	Sprites spritesClass;

	void LoadContent(sf::String string, sf::Texture &texture, sf::Vector2f position);
	void UnloadContent();
	void Update();
	void Draw(sf::RenderWindow &Window, sf::Clock clock);

	void jUpdate();//function for jumping animation
	void aUpdate();//function for attack animation
	void eUpdate();//enemy animation
protected:
private:

	int frame;
	int set;
	int PFcounter;
	float timer;
	float time_delay;
	int max_frame;

	
};
#endif