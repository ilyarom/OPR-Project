// Minigame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace sf;
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SQUARE_SIZE = 30;
const int OUTLINE_SIZE = 2;
const int NUM_OF_BOMBS = 5;

void CreateField(const int fieldSize, RectangleShape *field, const int squaresX, const int squaresY)
{
	int posX = (SCREEN_WIDTH / 2) - (squaresX * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
	int posY = (SCREEN_HEIGHT / 2) - (squaresY * (SQUARE_SIZE * OUTLINE_SIZE) / 2);

	// Отрисовываем поле

	for (int i = 0; i < fieldSize; ++i)
	{
		field[i].setOutlineThickness(OUTLINE_SIZE);
		field[i].setOutlineColor(Color::Black);
		field[i].setFillColor(Color::White);
		field[i].setSize(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
		field[i].setPosition(posX, posY);
		posX = posX + SQUARE_SIZE + OUTLINE_SIZE;
		if ((i + 1) % squaresX == 0)
		{
			posY = posY + SQUARE_SIZE + OUTLINE_SIZE;
			posX = posX - (SQUARE_SIZE * squaresX) - (OUTLINE_SIZE * squaresX);
		}
	}
}

void CreateFieldValues(const int fieldSize, vector <string> &matrix, const int squaresX, const int squaresY)
{
	int rnd;
	int nearBombs = 0;
	for (int i = 0; i < NUM_OF_BOMBS; ++i)
	{
		rnd = rand() % fieldSize;
		if (matrix[rnd] != '*')
		{
			matrix[rnd] = '*';
			if (rnd - 1 >= 0 && matrix[rnd - 1] != '*' && rnd % squaresX != 0)
			{
				nearBombs = atoi(matrix[rnd - 1].c_str());
				++nearBombs;
				matrix[rnd - 1] = to_string(nearBombs);
			}
			if (rnd + 1 < fieldSize && matrix[rnd + 1] != '*' && rnd % squaresX != squaresX - 1)
			{
				nearBombs = atoi(matrix[rnd + 1].c_str());
				++nearBombs;
				matrix[rnd + 1] = to_string(nearBombs);
			}
			if (rnd - squaresX >= 0 && matrix[rnd - squaresX] != '*')
			{
				nearBombs = atoi(matrix[rnd - squaresX].c_str());
				++nearBombs;
				matrix[rnd - squaresX] = to_string(nearBombs);
			}
			if (rnd + squaresX < fieldSize && matrix[rnd + squaresX] != '*')
			{
				nearBombs = atoi(matrix[rnd + squaresX].c_str());
				++nearBombs;
				matrix[rnd + squaresX] = to_string(nearBombs);
			}
			if (rnd - squaresX - 1 >= 0 && matrix[rnd - squaresX - 1] != '*' && rnd % squaresX != 0)
			{
				nearBombs = atoi(matrix[rnd - squaresX - 1].c_str());
				++nearBombs;
				matrix[rnd - squaresX - 1] = to_string(nearBombs);
			}
			if (rnd + squaresX - 1 < fieldSize && matrix[rnd + squaresX - 1] != '*'  && rnd % squaresX != 0)
			{
				nearBombs = atoi(matrix[rnd + squaresX - 1].c_str());
				++nearBombs;
				matrix[rnd + squaresX - 1] = to_string(nearBombs);
			}
			if (rnd - squaresX + 1 >= 0 && matrix[rnd - squaresX + 1] != '*' && rnd % squaresX != squaresX - 1)
			{
				nearBombs = atoi(matrix[rnd - squaresX + 1].c_str());
				++nearBombs;
				matrix[rnd - squaresX + 1] = to_string(nearBombs);
			}
			if (rnd + squaresX + 1 < fieldSize && matrix[rnd + squaresX + 1] != '*' && rnd % squaresX != squaresX - 1)
			{
				nearBombs = atoi(matrix[rnd + squaresX + 1].c_str());
				++nearBombs;
				matrix[rnd + squaresX + 1] = to_string(nearBombs);
			}
		}
		else if (matrix[rnd] == '*')
		{
			--i;
		}
	}

}

void CreateDigits(const int fieldSize, const int squaresX, const int squaresY, Font font, vector <string> &matrix, Text *values)
{
	int posX = (SCREEN_WIDTH / 2) - (squaresX * (SQUARE_SIZE + OUTLINE_SIZE) / 2) + 3;
	int posY = (SCREEN_HEIGHT / 2) - (squaresY * (SQUARE_SIZE * OUTLINE_SIZE) / 2);
	for (int i = 0; i < fieldSize; i++)
	{
		values[i].setFont(font);
		values[i].setCharacterSize(23);
		values[i].setFillColor(Color::Black);
		values[i].setString(matrix[i]);
		values[i].setPosition(posX, posY);
		posX = posX + SQUARE_SIZE + OUTLINE_SIZE;
		if ((i + 1) % squaresX == 0)
		{
			posY = posY + SQUARE_SIZE + OUTLINE_SIZE;
			posX = posX - (SQUARE_SIZE * squaresX) - (OUTLINE_SIZE * squaresX);
		}
	}
}

void ProcessGame(RenderWindow &window, const int squaresX, const int squaresY, const int fieldSize, vector <string> &matrix, Text *values, RectangleShape *field)
{
	window.clear(Color::White);
	bool isGameEnd = false;
	int checkedBombs = 0;
	bool *clickedNumbers = new bool[fieldSize];
	fill(clickedNumbers, clickedNumbers + fieldSize, 0);
	bool *flags = new bool[fieldSize];
	fill(flags, flags + fieldSize, 0);
	int posX = (SCREEN_WIDTH / 2) - (squaresX * (SQUARE_SIZE + OUTLINE_SIZE) / 2) + 3;
	int posY = (SCREEN_HEIGHT / 2) - (squaresY * (SQUARE_SIZE * OUTLINE_SIZE) / 2);
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed && !isGameEnd)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					for (int i = 0; i < fieldSize; i++)
					{
						if (event.mouseButton.x <= posX + SQUARE_SIZE && event.mouseButton.x >= posX && event.mouseButton.y <= posY + SQUARE_SIZE && event.mouseButton.y >= posY)
						{
							clickedNumbers[i] = true;
							if (matrix[i] == "*")
							{
								cout << "YOU LOSE" << "\n";
								window.draw(values[i]); // Преобразование багов в фичи :)
								isGameEnd = true;
							}
							break;
						}
						posX = posX + SQUARE_SIZE + OUTLINE_SIZE;
						if ((i + 1) % squaresX == 0)
						{
							posY = posY + SQUARE_SIZE + OUTLINE_SIZE;
							posX = posX - (SQUARE_SIZE * squaresX) - (OUTLINE_SIZE * squaresX);
						}
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					for (int i = 0; i < fieldSize; i++)
					{
						if (event.mouseButton.x <= posX + SQUARE_SIZE && event.mouseButton.x >= posX && event.mouseButton.y <= posY + SQUARE_SIZE && event.mouseButton.y >= posY)
						{
							if (flags[i])
							{
								flags[i] = false;
								values[i].setString(matrix[i]);
								clickedNumbers[i] = false;
								if (matrix[i] == "*")
								{
									checkedBombs--;
								}
							}
							else
							{
								flags[i] = true;
								if (matrix[i] == "*")
								{
									checkedBombs++;
									if (checkedBombs = NUM_OF_BOMBS)
									{
										cout << "YOU WIN" << "\n";
										window.draw(values[i]); // Преобразование багов в фичи :)
										isGameEnd = true;
									}
								}
								values[i].setString("P");
								clickedNumbers[i] = true;
							}
							break;
						}
						posX = posX + SQUARE_SIZE + OUTLINE_SIZE;
						if ((i + 1) % squaresX == 0)
						{
							posY = posY + SQUARE_SIZE + OUTLINE_SIZE;
							posX = posX - (SQUARE_SIZE * squaresX) - (OUTLINE_SIZE * squaresX);
						}
					}
				}
			}
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		for (int i = 0; i < fieldSize && !isGameEnd; ++i)
		{
			window.draw(field[i]);
			if (clickedNumbers[i])
			{
				window.draw(values[i]);
			}
		}
		window.display();
	}
}

int main()
{
	setlocale(LC_ALL, "rus");

	int squaresX;
    int squaresY;
	cout << "Введите значение клеток в ширину ";
	cin >> squaresX;
	cout << "\n" << "Введите значение клеток в длину ";
	cin >> squaresY;
    const int fieldSize = squaresX * squaresY;
	vector <string> matrix(fieldSize, "0");
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Game", Style::Close, settings);
	RectangleShape *field = new RectangleShape[fieldSize];
	Text *values = new Text[fieldSize];
	CreateField(fieldSize, field, squaresX, squaresY);
	CreateFieldValues(fieldSize, matrix, squaresX, squaresY);
	Font font;
	if (!font.loadFromFile("fonts/times.ttf"))
	{
		return EXIT_FAILURE;
	}
	CreateDigits(fieldSize, squaresX, squaresY, font, matrix, values);
	ProcessGame(window, squaresX, squaresY, fieldSize, matrix, values, field);
    return 0;
}

