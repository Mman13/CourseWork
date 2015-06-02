#include "SplashScreen.h"

SplashScreen::SplashScreen()
{

}

SplashScreen::~SplashScreen()
{

}

void SplashScreen::LoadContent()
{
	imageNumber = 0;

	if(!font.loadFromFile("../Fonts/Dead Kansas.ttf"))
	{
		// error...
	}
	text.setString("SplashScreen");
	text.setFont(font);

	keys.push_back(sf::Keyboard::Return);
	keys.push_back(sf::Keyboard::Space);

	file.LoadContent("Load/splash.cme", attributes, contents);

	for(int i = 0; i < attributes.size(); i++)
	{
		for(int j = 0; j < attributes[i].size(); j++)
		{
			std::string att = attributes[i][j];

			if(att == "Image")
			{
				sf::Vector2f pos((sf::VideoMode::getDesktopMode().width) / 3 - 100, (sf::VideoMode::getDesktopMode().height) / 3);
				texture.loadFromFile(contents[i][j]);
				fade.push_back(new FadeAnimation);
				fade[fade.size()-1]->LoadContent(string, texture, pos);
				fade[fade.size()-1]->SetValue(fade[fade.size()-1]->GetActive(), true);
				fade[fade.size()-1]->SetValue(fade[fade.size()-1]->GetScale(), 2.0f);
			}
		}
	}
}

void SplashScreen::UnloadContent()
{
	GameScreen::UnloadContent();
	for(int i = 0; i < fade.size(); i++)
	{
		fade[i]->UnloadContent();
		delete fade[i];
	}
	fade.clear();
}

void SplashScreen::UpdateEvent(sf::Event event, sf::RenderWindow &Window)
{
	input.Update(event);
	
	if(input.KeyPressed(keys))
	{
 		ScreenManager::GetInstance().AddScreen(new TitleScreen);
	}
}

void SplashScreen::Update(sf::RenderWindow &Window, sf::Clock clock)
{
	if(imageNumber >= fade.size() -1)
	{
		ScreenManager::GetInstance().AddScreen(new TitleScreen);
	}
	
	if(imageNumber < fade.size())
	{
		fade[imageNumber]->Update(Window, clock);

		if(fade[imageNumber]->GetAlpha() <= 0.0f)
		{
			imageNumber++;
		}
	}
}

void SplashScreen::Draw(sf::RenderWindow &Window, sf::Clock clock)
{
	if (imageNumber < fade.size())
	{
		fade[imageNumber]->Draw(Window, clock);
		Window.draw(text);
	}
}