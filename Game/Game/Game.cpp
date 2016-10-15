// Game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _USE_MATH_DEFINES

const int SCREEN_WIDTH = 1438;
const int SCREEN_HEIGHT = 715;
const int Y_POS_OF_CHARACTERS = 405;
const int X_POS_OF_HERO = 1200;

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Game", Style::Close, settings);
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("resources/textures/black-white-background.png"))
	{
		cout << "Failed to load file field.png" << "\n";
	}
	Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);//передаём в него объект Texture (текстуры)
	backgroundSprite.setPosition(0, 0); 
	window.clear(Color::White);
	// Персонаж
	sf::Texture heroTexture;
	if (!heroTexture.loadFromFile("resources/characters/starter.png"))
	{
		cout << "Failed to load file field.png" << "\n";
	}
	Sprite hero;
	hero.setTexture(heroTexture);//передаём в него объект Texture (текстуры)
	hero.setPosition(X_POS_OF_HERO, Y_POS_OF_CHARACTERS);
	// Враг
	sf::Texture enemyTexture;
	if (!enemyTexture.loadFromFile("resources/badcharacters/starter.png"))
	{
		cout << "Failed to load file field.png" << "\n";
	}
	Sprite enemy;
	enemy.setTexture(enemyTexture);//передаём в него объект Texture (текстуры)
	enemy.setPosition(0, Y_POS_OF_CHARACTERS);
	int n = 0;
	window.clear(Color::White);
	while(window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		window.draw(backgroundSprite);
		window.draw(enemy);
		window.draw(hero);
		window.display();
		n = n + 1;
		enemy.setPosition(n, Y_POS_OF_CHARACTERS);
	}
    return 0;
}

