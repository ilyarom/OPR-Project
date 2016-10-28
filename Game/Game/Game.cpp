// Game.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#define _USE_MATH_DEFINES

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 810;
const int Y_POS_OF_HERO = 700;
const int X_POS_OF_HERO = 720;

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <concrt.h>
#include <cmath>


using namespace sf;
using namespace std;

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Game", Style::Close, settings);
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("resources/textures/top_view_texture.jpg"))
	{
		cout << "Failed to load file field.png" << "\n";
	}
	Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);//передаём в него объект Texture (текстуры)
	backgroundSprite.setPosition(0, 0); 
	window.clear(Color::White);
	// Персонаж
	sf::Texture heroTexture;
	if (!heroTexture.loadFromFile("resources/characters/herosprite.png"))
	{
		cout << "Failed to load file field.png" << "\n";
	}
	Sprite hero;
	hero.setTexture(heroTexture);//передаём в него объект Texture (текстуры)
	hero.setTextureRect(IntRect(10, 35, 60, 40));
	hero.setRotation(270);
	hero.setPosition(X_POS_OF_HERO - 20, Y_POS_OF_HERO + 70); // Из за поворота спрайта приходится корректировать координаты

	// Враг
	Texture enemyTexture;
	if (!enemyTexture.loadFromFile("resources/badcharacters/zombies.png"))
	{
		cout << "Failed to load file field.png" << "\n";
	}
	int numOfEnemies = 3;
	vector <Sprite> enemy(numOfEnemies);//передаём в него объект Texture (текстуры)
	vector <float> currentFrameX(numOfEnemies);
	vector <int> currentFrameY(numOfEnemies);
	for (size_t i = 0; i < numOfEnemies; ++i)
	{
		currentFrameX[i] = 0;
		currentFrameY[i] = 0;
		enemy[i].setPosition(0, SCREEN_HEIGHT);
		enemy[i].setTexture(enemyTexture);
		enemy[i].setTextureRect(IntRect(90 * int(currentFrameX[i]), currentFrameY[i], 90, 90));
		enemy[i].setRotation(180);
	}
	// Пули
	Texture bulletTexture;
	if (!bulletTexture.loadFromFile("resources/bullet.png"))
	{
		cout << "Failed to load file bullet.png" << "\n";
	}
	vector <Sprite> bullet;
	Sprite bulletSprite;
	int currentBullet = 0;

	// Время
	Clock clock;



	//Игра
	int n = 0;
	int i = 0;
	window.clear(Color::White);
	vector <bool> shoot;
	vector <double> distance;
	double dx;
	double dy;
	vector <double> shotX;
	vector <double> shotY;
	double heroRotation;
	bool Left;
	bool Up;
	int s = 0;
	Vector2f currPos;
	vector <int> spriteX(numOfEnemies, 0);
	vector <int> spriteY(numOfEnemies, 0);
	while(window.isOpen())
	{
		Event event;			
		int coordDepend;
		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800; //скорость игры
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseMoved)
			{
				heroRotation = (atan2(double(event.mouseMove.y) - Y_POS_OF_HERO, double(event.mouseMove.x) - X_POS_OF_HERO) * 180) / M_PI;
				hero.setOrigin(Vector2f(30, 20));
				hero.setRotation(heroRotation);

			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					shoot.insert(shoot.end(), true);
					bullet.insert(bullet.end(), bulletSprite);
					bullet[currentBullet].setTexture(bulletTexture);
					bullet[currentBullet].setPosition(X_POS_OF_HERO - 20, Y_POS_OF_HERO +70);
					bullet[currentBullet].setOrigin(Vector2f(0, 50)); //-40
					bullet[currentBullet].setRotation(heroRotation + 90);
					shotX.insert(shotX.end(), event.mouseButton.x); 
					shotY.insert(shotY.end(), event.mouseButton.y);// Должны быть коориднатами края
					double diffX = shotX[currentBullet] - X_POS_OF_HERO;
					double diffY = shotY[currentBullet] - Y_POS_OF_HERO;
					distance.insert(distance.end(), sqrt(diffX * diffX + diffY * diffY));
					while (shotX[currentBullet] >= 0 && shotX[currentBullet] < SCREEN_WIDTH && shotY[currentBullet] >= 0 && shotY[currentBullet] < SCREEN_HEIGHT)
					{
						shotX[currentBullet] += 0.1*time*(shotX[currentBullet] - bullet[currentBullet].getPosition().x);
						shotY[currentBullet] += 0.1*time*(shotY[currentBullet] - bullet[currentBullet].getPosition().y);
					}
					++currentBullet;
				}
			}
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		for (size_t i = 0; i < currentBullet; ++i)
		{
			if (shoot[i])
			{
				distance[i] = sqrt((shotX[i] - bullet[i].getPosition().x)*(shotX[i] - bullet[i].getPosition().x) + (shotY[i] - bullet[i].getPosition().y)*(shotY[i] - bullet[i].getPosition().y));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора
																																																																											  //cout << distance << "\n";
				if (distance[i] > 2) 
				{//этим условием убираем дергание во время конечной позиции спрайта

					bullet[i].setPosition(bullet[i].getPosition().x + 0.1*time*(shotX[i] - bullet[i].getPosition().x) / distance[i], bullet[i].getPosition().y + 0.1*time*(shotY[i] - bullet[i].getPosition().y) / distance[i]);//идем по иксу с помощью вектора нормали
				}
				else { shoot[i] = false; }
				for (size_t j = 0; j < numOfEnemies; ++j)
				{
					if (bullet[i].getPosition().x + 70 >= enemy[j].getPosition().x && bullet[i].getPosition().x <= enemy[j].getPosition().x && bullet[i].getPosition().y + 65 >= enemy[j].getPosition().y && bullet[i].getPosition().y <= enemy[j].getPosition().y)
					{
						enemy[j].setPosition((rand() % (SCREEN_WIDTH - 100) + 50), 0);
						bullet[i].setPosition(-50, -50);
						shoot[i] = false;
					}
				}//говорим что уже никуда не идем и выводим веселое сообщение в консоль
			}
		}
		for (size_t i = 0; i < numOfEnemies; ++i)
		{
			if (enemy[i].getPosition().y >= SCREEN_HEIGHT)
			{
				enemy[i].setPosition((rand() % (SCREEN_WIDTH - 100) + 50), 0);
				
			}
			currentFrameX[i] += 0.005*time;
			if (int(currentFrameX[i]) > 8)
			{
				++currentFrameY[i];
				currentFrameX[i] = 0;
			}
			if (int(currentFrameX[i]) == 2 && currentFrameY[i] == 3)
			{
				currentFrameX[i] = 0;
				currentFrameY[i] = 0;
			}
			enemy[i].setTextureRect(IntRect(90 * int(currentFrameX[i]), currentFrameY[i] * 90, 90, 90));
			enemy[i].setPosition(enemy[i].getPosition().x, enemy[i].getPosition().y + +0.04*time);
		}
		window.clear(Color::White);
		window.draw(backgroundSprite);
		window.draw(hero);
		for (size_t i = 0; i < currentBullet; ++i)
		{
			window.draw(bullet[i]);
		}
		for (size_t i = 0; i < numOfEnemies; ++i)
		{
			window.draw(enemy[i]);
		}
		/*if (i < displace && shoot)
		{
			bullet[currentBullet].setPosition(X_POS_OF_HERO - i * displaceX, Y_POS_OF_CHARACTERS - 150 + i * displaceY);
			cout << X_POS_OF_HERO - i * displaceX << " : " << Y_POS_OF_CHARACTERS - 150 + i * displaceY << "\n";
			window.draw(bullet[currentBullet]);
			++i;
		} */
		window.display();
		n = n + 1;
	}

    return 0;
}

void MoveBullet()//передаешь пулю в функцию
{
	//изменяешь координаты: x + BULLET_SPEED, y + BULLET_SPEED
	//каждый ГеймЛуп делаешь эту функцию
	//profit
}