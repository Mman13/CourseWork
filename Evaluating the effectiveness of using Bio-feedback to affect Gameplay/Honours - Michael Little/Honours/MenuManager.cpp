#include "MenuManager.h"

MenuManager::MenuManager()
{

}

MenuManager::~MenuManager()
{

}

void MenuManager::EqualizeMenuItems()
{
	for(int i =0; i < menuItems.size(); i++)
	{
		if(menuImages.size() == i)
		{
			menuImages.push_back(null);
		}
	}

	for(int i = 0; i < menuImages.size(); i++)
	{
		if(menuItems.size() == i)
		{
			menuItems.push_back(string);
		}
	}
}

void MenuManager::SetAnimations()
{
	for(int i = 0; i < menuItems.size(); i++)
	{
		for(int j =0; j < animationTypes.size(); j++)
		{
			if(animationTypes[j] == "Fade")
			{
				tempAnimation.push_back(new FadeAnimation());
				tempAnimation[tempAnimation.size()-1]->LoadContent( menuItems[i], menuImages[i], position);
				tempAnimation[tempAnimation.size()-1]->SetValue(tempAnimation[tempAnimation.size()-1]->GetActive(), true);
				tempAnimation[tempAnimation.size()-1]->SetValue(tempAnimation[tempAnimation.size()-1]->GetScale(), 1.25f);
			}
		}
		animation.push_back(tempAnimation);
		tempAnimation.clear();
	}
}

void MenuManager::SetPositions()
{
	sf::Vector2f pos = position;

	for(int i = 0; i < animation.size(); i++)
	{
		for(int j = 0; j < animation[i].size(); j++)
		{
			animation[i][j]->SetValue(animation[i][j]->GetPosition(), pos);
		}

		float width = menuImages[i].getSize().x + animation[i][0]->GetText().getGlobalBounds().width;

		float height = menuImages[i].getSize().y + animation[i][0]->GetText().getGlobalBounds().height;

		if(axis == 1)
		{
			pos.x += 10 + width;
		}
		else
		{
			pos.y += (50 + height );
		}
	}
}

void MenuManager::LoadContent(std::string menuID)
{
	null.loadFromFile("null.png");
	itemNumber = 0;

	file.LoadContent("Load/menus.cme", attributes, contents, menuID);

	for(int i = 0; i < attributes.size(); i++)
	{
		for(int j = 0; j < attributes[i].size(); j++)
		{
			std::string att = attributes[i][j];
			std::string con = contents[i][j];

			if(att == "Item")
			{
				menuItems.push_back(con);
			}
			else if(att == "Image")
			{
				texture.loadFromFile(con);
				textures.push_back(texture);
				sprite.setTexture(textures[textures.size()-1]);
				menuImages.push_back(texture);
			}
			else if(att == "Position")
			{
				std::string pos[2];
				pos[0] = con.substr(0, con.find(','));
				pos[1] = con.substr(con.find(',') + 1);

				position.x = atof(pos[0].c_str());
				position.y = atof(pos[1].c_str());

				std::cout << "Pos: "<< position.x <<" "<< position.y << std::endl;
			}
			else if(att == "Align")
			{
				align = con;
			}
			else if(att == "Axis")
			{
				axis = atoi(con.c_str());
			}
			else if(att == "Animation")
			{
				animationTypes.push_back(con);
			}
		}
	}

	EqualizeMenuItems();
	SetAnimations();
	SetPositions();
}

void MenuManager::UnloadContent()
{
	menuItems.clear();
	menuImages.clear();

	for(int i = 0; i < animation.size(); i++)
	{
		for(int j = 0; j < animation[i].size(); j++)
		{
			delete animation[i][j];
		}
	}

	animation.clear();
	animationTypes.clear();

	attributes.clear();
	contents.clear();
}

void MenuManager::UpdateEvent(InputManager input)
{
		if(axis == 1)
		{
			if(input.KeyPressed(sf::Keyboard::Right) || input.KeyPressed(sf::Keyboard::D))
			{
				itemNumber++;
			}

			if(input.KeyPressed(sf::Keyboard::Left) || input.KeyPressed(sf::Keyboard::A))
			{
				itemNumber--;
			}
		}
		else
		{
			if(input.KeyPressed(sf::Keyboard::Up) || input.KeyPressed(sf::Keyboard::W))
			{
				itemNumber--;
			}

			if(input.KeyPressed(sf::Keyboard::Down) || input.KeyPressed(sf::Keyboard::S))
			{
				itemNumber++;
			}
		}


}

void MenuManager::Update(sf::RenderWindow &Window, sf::Clock clock)
{
	if(itemNumber < 0)
	{
		itemNumber = 0;
	}
	else if (itemNumber > menuItems.size() - 1 )
	{
		itemNumber = menuItems.size() - 1;
	}

	for(int i = 0; i < animation.size(); i++)
	{
		for(int j = 0; j < animation[i].size(); j++)
		{
			animation[i][j]->SetValue(animation[i][j]->GetActive(), false);
			if(i == itemNumber)
			{
				animation[i][j]->SetValue(animation[i][j]->GetActive(), true);
			}
			animation[i][j]->Update(Window, clock);
		}
	}
}

void MenuManager::Draw(sf::RenderWindow &Window, sf::Clock clock)
{
	for(int i = 0; i < animation.size(); i++)
	{
		for(int j = 0; j < animation[i].size(); j++)
		{
			animation[i][j]->Draw(Window, clock);
		}
	}
}

int MenuManager::GetItemNumber()
{
	return itemNumber;
}