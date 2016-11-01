// Clock.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

using namespace sf;
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int CLOCK_CIRCLE_SIZE = 250;
const int CLOCK_CIRCLE_THICKNESS = 2;
const int DOTS_QUANTITY = 60;
const int DIGIT_QUANTITY = 12;
const int NUMBER_PADDING = 45;
const int DOT_PADDING = 10;
const Vector2f HOUR_HAND_SIZE = Vector2f(5, 180);
const Vector2f MINUTE_HAND_SIZE = Vector2f(3, 240);
const Vector2f SECONDS_HAND_SIZE = Vector2f(2, 250);

struct Foundation
{
	CircleShape clockCircle;
	CircleShape centerCircle;
};

struct Hands
{
	RectangleShape hourHand;
	RectangleShape minuteHand;
	RectangleShape secondsHand;
};

struct Values
{
	CircleShape dot[DOTS_QUANTITY];
	Text digit[DIGIT_QUANTITY];
};

struct Clocks
{
	Foundation foundation;
	Hands hands;
	Values values;
	Music clockTick;
};

void CreateDots(RenderWindow &window, Clocks &clocks)
{
	float angle = 0.0;
	for (int i = 0; i < DOTS_QUANTITY; i++)
	{
		int x = (CLOCK_CIRCLE_SIZE - DOT_PADDING) * cos(angle);
		int y = (CLOCK_CIRCLE_SIZE - DOT_PADDING) * sin(angle);

		if (i % 5 == 0)
			clocks.values.dot[i] = CircleShape(3);
		else
			clocks.values.dot[i] = CircleShape(1);
		clocks.values.dot[i].setFillColor(Color::Black);
		clocks.values.dot[i].setOrigin(clocks.values.dot[i].getGlobalBounds().width / 2, clocks.values.dot[i].getGlobalBounds().height / 2);
		clocks.values.dot[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);

		angle = angle + ((2 * M_PI) / DOTS_QUANTITY);
	}
}

void CreateClockOutline(RenderWindow &window, Clocks &clocks)
{
	clocks.foundation.clockCircle.setRadius(250);
	clocks.foundation.clockCircle.setPointCount(100);
	clocks.foundation.clockCircle.setOutlineThickness(CLOCK_CIRCLE_THICKNESS);
	clocks.foundation.clockCircle.setOutlineColor(Color::Black);
	clocks.foundation.clockCircle.setOrigin(clocks.foundation.clockCircle.getGlobalBounds().width / 2, clocks.foundation.clockCircle.getGlobalBounds().height / 2);
	clocks.foundation.clockCircle.setPosition(window.getSize().x / 2 + CLOCK_CIRCLE_THICKNESS
		, window.getSize().y / 2 + CLOCK_CIRCLE_THICKNESS);
	clocks.foundation.clockCircle.setTextureRect(IntRect(40, 0, 500, 500));
}

void CreateCenterCircle(RenderWindow &window, Clocks &clocks, const Vector2f windowCenter)
{
	clocks.foundation.centerCircle.setRadius(10);
	clocks.foundation.centerCircle.setPointCount(100);
	clocks.foundation.centerCircle.setFillColor(Color::Red);
	clocks.foundation.centerCircle.setOrigin(clocks.foundation.centerCircle.getGlobalBounds().width / 2, clocks.foundation.centerCircle.getGlobalBounds().height / 2);
	clocks.foundation.centerCircle.setPosition(windowCenter);
}

void CreateHands(RenderWindow &window, Clocks &clocks, const Vector2f windowCenter)
{
    clocks.hands.hourHand.setSize(HOUR_HAND_SIZE);
	clocks.hands.minuteHand.setSize(MINUTE_HAND_SIZE);
	clocks.hands.secondsHand.setSize(SECONDS_HAND_SIZE);

	clocks.hands.hourHand.setFillColor(Color::Black);
	clocks.hands.minuteHand.setFillColor(Color::Black);
	clocks.hands.secondsHand.setFillColor(Color::Red);

	clocks.hands.hourHand.setOrigin(clocks.hands.hourHand.getGlobalBounds().width / 2, clocks.hands.hourHand.getGlobalBounds().height - 25);
	clocks.hands.minuteHand.setOrigin(clocks.hands.minuteHand.getGlobalBounds().width / 2, clocks.hands.minuteHand.getGlobalBounds().height - 25);
	clocks.hands.secondsHand.setOrigin(clocks.hands.secondsHand.getGlobalBounds().width / 2, clocks.hands.secondsHand.getGlobalBounds().height - 25);

	clocks.hands.hourHand.setPosition(windowCenter);
	clocks.hands.minuteHand.setPosition(windowCenter);
	clocks.hands.secondsHand.setPosition(windowCenter);

}

void CreateDigits(RenderWindow &window, Clocks &clocks, Font &font)
{
	float angle = 0.0;
	for (int i = 0; i < DIGIT_QUANTITY; i++)
	{
		int x = (CLOCK_CIRCLE_SIZE - NUMBER_PADDING) * cos(angle - (M_PI / 3));
		int y = (CLOCK_CIRCLE_SIZE - NUMBER_PADDING) * sin(angle - (M_PI / 3));
		clocks.values.digit[i].setFont(font);
		clocks.values.digit[i].setCharacterSize(40);
		clocks.values.digit[i].setFillColor(Color::Black);
		clocks.values.digit[i].setString(to_string(i + 1));
		clocks.values.digit[i].setOrigin(clocks.values.digit[i].getGlobalBounds().width / 1.8, clocks.values.digit[i].getGlobalBounds().height);
		clocks.values.digit[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);
		angle = angle + ((2 * M_PI) / DIGIT_QUANTITY);
	}
}

void HandleEvents(RenderWindow &window)
{
	Event event;
	while (window.pollEvent(event))
	{
		// Window closed: exit
		if (event.type == Event::Closed)
			window.close();
	}
}


void DetermineTime(Clocks &clocks)
{
	std::time_t currentTime = std::time(NULL);
	struct tm * ptm = localtime(&currentTime);
	clocks.hands.hourHand.setRotation(ptm->tm_hour * 30 + (ptm->tm_min / 2));
	clocks.hands.minuteHand.setRotation(ptm->tm_min * 6 + (ptm->tm_sec / 12));
	clocks.hands.secondsHand.setRotation(ptm->tm_sec * 6);
}

void SetClockTick(Clocks &clocks)
{
	clocks.clockTick.setLoop(true);
	clocks.clockTick.play();
}

void DrawElements(RenderWindow &window, Clocks &clocks)
{
	window.clear(Color::White);
	window.draw(clocks.foundation.clockCircle);
	for (int i = 0; i < DOTS_QUANTITY; ++i)
	{
		window.draw(clocks.values.dot[i]);
     	if (i < DIGIT_QUANTITY)
		{
			window.draw(clocks.values.digit[i]);
		}
	}

	window.draw(clocks.hands.hourHand);
	window.draw(clocks.hands.minuteHand);
	window.draw(clocks.hands.secondsHand);
	window.draw(clocks.foundation.centerCircle);

	window.display();
}



void CreateClocks(RenderWindow &window, Clocks &clocks, const Vector2f windowCenter, Font &font)
{
	CreateDots(window, clocks);
	CreateClockOutline(window, clocks);
	CreateCenterCircle(window, clocks, windowCenter);
	CreateHands(window, clocks, windowCenter);
	CreateDigits(window, clocks, font);
	SetClockTick(clocks);
}

void DisplayClocks(RenderWindow &window, Clocks &clocks, const Vector2f windowCenter)
{
	while (window.isOpen())
	{
		HandleEvents(window);
		DetermineTime(clocks);
		DrawElements(window, clocks);
		
	}
}


int main()
{
	Clocks clocks;
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Analog Clock", Style::Close, settings);
	Vector2f windowCenter = Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	Font font;
	if (!font.loadFromFile("fonts/times.ttf"))
	{
		return EXIT_FAILURE;
	}
	if (!clocks.clockTick.openFromFile("resources/clock-1.wav"))
	{
		return EXIT_FAILURE;
	}
	CreateClocks(window, clocks, windowCenter, font);
	DisplayClocks(window, clocks, windowCenter);

	return EXIT_SUCCESS;
}
