// JumpHeight.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.
//
// TODO: Fix all warnings on high warning level (/W4, -Wall -Wextra).
// TODO: Rename variables and split to several functions,
// see also https://ps-group.github.io/sfml/coding_conventions.html
// TODO: fix negative height values, fix heigh values higher than max height.

int AskHeight()
{
	int MaxHeight;
	printf("MaxHeight: ");
	if (0 == scanf("%d", &MaxHeight))
	{
		printf("\n" "expected floating-point number" "\n");
		exit(1);
	}
	if (MaxHeight < 0)
	{
		printf("\n" "expected floating-point number" "\n");
		exit(1);
	}
	return MaxHeight;
}

float CalculateMaxHeightTime(int MaxHeight)
{
	const float G = 9.8f;
	float MaxHeightTime = sqrt(MaxHeight * 2 / G);
	printf("MaxHeightTime=%f\n", MaxHeightTime);
	return MaxHeightTime;
}

int CalculateJump(float MaxHeightTime)
{
	const float G = 9.8f;
	float Half = 0.5;
	bool isObjFall = false;
	for (float CurrentTime = 0; CurrentTime < MaxHeightTime * 2; CurrentTime += 0.1f)
	{
		if (CurrentTime > MaxHeightTime && !isObjFall)
		{
			isObjFall = true;
			float StartSpeed = G * MaxHeightTime;
			float Height = StartSpeed * MaxHeightTime - Half * G * MaxHeightTime * MaxHeightTime;
			printf("CurrentTime=%f, Height=%f\n", MaxHeightTime, Height);
		}
		float StartSpeed = G * MaxHeightTime;
		float Height = StartSpeed * CurrentTime - Half * G * CurrentTime * CurrentTime;
		printf("CurrentTime=%f, Height=%f\n", CurrentTime, Height);
	};
	float StartSpeed = G * MaxHeightTime;
	float Height = StartSpeed * (MaxHeightTime * 2) - Half * G * (MaxHeightTime * 2) * (MaxHeightTime * 2);
	printf("CurrentTime=%f, Height=%f\n", MaxHeightTime * 2, Height);
	return 0;
}

int main(int, char *[])
{
	int MaxHeight = AskHeight();
	float MaxHeightTime = CalculateMaxHeightTime(MaxHeight);
	CalculateJump(MaxHeightTime);
	system("pause");
    return 0;
}

