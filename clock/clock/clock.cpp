// Clock.cpp: определяет точку входа для консольного приложения.
//
#define _USE_MATH_DEFINES
#include "stdafx.h"


////////////////////////////////////////////////////////////
// Headers:
// ctime for getting system time and
// cmath for sin and cos functions
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace sf;
////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
	// Define some variables and constants
	const int screenWidth = 800;
	const int screenHeight = 600;
	const float PI = 3.1415927;
	const int clockCircleSize = 250;
	const int clockCircleThickness = 2;
	int x, y;
	float angle = 0.0;
 	// Set multisampling level
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	// Create the window of the application
	RenderWindow window(VideoMode(screenWidth, screenHeight), "SFML Analog Clock", Style::Close, settings);

	// Define windowCenter which gets the center of the window here, right after creating window
	Vector2f windowCenter = Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	// Create a list for clock's dots
	CircleShape dot[60];

	// Create dots and place them to very right positions
	for (int i = 0; i<60; i++)
	{
		x = (clockCircleSize - 10) * cos(angle);
		y = (clockCircleSize - 10) * sin(angle);

		if (i % 5 == 0)
			dot[i] = CircleShape(3);
		else
			dot[i] = CircleShape(1);
		dot[i].setFillColor(Color::Black);
		dot[i].setOrigin(dot[i].getGlobalBounds().width / 2, dot[i].getGlobalBounds().height / 2);
		dot[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);

		angle = angle + ((2 * PI) / 60);
	}
	// Create outline of the clock
	CircleShape clockCircle(clockCircleSize);

	clockCircle.setPointCount(100);
	clockCircle.setOutlineThickness(clockCircleThickness);
	clockCircle.setOutlineColor(Color::Black);
	clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2, clockCircle.getGlobalBounds().height / 2);
	clockCircle.setPosition(window.getSize().x / 2 + clockCircleThickness, window.getSize().y / 2 + clockCircleThickness);

	// Crate another circle for center
	CircleShape centerCircle(10);

	centerCircle.setPointCount(100);
	centerCircle.setFillColor(Color::Red);
	centerCircle.setOrigin(centerCircle.getGlobalBounds().width / 2, centerCircle.getGlobalBounds().height / 2);
	centerCircle.setPosition(windowCenter);

	// Create hour, minute, and seconds hands
	RectangleShape hourHand(Vector2f(5, 180));
	RectangleShape minuteHand(Vector2f(3, 240));
	RectangleShape secondsHand(Vector2f(2, 250));

	hourHand.setFillColor(Color::Black);
	minuteHand.setFillColor(Color::Black);
	secondsHand.setFillColor(Color::Red);

	hourHand.setOrigin(hourHand.getGlobalBounds().width / 2, hourHand.getGlobalBounds().height - 25);
	minuteHand.setOrigin(minuteHand.getGlobalBounds().width / 2, minuteHand.getGlobalBounds().height - 25);
	secondsHand.setOrigin(secondsHand.getGlobalBounds().width / 2, secondsHand.getGlobalBounds().height - 25);

	hourHand.setPosition(windowCenter);
	minuteHand.setPosition(windowCenter);
	secondsHand.setPosition(windowCenter);

	// Create sound effect
	Music clockTick;
	if (!clockTick.openFromFile("resources/clock-1.wav"))
		return EXIT_FAILURE;
	clockTick.setLoop(true);
	clockTick.play();

	// Use a part of SFML logo as clock brand
	Texture clockBrand;
	if (!clockBrand.loadFromFile("resources/clock-brand.png"))
	{
		return EXIT_FAILURE;
	}

	Sprite clockBrandSprite;
	clockBrandSprite.setTexture(clockBrand);
	clockBrandSprite.setOrigin(clockBrandSprite.getTextureRect().left + clockBrandSprite.getTextureRect().width / 2.0f,
		clockBrandSprite.getTextureRect().top + clockBrandSprite.getTextureRect().height / 2.0f);

	clockBrandSprite.setPosition(window.getSize().x / 2, window.getSize().y - 100);


	// Create clock background
	Texture clockImage;
	if (!clockImage.loadFromFile("resources/clock-image.png"))
	{
		return EXIT_FAILURE;
	}

	clockCircle.setTexture(&clockImage);
	clockCircle.setTextureRect(IntRect(40, 0, 500, 500));

	while (window.isOpen())
	{
		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			// Window closed: exit
			if (event.type == Event::Closed)
				window.close();
		}

		// Get system time
		std::time_t currentTime = std::time(NULL);
		struct tm * ptm = nullptr;
		localtime_s(ptm, &currentTime);
		hourHand.setRotation(ptm->tm_hour * 30 + (ptm->tm_min / 2));
		minuteHand.setRotation(ptm->tm_min * 6 + (ptm->tm_sec / 12));
		secondsHand.setRotation(ptm->tm_sec * 6);

		// Clear the window
		window.clear(Color::White);

		// Draw all parts of clock
		window.draw(clockCircle);

		for (int i = 0; i<60; i++)
		{
			window.draw(dot[i]);
		}

		window.draw(clockBrandSprite);
		window.draw(hourHand);
		window.draw(minuteHand);
		window.draw(secondsHand);
		window.draw(centerCircle);

		// Display things on screen
		window.display();
	}

	return EXIT_SUCCESS;
}
