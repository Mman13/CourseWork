#include "ScreenManager.h"

GameScreen *currentScreen, *newScreen;

ScreenManager &ScreenManager::GetInstance()
{
	static ScreenManager instance;
	return instance;
}

ScreenManager::ScreenManager()
{

}

ScreenManager::~ScreenManager()
{

}

float ScreenManager::GetAlpha()
{
	return fade.GetAlpha();
}

void ScreenManager::Transition(sf::RenderWindow &Window, sf::Clock clock)
{
	if(transition)
	{
		fade.Update(Window, clock);
		if(fade.GetAlpha() <= 1.0f)
		{
			currentScreen->UnloadContent();
			delete currentScreen;
			currentScreen = newScreen;
			currentScreen->LoadContent();
			newScreen = NULL;
			transition = false;
		}
		else if(fade.GetAlpha() >= 0.0f)
		{
			transition = false;
			fade.SetValue(fade.GetActive(), false);
			fade.SetActive(false);
		}
	}

}

void ScreenManager::AddScreen(GameScreen *screen)
{
	transition = true;
	newScreen = screen;
	fade.SetValue(fade.GetActive(), true);
	//fade.SetActive(true);
	fade.SetAlpha(0.0f);
}

void ScreenManager::Initialise()
{
	currentScreen = new SplashScreen();
	//currentScreen = new TitleScreen();

	transition = false;

	sf::Text text;
	sf::String string;
	sf::Texture texture;
	sf::Vector2f pos;

	fade.LoadContent(string, texture, pos);
	//file.LoadContent("Load/test.cme", attributes, contents);
}

void ScreenManager::LoadContent()
{
	currentScreen->LoadContent();

	sf::Text text;
	sf::String string;
	sf::Texture texture;
	sf::Vector2f pos;

	fade.LoadContent(string, texture, pos);
	fade.SetAlpha(0.0f);
}

void ScreenManager::UnloadContent()
{

}

void ScreenManager::UpdateEvent(sf::Event event, sf::RenderWindow &Window)
{
	currentScreen->UpdateEvent(event, Window);
}

void ScreenManager::Update(sf::RenderWindow &Window, sf::Clock clock)
{
	if(transition == false)
	{
		currentScreen->Update(Window, clock);
	}
	Transition(Window, clock);
}

void ScreenManager::Draw(sf::RenderWindow &Window, sf::Clock clock)
{
	currentScreen->Draw(Window, clock);
}

