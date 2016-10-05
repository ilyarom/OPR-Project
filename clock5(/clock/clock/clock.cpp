// clock.cpp: ���������� ����� ����� ��� ����������� ����������.
//
#include "stdafx.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;

void InitCircle(CircleShape &circle, int x, int y)
{
	circle.setRadius(5);
	circle.setFillColor(Color::Red);
	circle.setOutlineThickness(5);
	circle.setPosition(x, y);
}

void DrawCircles(RenderWindow &window, vector<CircleShape> circles)
{
	for (auto circle : circles)
	{
		window.draw(circle);
	}
}

int main()
{
	// �������������
	RenderWindow window(VideoMode(800, 600), "SFML window");
	// �������� ���� (game loop)
	vector<CircleShape> circles;

	while (window.isOpen())
	{
		CircleShape circle;
		// ��������� �������
		Event event;
		while (window.pollEvent(event))
		{
			// �������� ����
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == Event::MouseButtonPressed)
			{
				cout << "This is coordinates of mouse click.\n";
				cout << event.mouseButton.x << ' ' << event.mouseButton.y << '\n';
				InitCircle(circle, event.mouseButton.x, event.mouseButton.y);
				circles.push_back(circle);
				if (circles.size() == 6)
				{
					circles.erase(circles.begin());
				}
			}
		}
		// ���������
		window.clear();
		DrawCircles(window, circles);
		// ���������� �������
		window.display();

	}
	return 0;
}

