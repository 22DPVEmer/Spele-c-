#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;


class Game
{
public:
	//Konstruktori / Destruktori
	Game();
	virtual ~Game();

	//konstanti
	const bool running() const;
	const bool getendGame() const;
	//Funkcijas
	void spawnEnemy();
	void pollEvents();
	void updateMousePos();
	void updateText();
	void update();
	void updateEnemies();

	void rendertext(RenderTarget& target);
	void renderEnemies(RenderTarget& target);
	void render();


	RenderWindow* window;
	Event ev;
	VideoMode videomode;


	Vector2i MousePosWindow;
	Vector2f MousePosView;

	Font font;

	Text Uitext;


	bool endGame;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;


	vector<RectangleShape> enemies;
	RectangleShape enemy;



	void initVariables();
	void initWindow();
	void initEnemies();
	void initFonts();
	void initTexts();

};



void Game::initVariables()
{
	window = nullptr;

	//Game logic
	endGame = false;
	points = 0;
	health = 30;
	enemySpawnTimerMax = 10.f;
	enemySpawnTimer = enemySpawnTimerMax;
	maxEnemies = 5;
	mouseHeld = false;


}
void Game::initWindow()
{
	videomode.height = 600;
	videomode.width = 800;
	window = new RenderWindow(videomode, "Bloku cina", Style::Titlebar | Style::Close);
	window->setFramerateLimit(60);
}

void Game::initEnemies()
{
	enemy.setPosition(10.f, 10.f);
	enemy.setSize(Vector2f(100.f, 100.f));
	enemy.setFillColor(Color::Cyan);

}

void Game::initFonts()
{
	if (!font.loadFromFile("dosis.light.ttf"))
	{
		cout << "ERROR::GAME::INITFONTS::Nevar ieladet fontu" << "\n";
	}
}

void Game::initTexts()
{
	Uitext.setFont(font);
	Uitext.setCharacterSize(30);
	Uitext.setFillColor(Color::White);
	Uitext.setString("None");
}



//Konstruktori un dekonstruktori
Game:: Game()
{
	
	initVariables();
	initWindow();
	initFonts();
	initTexts();
	initEnemies();
}
Game::~Game()
{
	delete window;
}

const bool Game::running() const
{
	return window->isOpen();
}

const bool Game::getendGame() const
{
	return endGame;
}

void Game::spawnEnemy()
{

	this->enemy.setPosition(
		static_cast<float>(rand()%static_cast<int>(window->getSize().x - enemy.getSize().x)),
		0.f

	);
	//randomize ienaidniekus no 0 lidz 5 
	int type = rand() % 5;
	switch (type)
	{
	case 0:
		enemy.setSize(Vector2f(20.f, 20.f));
		enemy.setFillColor(Color::Magenta);
		break;
	case 1:
		enemy.setSize(Vector2f(30.f, 30.f));
		enemy.setFillColor(Color::Blue);
		break;
	case 2:
		enemy.setSize(Vector2f(50.f, 50.f));
		enemy.setFillColor(Color::Cyan);
		break;
	case 3:
		enemy.setSize(Vector2f(70.f, 70.f));
		enemy.setFillColor(Color::Red);
		break;
	case 4:
		enemy.setSize(Vector2f(100.f, 100.f));
		enemy.setFillColor(Color::Green);
		break;
		
	}

	// uztaisa enemijus
	enemies.push_back(enemy);

	
}

void Game::pollEvents()
{
	while (window->pollEvent(ev))
	{
		switch (this->ev.type)
		{
		case Event::Closed:
			window->close();
			break;
		case Event::KeyPressed:
			if (ev.key.code == Keyboard::Escape)
				window->close();
			break;
		}
	}
}

void Game::updateMousePos()
{
	MousePosWindow = Mouse::getPosition(*window);
	MousePosView = window->mapPixelToCoords(MousePosWindow);
}

void Game::updateText()
{
	stringstream ss;
	ss << "Punkti: " << points << "\n"
		<< "Dzīvības: " << health << "\n";

	Uitext.setString(ss.str());


}


void Game::update()
{
	pollEvents();
	if (endGame == false)
	{

		//Relative to the window
		updateMousePos();

		updateText();

		updateEnemies();
		
	}
	if(health <= 0)
	{
		endGame = true;
	}

}

void Game::updateEnemies()
{
	

	if (enemies.size() < maxEnemies)
	{
		if (enemySpawnTimer >= enemySpawnTimerMax)
		{
			//veido enemy un reseto timeru
			spawnEnemy();
			enemySpawnTimer = 0.f;
		}
		else
			enemySpawnTimer += 1.f;
	}
	
	for (int i = 0; i < enemies.size(); i++)
	{
		
		enemies[i].move(0.f, 3.f);

		if (enemies[i].getPosition().y > window->getSize().y && enemies[i].getFillColor() == Color::Green)
		{
			enemies.erase(enemies.begin() + i);
			health -= 4;	
		}
		else if (enemies[i].getPosition().y > window->getSize().y && enemies[i].getFillColor() == (Color::Red))
		{
			enemies.erase(enemies.begin() + i);
			health -= 3;
		}
		else if (enemies[i].getPosition().y > window->getSize().y && enemies[i].getFillColor() == (Color::Cyan))
		{
			enemies.erase(enemies.begin() + i);
			health -= 2;
		}
		else if (enemies[i].getPosition().y > window->getSize().y)
		{
			enemies.erase(enemies.begin() + i);
			health -= 1;
		}
	}
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (mouseHeld == false)
		{
			mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < enemies.size() && deleted == false; i++)
			{
				if (enemies[i].getGlobalBounds().contains(MousePosView))
				{
					
					if(enemies[i].getFillColor() == Color::Magenta)
						points += 10;
					else if(enemies[i].getFillColor() == Color::Blue)
						points += 7;
					else if (enemies[i].getFillColor() == Color::Cyan)
						points += 5;
					else if (enemies[i].getFillColor() == Color::Red)
						points += 3;
					else if (enemies[i].getFillColor() == Color::Green)
						points += 1;
					//bloka izdzeshana
					deleted = true;
					enemies.erase(enemies.begin() + i);
				}
			}
		}
	}
	else
	{
		mouseHeld = false;
	}

}
	
	
void Game::rendertext(RenderTarget& target)
{
	target.draw(Uitext);
}

void Game::renderEnemies(RenderTarget& target)
{
	
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::render()
{

	window->clear();
	
	renderEnemies(*window);
	
	rendertext(*window);

	window->display();
}


int main()
{
	Music music;
	music.openFromFile("Kiss.ogg");
	music.setVolume(10);
	music.play();
	srand(static_cast<unsigned>(time(NULL)));

	Game game;
	
	while (game.running() && !game.getendGame())
	{
		
		game.update();
		
		game.render();
	}



	
	return 0;
}
