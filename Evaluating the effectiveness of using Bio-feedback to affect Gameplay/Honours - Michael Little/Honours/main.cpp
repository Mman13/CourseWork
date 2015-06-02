#include "Emotiv.h"
#include "ScreenManager.h"
Emotiv* emo;

#include "GlobalVariables.h"


#include <iostream>
using namespace std;
#include <fstream>
ofstream outputFile;

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#include <Windows.h>
#include <stdio.h>
#include <MMSystem.h>
#include <math.h>
#include <vector>

#include <time.h>
#include <ctime>
time_t rawTime;
struct tm *current;

static float frameCount = 0;
double ID = 1;
int updateRate = 0;

string mode;
void Update();

void EmotivUpdate();

sf::Mutex emotivMutex;

int main(int argc, char* argv[]) 
{
	emo = new Emotiv();
	//GlobalVariables globalVariables;

	// sets up text
//////////////////////////////////////////////////////
	sf::Font font;
	if(!font.loadFromFile("../Fonts/arial.ttf"))
	{
		// error...
	}

	sf::Text text;
	text.setFont(font);
	text.setColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setPosition(0,0);

//////////////////////////////////////////////////////

	sf::RectangleShape Fade(sf::Vector2f(1000, 1000));
//////////////////////////////////////////////////////

	sf::VideoMode RSmode(600,400,32);
	sf::VideoMode GSmode(1550, 1000, 32);
	sf::RenderWindow main(GSmode, "Game Screen", sf::Style::Fullscreen);
	main.setPosition(sf::Vector2i(0, 0));

	ScreenManager::GetInstance().Initialise();
	ScreenManager::GetInstance().LoadContent();

	//main.EnableKeyRepeat(false);

	while(main.isOpen())
	{
		sf::Event event;
		sf::Clock clock;

		if(main.isOpen())
		{
			main.clear();
			main.pushGLStates();

			main.setFramerateLimit(24);
			
			while(main.pollEvent(event))
			{	
				if(event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
				{
					main.close();
				}

				ScreenManager::GetInstance().UpdateEvent(event, main);
			}	

			main.clear(sf::Color::Black);

			ScreenManager::GetInstance().Update(main, clock);
			Fade.setFillColor(sf::Color(0, 0, 0, 255 * ScreenManager::GetInstance().GetAlpha()));
			
			ScreenManager::GetInstance().Draw(main, clock);
			main.draw(Fade);

			
			sf::Thread thread(&EmotivUpdate);
			thread.launch();
			
			main.popGLStates();
			main.display();
			
		}
	}
	
}


void EmotivUpdate()
{
	GlobalVariables* globalPointer = new GlobalVariables();
	//GlobalVariables* globalVariables;
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Emotiv //
	//globalVariables = globalPointer;

	emotivMutex.lock();

	//cout << "emotiv thread running" << endl;
	emo->checkForNewEvent();
	emo->parseAffectiv(emo->getEstate());
	globalPointer->SetGlobalExcitement(emo->getAffectiv().shortTermExcitementScore);
	globalPointer->SetGlobalBoredom(emo->getAffectiv().engagementBoredomScore);
	globalPointer->SetGlobalFrustration(emo->getAffectiv().frustrationScore);
	globalPointer->SetGlobalMeditation(emo->getAffectiv().meditationScore);
	globalPointer->SetDetectedEmotion(emo->getAffectiv().emotion);


	globalPointer->SetGlobalExcitement(0.5);
	globalPointer->SetGlobalBoredom(0.5);
	globalPointer->SetGlobalFrustration(0.5);
	globalPointer->SetGlobalMeditation(0.5);

	//emotionDetected = emo->getAffectiv().emotion;

	emotivMutex.unlock();

	//cout << "current time " << globalPointer->currentDateTime() << endl;
	
	if(globalPointer->GetGameStart() == true)
	{

		if(updateRate >= 20)
		{
			//cout << "update text" << endl;
			/* Specify file to output to */
			outputFile.open("../Emotions.txt", ios:: in | ios::out | ios::ate);
			outputFile << "Current time: " << globalPointer->currentDateTime() << endl;

			outputFile << "Current Score: " << globalPointer->GetScore() << endl;
			outputFile << "Number of Enemies destroyed: " << globalPointer->GetEnemiesKilled() << endl;
			outputFile << "Number of times hit: " << globalPointer->GetTimesHit() << endl;

			if(globalPointer->GetBossSpawned() == true)
			{
				outputFile << "Boss has spawned" << endl
					<< "Boss health: " << globalPointer->GetBossHealth() << endl;
			}

			outputFile << "User Excitement: " << emo->getAffectiv().shortTermExcitementScore << endl
						<< "User Boredom: " <<  emo->getAffectiv().engagementBoredomScore << endl
						<< "User Frustration: " << emo->getAffectiv().frustrationScore << endl
						<< "User Meditation: " << emo->getAffectiv().meditationScore << endl 
						<< "User Emotional State: " << emo->getAffectiv().emotion << endl << endl;
			outputFile.close();
			updateRate = 0;
		}
		else
			updateRate += 1;
	}
	
}