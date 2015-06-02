#include "EmotivShooterScreen.h"
#include<ctime> // for time() function
#include<cstdlib> // for srand() and rand()


namespace {const int NumE = 1;}
EmotivShooterScreen::EmotivShooterScreen()
{
	srand(time(0));// seeds time
}

//EmotivShooterScreen::EmotivShooterScreen(GlobalVariables* gvPointer)
//{
//	//globalVariables = gvPointer;
//}

EmotivShooterScreen::~EmotivShooterScreen()
{

}

void EmotivShooterScreen::LoadContent()
{
	if(!font.loadFromFile("../Fonts/Dead Kansas.ttf"))
	{
		// error...
	}
	text.setString("Game screen");
	text.setColor(sf::Color::Green);
	text.setFont(font);

	Background.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	Background.setPosition(0, 0);
	bgTex.loadFromFile("Sprites/Background.png");

	for(unsigned i = 0; i < NumE; i++)
	{
		enemy.push_back(Enemy());	
		bullet.push_back(Bullet());
		enemyFire.push_back(EnemyFire());
	}

	enemySpeed = 0.3f;
	fireRate = 5.0f;
	bossFireRate = 3.0f;
	delayTime = 0;
	bossDelayTime = 0;
	
	score = 0;
	killCount = 0;
	timesHit = 0;

	minEnemies = 1;

	paused = false;
	GameStart = true;
	GameOver = false;
	collision = false;
	enemyHit = false;
	victory = false;
	mouseClicked = false;

	globalVariables.SetGameStart(true);

/////////////////////////////////////////////
// Emotiv //
	
	frustrationScore = 0;
	excitementScore = 0;
	meditationScore = 0;
	boredomScore = 0;

	updateRate = 0;
}

void EmotivShooterScreen::UnloadContent()
{
	globalVariables.SetGameStart(false);
	globalVariables.SetBossSpawned(false);
	globalVariables.SetScore(0);
	globalVariables.SetEnemiesKilled(0);
	globalVariables.SetTimesHit(0);
	globalVariables.SetBossHealth(0);
	GameScreen::UnloadContent();
}

void EmotivShooterScreen::UpdateEvent(sf::Event event, sf::RenderWindow &Window)
{
	if(victory == false)
	{
		if(paused == false)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{		
				if (player.GetVelocity().x >= -12.0f)
				{
					player.ChangeVelocity(sf::Vector2f(-player.GetPlayerSpeed(), 0.0f));
				}
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				if (player.GetVelocity().x <= 12.0f)
				{
					player.ChangeVelocity(sf::Vector2f(player.GetPlayerSpeed(), 0.0f));
				}
			}
	
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				if (player.GetVelocity().y >= -12.0f)
				{
					player.ChangeVelocity(sf::Vector2f(0.0f, -player.GetPlayerSpeed()));
				}
			}
	
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				if (player.GetVelocity().y <= 12.0f)
				{
					player.ChangeVelocity(sf::Vector2f(0.0f, player.GetPlayerSpeed()));
				}
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::J))
			{
				player.SetAttack(true);
			}
			else
			{
				player.SetAttack(false);
			}

/////////////////////////////////////////////////////////////////
			//if(sf::Keyboard::isKeyPressed(sf::Keyboard::U))
			//{
			//	frustrationScore -= 0.1;
			//}
			//if(sf::Keyboard::isKeyPressed(sf::Keyboard::I))
			//{
			//	frustrationScore += 0.1;
			//}

			//if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			//{
			//	excitementScore -= 0.1;
			//}
			//if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			//{
			//	excitementScore += 0.1;
			//}

			//if(sf::Keyboard::isKeyPressed(sf::Keyboard::V))
			//{
			//	boredomScore -= 0.1;
			//}
			//if(sf::Keyboard::isKeyPressed(sf::Keyboard::B))
			//{
			//	boredomScore += 0.1;
			//}

			//if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			//{
			//	meditationScore -= 0.1;
			//}
			//if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
			//{
			//	meditationScore += 0.1;
			//}

		}// paused loop end

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Pause Menu
			if(paused == true)
			{
				sf::FloatRect ybBB = YesBtn.getGlobalBounds();
		
				sf::FloatRect nbBB = NoBtn.getGlobalBounds();

				if (ybBB.contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseClicked == false)
				{
					ScreenManager::GetInstance().AddScreen(new TitleScreen);
				}

				if (nbBB.contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseClicked == false)
				{
					paused = false;
				}
			}

	} // victory loop end

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// victory screen //

	if(victory == true)
	{
		sf::FloatRect vsBB = EndBtn.getGlobalBounds();


		if (vsBB.contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseClicked == false)
		{
			ScreenManager::GetInstance().AddScreen(new TitleScreen);
		}
	}

////////////////////////////////////////////////////////////////////

	if(input.KeyPressed(sf::Keyboard::P))
	{
		paused = !paused;
	}
	
	input.Update(event);
}



void EmotivShooterScreen::Update(sf::RenderWindow &Window, sf::Clock clock)
{
	if(updateRate >= 5)
	{
		cout << "update" << endl;
		GetEmotiv();
		updateRate = 0;
	}
	else
		updateRate += 1;

	fireRate = 5.0f - (frustrationScore * 2);
	enemySpeed = 0.01f + boredomScore;
	bossFireRate = 1.0f + (excitementScore);

	mousePos = (sf::Vector2<float>)sf::Mouse::getPosition(Window);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	sf::FloatRect playerBB = player.getGlobalBounds();
	sf::FloatRect bossBB = boss.getGlobalBounds();

	if(victory == false)
	{
		if (paused == false)
		{
			delayTime++;
			bossDelayTime++;
			timer = clock2.getElapsedTime();
		//////////////////////////////////////////////////////////////////////////
	
			for(int e =0; e < enemy.size(); e++)
			{
				enemy[e].Update();

				sf::FloatRect enemyBB = enemy[e].getGlobalBounds();

				if(enemy[e].GetPosition().x < -50)
				{
					enemy[e] = enemy[enemy.size() - 1];
					enemy.pop_back();

						if(enemy.size() < 30)
						{
							enemy.push_back(Enemy());
							enemy.push_back(Enemy());
							enemy[enemy.size() - 1].SetPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width + rand()%301,  rand() % 1000));
						}
						else enemy.push_back(Enemy());
							enemy[enemy.size() - 1].SetPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width + rand()%301,  rand() % 1000));
				}
				else
				{
					enemy[e].ChangeVelocity(sf::Vector2f(-enemySpeed, 0.0f));
				}


				if(enemy[e].getPosition().y > 500)
				{
					enemy[e].ChangeVelocity(sf::Vector2f(0.0f, -enemySpeed));
				}
				if(enemy[e].getPosition().y < 500)
				{
					enemy[e].ChangeVelocity(sf::Vector2f(0.0f, enemySpeed));
				}

				if(enemy[e].GetHit() == true) // if enemy is hit delete enemy
				{
					enemy[e] = enemy[enemy.size() - 1];
					enemy.pop_back();

					killCount += 1; //increases kill count, used for research

					for (int i = 0; i < 1; i++)
					{
						enemy.push_back(Enemy());
						enemy[enemy.size() - 1].SetPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width + rand()%301,  rand() % 1000));
					}
				
				}
			

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Player / Bullet Collision //

				if(boss.GetActive() == false)
				{
					if(playerBB.intersects(enemyBB))// Player/Enemy Collision
					{	
						enemy[e].SetHit(true);
						collision = true;
					}
					else 
					{
						//collision = false;
						player.SetInvincible(false);
					}
				}

				for(int b =0; b < bullet.size(); b++)
				{
					sf::FloatRect bulletBB = bullet[b].getGlobalBounds();
				
					if(boss.GetActive() == false)
					{
						if(bulletBB.intersects(enemyBB)) // Bullet/Enemy colision
						{
							enemy[e].SetHit(true);
							score += 100;
							bullet[b] = bullet[bullet.size() - 1];
							bullet.pop_back();
						}
					}

					if(bulletBB.intersects(bossBB)) // Bullet/Boss colision
					{
						boss.SetHit(true);
						bullet[b] = bullet[bullet.size() - 1];
						bullet.pop_back();
					}
					else
					{
						if(boss.GetActive() == true)
						{
							boss.SetInvincible(false);
						}
					}

				}// end bullet loop
			}// enemy loop end


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Boss Functions //

			if(boss.GetHit() == true) // if boss is hit lower its health
			{
				if(boss.GetInvincible() == false)
				{
					if(boss.GetHealth() <= 0)
					{
						if (GameOver == false)
						{
							score += 5000;
							GameOver = true;
						}
						victory = true;
					}
					else 
					{
						score += 1;
						boss.ChangeHealth(-1);
						boss.SetHit(false);
						boss.SetInvincible(true);
					}
				}	
			}

			if(boss.GetAttack() == true)
			{
				for(int f = 0; f < enemyFire.size(); f++)
				{
					sf::FloatRect enemyFireBB = enemyFire[f].getGlobalBounds();
					enemyFire[f].Update();

					if(enemyFire[f].getPosition().x < -50) // Bullet clean up
					{
						enemyFire[f] = enemyFire[enemyFire.size() - 1];
						enemyFire.pop_back();
					}	

					if(bossDelayTime >= bossFireRate)
					{
						EnemyFire temp(&boss);
						sf::Vector2f pos = sf::Vector2f(boss.GetPosition().x + 50, boss.GetPosition().y + (sf::VideoMode::getDesktopMode().height / 2) + (5 + rand()% 90));
						temp.SetPosition(pos);
						enemyFire.push_back(temp);
						bossDelayTime = 0;
					}

					if(playerBB.intersects(enemyFireBB))// Player/EnemyFire Collision
					{
						collision = true;
						enemyFire[f] = enemyFire[enemyFire.size() - 1];
						enemyFire.pop_back();
					}
					else 
					{
						//collision = false;
						player.SetInvincible(false);
					}

				}
			}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Player functions //

			if(player.GetAttack() == true)
			{	
				if(delayTime >= fireRate)
				{
					Bullet temp(&player);
					sf::Vector2f pos = sf::Vector2f(player.GetPosition().x + 70.0f, player.GetPosition().y + 30);
					temp.SetPosition(pos);
					bullet.push_back(temp);
					delayTime = 0;
				}
			}

			for(int b = 0; b < bullet.size(); b++)
			{
				bullet[b].Update();
				if(bullet[b].getPosition().x > sf::VideoMode::getDesktopMode().width) // Bullet clean up
				{
					bullet[b] = bullet[bullet.size() - 1];
					bullet.pop_back();
				}	
			}


			if(collision == true)
			{
				if(player.GetInvincible() == false)
				{
					timesHit += 1; // counts how many times the player was hit, used for research
					if (score <= 0)
					{
						score = 0;
					}
					else 
					{
						score -= 500;
						
						player.SetInvincible(true);
						collision = false;
					}
				}
			}

////////////////////////////////////////////////////////////////////
// set globals //
			globalVariables.SetScore(score);
			globalVariables.SetEnemiesKilled(killCount);
			globalVariables.SetTimesHit(timesHit);
			globalVariables.SetBossHealth(boss.GetHealth());
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// update calls //
			
			player.Update();
			if(timer.asSeconds() >= boss.GetSpawnTime())
			{
				boss.Update();
			}
		}// paused loop end
	}

}

void EmotivShooterScreen::Draw(sf::RenderWindow &Window, sf::Clock clock)
{
	Background.setTexture(&bgTex);
	Window.draw(Background);

	if(boss.GetActive() == false)
	{
		for(int e =0; e < enemy.size(); e++)
		{
			Window.draw(enemy[e]);				
		}
	}

	for(int b =0; b < bullet.size(); b++)
	{
		Window.draw(bullet[b]);				
	}

	if(timer.asSeconds() >= boss.GetSpawnTime())
	{
		Window.draw(boss);
	}

	if(boss.GetAttack() == true)
	{
		for(int f = 0; f < enemyFire.size(); f++)
		{
			Window.draw(enemyFire[f]);
		}
	}

	Window.draw(player);

///////////////////////////////////////////////////////////////////
// text //
	std::ostringstream gs;
	gs << "Score: " << score << endl;
	/*<< "Time: " << timer.asSeconds() << endl
	<< "Frustration: " << frustrationScore << endl
	<< "Excitement: " << excitementScore << endl
	<< "Boredom: " << boredomScore << endl
	<< "meditation: " << meditationScore << endl; */

	text.setString(gs.str());
	text.setPosition(0,20);
	Window.draw(text);

	if(boss.GetActive() == true)
	{
		std::ostringstream bs;
		bs << "Master Controller: " << boss.GetHealth() << endl;
		text.setString(bs.str());
		text.setPosition(sf::VideoMode::getDesktopMode().width / 2, 20);
		globalVariables.SetBossSpawned(true);
	}

	if(paused == true)
	{
		MsgBox.setSize(sf::Vector2f(610,150));
		MsgBox.setPosition((sf::VideoMode::getDesktopMode().width / 3), (sf::VideoMode::getDesktopMode().height / 3));

		YesBtn.setSize(sf::Vector2f(50, 50));
		YesBtn.setPosition((sf::VideoMode::getDesktopMode().width / 3) + 150, (sf::VideoMode::getDesktopMode().height / 3) + 100);
		YesBtn.setFillColor(sf::Color::Blue);

		NoBtn.setSize(sf::Vector2f(50, 50));
		NoBtn.setPosition((sf::VideoMode::getDesktopMode().width / 3) + 400, (sf::VideoMode::getDesktopMode().height / 3) + 100);
		NoBtn.setFillColor(sf::Color::Blue);

		Window.draw(MsgBox);
			std::ostringstream pm;
			pm << "PAUSED " << endl
				<<"Would you like to return to the Main Menu?" << endl;
				text.setString(pm.str());
				text.setCharacterSize(24);
				text.setColor(sf::Color::Green);
				text.setPosition((sf::VideoMode::getDesktopMode().width / 3), (sf::VideoMode::getDesktopMode().height / 3));
			Window.draw(text);

		Window.draw(YesBtn);
			std::ostringstream yb;
			yb << "Yes" << endl;
				text.setString(yb.str());
				text.setCharacterSize(24);
				text.setColor(sf::Color::Green);
				text.setPosition((sf::VideoMode::getDesktopMode().width / 3) + 150, (sf::VideoMode::getDesktopMode().height / 3) + 100);
			Window.draw(text);
			//sf::FloatRect ybBB = YesBtn.getGlobalBounds();

		Window.draw(NoBtn);
			std::ostringstream nb;
			nb << "No" << endl;
				text.setString(nb.str());
				text.setCharacterSize(24);
				text.setPosition((sf::VideoMode::getDesktopMode().width / 3) + 400, (sf::VideoMode::getDesktopMode().height / 3) + 100);
				text.setColor(sf::Color::Green);
			Window.draw(text);
			//sf::FloatRect nbBB = NoBtn.getGlobalBounds();
	}


	if(victory == true)
	{
		EndBtn.setSize(sf::Vector2f(560,200));
		EndBtn.setPosition((sf::VideoMode::getDesktopMode().width / 3), (sf::VideoMode::getDesktopMode().height / 3));
		Window.draw(EndBtn);

		std::ostringstream vs;
		vs << "CONGRATULATIONS!! " << endl
			<< " Your Final Score is: " << score << endl
			<<"Click here to return to the Main Menu?" << endl;
			text.setString(vs.str());
			text.setCharacterSize(24);
			text.setColor(sf::Color::Green);
			text.setPosition((sf::VideoMode::getDesktopMode().width / 3) + 5, (sf::VideoMode::getDesktopMode().height / 3) + 10);
		Window.draw(text);
		//sf::FloatRect vsBB = EndBtn.getGlobalBounds();
	}


	Window.draw(text);
}

void EmotivShooterScreen::GetEmotiv()
{
	shooterMutex.lock();

	frustrationScore = globalVariables.GetGlobalFrustration();
	meditationScore = globalVariables.GetGlobalMeditation();
	boredomScore = globalVariables.GetGlobalBoredom();
	excitementScore = globalVariables.GetGlobalExcitement();

	/*cout << globalVariables.GetGlobalFrustration() << endl
		<< globalVariables.GetGlobalExcitement() << endl
		<< globalVariables.GetGlobalMeditation() << endl
		<< globalVariables.GetGlobalBoredom() << endl;*/

	//cout << frustrationScore << endl
	//	<< excitementScore << endl
	//	<< meditationScore << endl
	//	<< boredomScore << endl;

	shooterMutex.unlock();
}