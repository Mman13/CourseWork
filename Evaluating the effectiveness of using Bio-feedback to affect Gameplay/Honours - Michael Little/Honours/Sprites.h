#ifndef SPRITES_H
#define SPRITES_H

#include <Windows.h>
#include<SFML/Graphics.hpp>
#include<vector>

struct Sprites	//struct for sprite position and image
{
public:
	Sprites();
	~Sprites();

	int GetTNF();
	//int GetWidth();
	//int GetHeight();
	int GetFrame();
	int GetSet();
	bool GetExist();
	int GetLives();
	bool GetGameOver();
	sf::Vector2f GetPosition();
	sf::Vector2f Get_M_Position();
	sf::Vector2f GetDimensions();
	sf::Vector2f GetVelocity();
	HBITMAP GetBitmap();

	void SetTNF(int i);
	void SetFrame(int i);
	void SetExist(bool i);
	void SetLives(int i);
	void SetGameOver(bool i);
	void SetSet(int i);
	void SetPosition(sf::Vector2f i);
	void Set_M_Position(sf::Vector2f i);
	void SetDimensions(sf::Vector2f i);
	void SetVelocity(sf::Vector2f i);
	void SetBitmap(HBITMAP tempBitmap);

	void ChangeLives(int i);
	void ChangeVelocity(sf::Vector2f i);
	void ChangePosition(sf::Vector2f i);

protected:
	sf::Sprite sprite;
	sf::Vector2f position;
	sf::Vector2f m_position;
	sf::Vector2f dimensions;
	sf::Vector2f velocity;
	//float x, y;
	//float m_x, m_y;
	//int height;
	//int width;
	int frame;
	int set;
	int total_number_frame;
	//int Xvelo;
	//int Yvelo;
	bool exist;
	int lives;
	bool GAME_OVER;

	HBITMAP bitmap;
private:

};
#endif