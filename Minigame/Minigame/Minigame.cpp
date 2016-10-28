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


struct Cells
{
	int squaresX;
	int squaresY;
};


struct Sapper
{
	Cells cells;
	int fieldSize;
	int numOfBombs;
	vector <RectangleShape> field;
	vector <Text> values;
	vector <bool> clickedNumbers;
	vector <bool> processedCells;
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
	for (int i = 0; i < sapper.numOfBombs; ++i)
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
	int posX = (SCREEN_WIDTH / 2) - (sapper.cells.squaresX * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
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
	sapper.processedCells[i] = true;
	int j = i;
	if ((j - 1 >= 0) && (sapper.matrix[j] == '0') && (j % sapper.cells.squaresX != 0))
	{
		sapper.clickedNumbers[j-1] = true;
		--j;
		if (sapper.matrix[j] == "0" && !sapper.processedCells[j])
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j + 1 < sapper.fieldSize) && (sapper.matrix[j] == '0') && (j % sapper.cells.squaresX != sapper.cells.squaresX - 1))
	{
		sapper.clickedNumbers[j+1] = true;
		++j;
		if (sapper.matrix[j] == "0" && !sapper.processedCells[j])
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j - sapper.cells.squaresX >= 0) && (sapper.matrix[j] == '0') && (j < sapper.fieldSize - sapper.cells.squaresX - 1))
	{
		sapper.clickedNumbers[j - sapper.cells.squaresX] = true;
		j = j - sapper.cells.squaresX;
		if (sapper.matrix[j] == "0" && !sapper.processedCells[j])
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j + sapper.cells.squaresX < sapper.fieldSize) && (sapper.matrix[j] == '0'))
	{
		sapper.clickedNumbers[j + sapper.cells.squaresX] = true;
		j = j + sapper.cells.squaresX;
		if (sapper.matrix[j] == "0" && !sapper.processedCells[j])
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j - sapper.cells.squaresX - 1 >= 0) && (sapper.matrix[j] == '0') && (j % sapper.cells.squaresX != 0))
	{
		sapper.clickedNumbers[j - sapper.cells.squaresX - 1] = true;
		j = j - sapper.cells.squaresX - 1;
		if (sapper.matrix[j] == "0" && !sapper.processedCells[j])
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j + sapper.cells.squaresX - 1 < sapper.fieldSize) && (sapper.matrix[j] == '0')  && (j % sapper.cells.squaresX != 0))
	{
		sapper.clickedNumbers[j + sapper.cells.squaresX - 1] = true;
		j = j + sapper.cells.squaresX - 1;
		if (sapper.matrix[j] == "0" && !sapper.processedCells[j])
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j - sapper.cells.squaresX + 1 >= 0) && (sapper.matrix[j] == '0') && (j % sapper.cells.squaresX != sapper.cells.squaresX - 1))
	{
		sapper.clickedNumbers[j - sapper.cells.squaresX + 1] = true;
		j = j - sapper.cells.squaresX + 1;
		if (sapper.matrix[j] == "0" && !sapper.processedCells[j])
		{
			CheckNearNulls(sapper, j);
		}
	}
	j = i;
	if ((j + sapper.cells.squaresX + 1 < sapper.fieldSize) && (sapper.matrix[j] == '0') && (j % sapper.cells.squaresX != sapper.cells.squaresX - 1))
	{
		sapper.clickedNumbers[j + sapper.cells.squaresX + 1] = true;
		j = j + sapper.cells.squaresX + 1;
		if (sapper.matrix[j] == "0" && !sapper.processedCells[j])
		{
			CheckNearNulls(sapper, j);
		}
	}
}

void Click(RenderWindow &window, Sapper &sapper, Event &event, bool &isGameEnd)
{
	int posX = (SCREEN_WIDTH / 2) - (sapper.cells.squaresX * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
	int posY = (SCREEN_HEIGHT / 2) - (sapper.cells.squaresY * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
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
				cout << "Игра проиграна. Попробуйте ещё раз\nНажмите R чтобы начать заново\n";
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

void SetFlag(RenderWindow &window, Sapper &sapper, Event &event, bool &isGameEnd, int &checkedBombs, int &checkedCells)
{
	int posX = (SCREEN_WIDTH / 2) - (sapper.cells.squaresX * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
	int posY = (SCREEN_HEIGHT / 2) - (sapper.cells.squaresY * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
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
					if ((checkedBombs == sapper.numOfBombs) && (checkedBombs == checkedCells))
					{
						cout << "Вы победили! Поздравляю!\nНажмите R чтобы сыграть ещё раз\n";
						sapper.values[i].setString("P");
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

void Restart(Sapper &sapper)
{
	for (int i = 0; i < sapper.fieldSize; ++i)
	{
		sapper.clickedNumbers[i] = false;
		sapper.flags[i] = false;
		sapper.matrix[i] = "0";
		sapper.processedCells[i] = false;
	}
	CreateField(sapper);
	CreateFieldValues(sapper);
	CreateDigits(sapper);
}

void ProcessGame(RenderWindow &window, Sapper &sapper)
{
	bool isGameEnd = false;
	bool stop = false;
	int checkedBombs = 0;
	int checkedCells = 0;
	while (window.isOpen())
	{
		window.clear(Color::White);
		Event event;
		int posX = (SCREEN_WIDTH / 2) - (sapper.cells.squaresX * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
		int posY = (SCREEN_HEIGHT / 2) - (sapper.cells.squaresY * (SQUARE_SIZE + OUTLINE_SIZE) / 2);
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed && !isGameEnd)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					Click(window, sapper, event, isGameEnd);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					SetFlag(window, sapper, event, isGameEnd, checkedBombs, checkedCells);
				}
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::R)
				{
					Restart(sapper);
					return;
				}
			}
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		for (int i = 0; i < sapper.fieldSize; ++i)
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
	cout << "\n" << "Введите количество бомб ";
	cin >> sapper.numOfBombs;
	if (sapper.numOfBombs > sapper.fieldSize)
	{
		cout << "Количество бомб не может быть больше суммарного размера поля\n";
		return EXIT_FAILURE;
	}
	if (sapper.numOfBombs <= 0 || sapper.cells.squaresX <= 0 || sapper.cells.squaresY <= 0 || sapper.numOfBombs > 15 || sapper.cells.squaresX > 15 || sapper.cells.squaresY > 15)
	{
		cout << "Указанные значения должны быть больше нуля и не больше 15\n";
		return EXIT_FAILURE;
	}
	sapper.field.resize(sapper.fieldSize);
	sapper.values.resize(sapper.fieldSize);
	sapper.clickedNumbers.resize(sapper.fieldSize);
	sapper.flags.resize(sapper.fieldSize);
	sapper.matrix.resize(sapper.fieldSize);
	sapper.processedCells.resize(sapper.fieldSize);
	for (int i = 0; i < sapper.fieldSize; ++i)
	{
		sapper.clickedNumbers[i] = false;
		sapper.flags[i] = false;
		sapper.matrix[i] = "0";
		sapper.processedCells[i] = false;
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
	while (true)
	{
		ProcessGame(window, sapper);
	}
    return 0;
}

