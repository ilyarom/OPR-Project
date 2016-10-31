// Animation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <concrt.h>
#include <cmath>

using namespace sf;
using namespace std;


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;
const size_t RECTANGLE_AMOUNT = 8;
const Color STARTER_COLOR = Color(32, 227, 149);
const int RECTANGLE_STARTER_WIDTH = 30;
const int RECTANGLE_STARTER_HEIGHT = 30;
const int STARTER_PADDING_X = 20;
const int STARTER_PADDING_Y = 30;
const int PADDING_BETWEEN_RECTANGLES = 5;
const int MODES = 6;
const Vector2f CHECKPOINT_1(500, 0);
const Vector2f CHECKPOINT_2(0, 500);
const Vector2f CHECKPOINT_3(800, 0);
const Vector2f CHECKPOINT_4(0, 650);
const float ASYNCH_STARTER_POS_X = CHECKPOINT_3.x;
const float ASYNCH_END_POS_Y = CHECKPOINT_4.y - ((RECTANGLE_STARTER_HEIGHT + PADDING_BETWEEN_RECTANGLES) * (RECTANGLE_AMOUNT / 2) / 2);
const float SPEED = 0.5;

struct Blocks
{
	RectangleShape rectangle[RECTANGLE_AMOUNT];
	Vector2f size;
	Color color;
};


void InitAsynchEffect(Blocks &blocks, float &x, float &y, vector <float> asynchStarterPosY, size_t i)
{
	if (x > ((ASYNCH_STARTER_POS_X - (STARTER_PADDING_X + blocks.rectangle[i].getOrigin().x)) / 2))
	{
		// До середины
		if (!(blocks.rectangle[i].getPosition().y <= ASYNCH_END_POS_Y + 1 && blocks.rectangle[i].getPosition().y >= ASYNCH_END_POS_Y - 1))
		{
			if (i < 4)
			{
				y = blocks.rectangle[i].getPosition().y + (SPEED * 0.2 * abs(int(i - (RECTANGLE_AMOUNT / 2))));
			}
			else
			{
				y = blocks.rectangle[i].getPosition().y - (SPEED * 0.2 * abs(int(i % (RECTANGLE_AMOUNT / 2))));
			}
		}
		else
		{
			y = ASYNCH_END_POS_Y;
		}

	}
	else
	{
		// После
		if (!(blocks.rectangle[i].getPosition().y <= asynchStarterPosY[i] + 1 && blocks.rectangle[i].getPosition().y >= asynchStarterPosY[i] - 1))
		{
			if (i < 4)
			{
				y = blocks.rectangle[i].getPosition().y - (SPEED * 0.2 * abs(int(i - (RECTANGLE_AMOUNT / 2))));
			}
			else
			{
				y = blocks.rectangle[i].getPosition().y + (SPEED * 0.2 * abs(int(i % (RECTANGLE_AMOUNT / 2))));
			}
		}
		else
		{
			y = asynchStarterPosY[i];
		}
	}
}

void LeedToEndPos(Blocks &blocks, size_t i, float &x)
{
	for (size_t j = i; j < RECTANGLE_AMOUNT; ++j)
	{
		x = blocks.rectangle[j].getPosition().x + SPEED;
		blocks.rectangle[j].setPosition(x, blocks.rectangle[j].getPosition().y);
	}
}
void Motion(Blocks &blocks, int &mode, float &changer, vector <float> &asynchStarterPosY)
{
	float x = 0;
	float y = 0;
	Color currentColor;
	for (int i = 0; i < RECTANGLE_AMOUNT; ++i)
	{
		if (mode == 0)
		{
			if (x == CHECKPOINT_1.x)
			{
				++mode;
				changer = 0;
				LeedToEndPos(blocks, i, x);
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x + SPEED;
				y = blocks.rectangle[i].getPosition().y;
				currentColor = Color(32, 64, int(x) % 255);
			}

		}
		if (mode == 1)
		{
			if (y == CHECKPOINT_2.y)
			{
				++mode;
				changer = 0;
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x;
				y = blocks.rectangle[i].getPosition().y + SPEED;
				blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH, RECTANGLE_STARTER_HEIGHT - changer);
				blocks.rectangle[i].setSize(blocks.size);
				currentColor = Color(32, 64, int(blocks.rectangle[0].getPosition().y) % 255);
			}

		}
		if (mode == 2)
		{
			if (x == CHECKPOINT_3.x)
			{
				++mode;
				changer = 0;
				LeedToEndPos(blocks, i, x);
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x + SPEED; // *0.1 * time;
				y = blocks.rectangle[i].getPosition().y;
				blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH - changer, RECTANGLE_STARTER_HEIGHT);
				blocks.rectangle[i].setSize(blocks.size);
				currentColor = Color(32, 64, int(x) % 255);
			}
		}
		if (mode == 3)
		{
			if (y == CHECKPOINT_4.y)
			{
				++mode;
				changer = 0;
				for (size_t j = 0; j < RECTANGLE_AMOUNT; ++j)
				{
					asynchStarterPosY[j] = blocks.rectangle[j].getPosition().y;
				}
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x;
				y = blocks.rectangle[i].getPosition().y + SPEED;
				currentColor = Color(32, 64, int(blocks.rectangle[0].getPosition().y) % 255);
			}
		}
		if (mode == 4)
		{
			if (x == STARTER_PADDING_X + blocks.rectangle[i].getOrigin().x)
			{
				++mode;
				changer = 0;
				LeedToEndPos(blocks, i, x);
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x - SPEED;
				InitAsynchEffect(blocks, x, y, asynchStarterPosY, i);
				blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH, RECTANGLE_STARTER_HEIGHT);
				blocks.rectangle[i].setSize(blocks.size);
				currentColor = Color(32, 64, int(x) % 255);

			}
		}
		if (mode == 5)
		{
			if (blocks.rectangle[0].getPosition().y == STARTER_PADDING_Y + blocks.rectangle[0].getOrigin().y)
			{
				mode = 0;
				changer = 0;
				blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH, RECTANGLE_STARTER_HEIGHT);
				for (size_t j = 0; j < RECTANGLE_AMOUNT; ++j)
				{
					blocks.rectangle[j].setSize(blocks.size);
					blocks.rectangle[j].setPosition(STARTER_PADDING_X + blocks.rectangle[j].getOrigin().x, (((RECTANGLE_STARTER_HEIGHT + PADDING_BETWEEN_RECTANGLES) * j) + STARTER_PADDING_Y + blocks.rectangle[j].getOrigin().y));
				}
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x;
				y = blocks.rectangle[i].getPosition().y - SPEED;
				blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH - changer, RECTANGLE_STARTER_HEIGHT);
				blocks.rectangle[i].setSize(blocks.size);
				currentColor = Color(32, 64, int(blocks.rectangle[0].getPosition().y) % 255);
			}
		}
		if (!(mode == 0 && changer == 0))
		{
			blocks.rectangle[i].setPosition(x, y);
		}
		blocks.rectangle[i].setFillColor(currentColor);
	}
}

void DrawFrame(Blocks &blocks, RenderWindow &window)
{
	for (int i = 0; i < RECTANGLE_AMOUNT; ++i)
	{
		window.draw(blocks.rectangle[i]);
	}
	window.display();
}

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Animation", Style::Close, settings);
	Blocks blocks;
	blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH, RECTANGLE_STARTER_HEIGHT);
	blocks.color = STARTER_COLOR;
	float changer = 0;
	int mode = 0;
	vector <float> asynchStarterPosY(RECTANGLE_AMOUNT);
	for (size_t i = 0; i < RECTANGLE_AMOUNT; ++i)
	{
		blocks.rectangle[i].setSize(blocks.size);
		blocks.rectangle[i].setFillColor(blocks.color);
		blocks.rectangle[i].setOrigin(RECTANGLE_STARTER_WIDTH / 2, RECTANGLE_STARTER_HEIGHT / 2);
		blocks.rectangle[i].setPosition(STARTER_PADDING_X + blocks.rectangle[i].getOrigin().x, (((RECTANGLE_STARTER_HEIGHT + PADDING_BETWEEN_RECTANGLES) * i) + STARTER_PADDING_Y + blocks.rectangle[i].getOrigin().y));
	}
	while (window.isOpen())
	{//дать прошедшее время в микросекундах
		window.clear(Color::White);
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		changer = changer + 0.001;
		Motion(blocks, mode, changer, asynchStarterPosY);
		DrawFrame(blocks, window);
	}
    return 0;
}

