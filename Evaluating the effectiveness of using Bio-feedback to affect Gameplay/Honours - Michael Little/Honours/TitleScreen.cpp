#include "TitleScreen.h"

TitleScreen::TitleScreen()
{

}

TitleScreen::~TitleScreen()
{

}

void TitleScreen::LoadContent()
{
	if(!font.loadFromFile("../Fonts/Dead Kansas.ttf"))
	{
		// error...
	}
	text.setString("TitleScreen");
	text.setFont(font);

	Background.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	Background.setPosition(0, 0);
	menuTex.loadFromFile("Sprites/Background.png");

	keys.push_back(sf::Keyboard::Return);
	keys.push_back(sf::Keyboard::Space);

	menu.LoadContent("Title");
}

void TitleScreen::UnloadContent()
{
	GameScreen::UnloadContent();
	menu.UnloadContent();
}

void TitleScreen::UpdateEvent(sf::Event event, sf::RenderWindow &Window)
{
	input.Update(event);
	menu.UpdateEvent(input);
	
	if(input.KeyPressed(keys) && menu.GetItemNumber() == 0)
	{
		ScreenManager::GetInstance().AddScreen(new ShooterScreen);		
	}

	if(input.KeyPressed(keys) && menu.GetItemNumber() == 1)
	{
		ScreenManager::GetInstance().AddScreen(new EmotivShooterScreen);	
	}

	if(input.KeyPressed(keys) && menu.GetItemNumber() == 2)
	{
		Window.close();		
	}
}

void TitleScreen::Update(sf::RenderWindow &Window, sf::Clock clock)
{
	menu.Update(Window, clock);
}

void TitleScreen::Draw(sf::RenderWindow &Window, sf::Clock clock)
{
	Background.setTexture(&menuTex);
	Window.draw(Background);

	std::ostringstream controls;
	controls << "Controls: " << endl
			<< "W = Up" << endl
			<< "A = Backwards" << endl
			<< "S = Down" << endl
			<< "D = Forward" << endl
			<< "J = Fire" << endl
			<< "P = Pause" << endl;
	text.setString(controls.str());
	text.setPosition( sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2);


	Window.draw(text);

	menu.Draw(Window, clock);
}