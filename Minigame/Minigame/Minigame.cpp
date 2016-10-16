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


struct Cells
{
	int squaresX;
	int squaresY;
};


struct Sapper
{
	Cells cells;
	int fieldSize;
	vector <RectangleShape> field;
	vector <Text> values;
	vector <bool> clickedNumbers;
	vector <string> matrix;
	vector <bool> flags;
	Font font;
};

void CreateField(Sapper &sapper)
{
	int posX = (SCREEN_WIDTH / 2) - (sapper.cells.squaresX * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
	int posY = (SCREEN_HEIGHT / 2) - (sapper.cells.squaresY * (SQUARE_SIZE + OUTLINE_SIZE) / 2);

	// Отрисовываем поле

	for (int i = 0; i < sapper.fieldSize; ++i)
	{
		sapper.field[i].setOutlineThickness(OUTLINE_SIZE);
		sapper.field[i].setOutlineColor(Color::Black);
		sapper.field[i].setFillColor(Color::White);
		sapper.field[i].setSize(Vector2f(SQUARE_SIZE, SQUARE_SIZE));
		sapper.field[i].setPosition(posX, posY);
		posX = posX + SQUARE_SIZE + OUTLINE_SIZE;
		if ((i + 1) % sapper.cells.squaresX == 0)
		{
			posY = posY + SQUARE_SIZE + OUTLINE_SIZE;
			posX = posX - (SQUARE_SIZE * sapper.cells.squaresX) - (OUTLINE_SIZE * sapper.cells.squaresX);
		}
	}
}

void CreateFieldValues(Sapper &sapper)
{
	int rnd;
	int nearBombs = 0;
	for (int i = 0; i < NUM_OF_BOMBS; ++i)
	{
		rnd = rand() % sapper.fieldSize;
		if (sapper.matrix[rnd] != '*')
		{
			sapper.matrix[rnd] = '*';
			if (rnd - 1 >= 0 && sapper.matrix[rnd - 1] != '*' && rnd % sapper.cells.squaresX != 0)
			{
				nearBombs = atoi(sapper.matrix[rnd - 1].c_str());
				++nearBombs;
				sapper.matrix[rnd - 1] = to_string(nearBombs);
			}
			if (rnd + 1 < sapper.fieldSize && sapper.matrix[rnd + 1] != '*' && rnd % sapper.cells.squaresX != sapper.cells.squaresX - 1)
			{
				nearBombs = atoi(sapper.matrix[rnd + 1].c_str());
				++nearBombs;
				sapper.matrix[rnd + 1] = to_string(nearBombs);
			}
			if (rnd - sapper.cells.squaresX >= 0 && sapper.matrix[rnd - sapper.cells.squaresX] != '*')
			{
				nearBombs = atoi(sapper.matrix[rnd - sapper.cells.squaresX].c_str());
				++nearBombs;
				sapper.matrix[rnd - sapper.cells.squaresX] = to_string(nearBombs);
			}
			if (rnd + sapper.cells.squaresX < sapper.fieldSize && sapper.matrix[rnd + sapper.cells.squaresX] != '*')
			{
				nearBombs = atoi(sapper.matrix[rnd + sapper.cells.squaresX].c_str());
				++nearBombs;
				sapper.matrix[rnd + sapper.cells.squaresX] = to_string(nearBombs);
			}
			if (rnd - sapper.cells.squaresX - 1 >= 0 && sapper.matrix[rnd - sapper.cells.squaresX - 1] != '*' && rnd % sapper.cells.squaresX != 0)
			{
				nearBombs = atoi(sapper.matrix[rnd - sapper.cells.squaresX - 1].c_str());
				++nearBombs;
				sapper.matrix[rnd - sapper.cells.squaresX - 1] = to_string(nearBombs);
			}
			if (rnd + sapper.cells.squaresX - 1 < sapper.fieldSize && sapper.matrix[rnd + sapper.cells.squaresX - 1] != '*'  && rnd % sapper.cells.squaresX != 0)
			{
				nearBombs = atoi(sapper.matrix[rnd + sapper.cells.squaresX - 1].c_str());
				++nearBombs;
				sapper.matrix[rnd + sapper.cells.squaresX - 1] = to_string(nearBombs);
			}
			if (rnd - sapper.cells.squaresX + 1 >= 0 && sapper.matrix[rnd - sapper.cells.squaresX + 1] != '*' && rnd % sapper.cells.squaresX != sapper.cells.squaresX - 1)
			{
				nearBombs = atoi(sapper.matrix[rnd - sapper.cells.squaresX + 1].c_str());
				++nearBombs;
				sapper.matrix[rnd - sapper.cells.squaresX + 1] = to_string(nearBombs);
			}
			if (rnd + sapper.cells.squaresX + 1 < sapper.fieldSize && sapper.matrix[rnd + sapper.cells.squaresX + 1] != '*' && rnd % sapper.cells.squaresX != sapper.cells.squaresX - 1)
			{
				nearBombs = atoi(sapper.matrix[rnd + sapper.cells.squaresX + 1].c_str());
				++nearBombs;
				sapper.matrix[rnd + sapper.cells.squaresX + 1] = to_string(nearBombs);
			}
		}
		else if (sapper.matrix[rnd] == '*')
		{
			--i;
		}
	}

}

void CreateDigits(Sapper &sapper)
{
	int posX = (SCREEN_WIDTH / 2) - (sapper.cells.squaresX * (SQUARE_SIZE + OUTLINE_SIZE) / 2) + 3;
	int posY = (SCREEN_HEIGHT / 2) - (sapper.cells.squaresY * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
	for (int i = 0; i < sapper.fieldSize; i++)
	{
		sapper.values[i].setFont(sapper.font);
		sapper.values[i].setCharacterSize(23);
		sapper.values[i].setFillColor(Color::Black);
		sapper.values[i].setString(sapper.matrix[i]);
		sapper.values[i].setPosition(posX, posY);
		posX = posX + SQUARE_SIZE + OUTLINE_SIZE;
		if ((i + 1) % sapper.cells.squaresX == 0)
		{
			posY = posY + SQUARE_SIZE + OUTLINE_SIZE;
			posX = posX - (SQUARE_SIZE * sapper.cells.squaresX) - (OUTLINE_SIZE * sapper.cells.squaresX);
		}
	}
}

void CheckNearNulls(Sapper &sapper, int i)
{
	int j = i;
	if ((j - 1 >= 0) && (sapper.matrix[j] == '0') && (j % sapper.cells.squaresX != 0))
	{
		sapper.clickedNumbers[j-1] = true;
		--j;
		if (sapper.matrix[j] == "0")
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j + 1 < sapper.fieldSize) && (sapper.matrix[j] == '0') && (j % sapper.cells.squaresX != sapper.cells.squaresX - 1))
	{
		sapper.clickedNumbers[j+1] = true;
		++j;
		if (sapper.matrix[j] == "0")
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j - sapper.cells.squaresX >= 0) && (sapper.matrix[j] == '0'))
	{
		sapper.clickedNumbers[j - sapper.cells.squaresX] = true;
		j = j - sapper.cells.squaresX;
		if (sapper.matrix[j] == "0")
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j + sapper.cells.squaresX < sapper.fieldSize) && (sapper.matrix[j + sapper.cells.squaresX] == '0'))
	{
		sapper.clickedNumbers[j + sapper.cells.squaresX] = true;
		j = j + sapper.cells.squaresX;
		if (sapper.matrix[j] == "0")
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j - sapper.cells.squaresX - 1 >= 0) && (sapper.matrix[j] == '0') && (j % sapper.cells.squaresX != 0))
	{
		sapper.clickedNumbers[j - sapper.cells.squaresX - 1] = true;
		++j;
		if (sapper.matrix[j] == "0")
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j + sapper.cells.squaresX - 1 < sapper.fieldSize) && (sapper.matrix[j] == '0')  && (j % sapper.cells.squaresX != 0))
	{
		sapper.clickedNumbers[j + sapper.cells.squaresX - 1] = true;
		j = j + sapper.cells.squaresX - 1;
		if (sapper.matrix[j] == "0")
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j - sapper.cells.squaresX + 1 >= 0) && (sapper.matrix[j] == '0') && (j % sapper.cells.squaresX != sapper.cells.squaresX - 1))
	{
		sapper.clickedNumbers[j - sapper.cells.squaresX + 1] = true;
		j = j - sapper.cells.squaresX + 1;
		if (sapper.matrix[j] == "0")
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j + sapper.cells.squaresX + 1 < sapper.fieldSize) && (sapper.matrix[j] == '0') && (j % sapper.cells.squaresX != sapper.cells.squaresX - 1))
	{
		sapper.clickedNumbers[j + sapper.cells.squaresX + 1] = true;
		j = j + sapper.cells.squaresX + 1;
		if (sapper.matrix[j] == "0")
		{
			CheckNearNulls(sapper, j);
		}
	}
}


void ProcessGame(RenderWindow &window, Sapper &sapper)
{
	window.clear(Color::White);
	bool isGameEnd = false;
	int checkedBombs = 0;
	int checkedCells = 0;
	while (window.isOpen())
	{
		Event event;
		int posX = (SCREEN_WIDTH / 2) - (sapper.cells.squaresX * (SQUARE_SIZE + OUTLINE_SIZE) / 2) + 3;
		int posY = (SCREEN_HEIGHT / 2) - (sapper.cells.squaresY * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed && !isGameEnd)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					for (int i = 0; i < sapper.fieldSize; i++)
					{
						if (event.mouseButton.x <= posX + SQUARE_SIZE && event.mouseButton.x >= posX && event.mouseButton.y <= posY + SQUARE_SIZE && event.mouseButton.y >= posY)
						{
							sapper.clickedNumbers[i] = true;
							if (sapper.matrix[i] == "0")
							{
								CheckNearNulls(sapper, i);
							} 
							if (sapper.matrix[i] == "*")
							{
								cout << "YOU LOSE" << "\n";
								window.draw(sapper.values[i]); // Преобразование багов в фичи :)
								isGameEnd = true;
							}
							break;
						}
						posX = posX + SQUARE_SIZE + OUTLINE_SIZE;
						if ((i + 1) % sapper.cells.squaresX == 0)
						{
							posY = posY + SQUARE_SIZE + OUTLINE_SIZE;
							posX = posX - (SQUARE_SIZE * sapper.cells.squaresX) - (OUTLINE_SIZE * sapper.cells.squaresX);
						}
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					for (int i = 0; i < sapper.fieldSize; i++)
					{
						if ((event.mouseButton.x <= posX + SQUARE_SIZE && event.mouseButton.x >= posX && event.mouseButton.y <= posY + SQUARE_SIZE && event.mouseButton.y >= posY) && (!sapper.clickedNumbers[i]))
						{
							if (sapper.flags[i])
							{
								sapper.flags[i] = false;
								sapper.values[i].setString(sapper.matrix[i]);
								--checkedCells;
								if (sapper.matrix[i] == "*")
								{
									checkedBombs--;
								}
							}
							else
							{
								sapper.flags[i] = true;
								++checkedCells;
								if (sapper.matrix[i] == "*")
								{
									checkedBombs++;
									if ((checkedBombs == NUM_OF_BOMBS) && (checkedBombs == checkedCells))
									{
										cout << "YOU WIN" << "\n";
										sapper.values[i].setString("P");
										window.draw(sapper.values[i]); // Преобразование багов в фичи :)
										isGameEnd = true;
									}
								}
								sapper.values[i].setString("P");
							}
							break;
						}
						posX = posX + SQUARE_SIZE + OUTLINE_SIZE;
						if ((i + 1) % sapper.cells.squaresX == 0)
						{
							posY = posY + SQUARE_SIZE + OUTLINE_SIZE;
							posX = posX - (SQUARE_SIZE * sapper.cells.squaresX) - (OUTLINE_SIZE * sapper.cells.squaresX);
						}
					}
				}
			}
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		for (int i = 0; i < sapper.fieldSize && !isGameEnd; ++i)
		{
			window.draw(sapper.field[i]);
			if (sapper.clickedNumbers[i] || sapper.flags[i])
			{
				window.draw(sapper.values[i]);
			}
		}
		window.display();
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	Sapper sapper;
	cout << "Введите значение клеток в ширину ";
	cin >> sapper.cells.squaresX;
	cout << "\n" << "Введите значение клеток в длину ";
	cin >> sapper.cells.squaresY;
    sapper.fieldSize = sapper.cells.squaresX * sapper.cells.squaresY;
	sapper.field.resize(sapper.fieldSize);
	sapper.values.resize(sapper.fieldSize);
	sapper.clickedNumbers.resize(sapper.fieldSize);
	sapper.flags.resize(sapper.fieldSize);
	sapper.matrix.resize(sapper.fieldSize);
	for (int i = 0; i < sapper.fieldSize; ++i)
	{
		sapper.clickedNumbers[i] = false;
		sapper.flags[i] = false;
		sapper.matrix[i] = "0";
	}
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Game", Style::Close, settings);
	CreateField(sapper);
	CreateFieldValues(sapper);
	if (!sapper.font.loadFromFile("fonts/times.ttf"))
	{
		return EXIT_FAILURE;
	}
	CreateDigits(sapper);
	ProcessGame(window, sapper);
    return 0;
}

