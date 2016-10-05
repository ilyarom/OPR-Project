// clock.cpp: определ€ет точку входа дл€ консольного приложени€.
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
	// »нициализаци€
	RenderWindow window(VideoMode(800, 600), "SFML window");
	// ќсновной цикл (game loop)
	vector<CircleShape> circles;

	while (window.isOpen())
	{
		CircleShape circle;
		// обработка событи€
		Event event;
		while (window.pollEvent(event))
		{
			// «акрытие окна
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
		// рендеринг
		window.clear();
		DrawCircles(window, circles);
		// обновление системы
		window.display();

	}
	return 0;
}

