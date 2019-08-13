#pragma once
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include "HookFunctions.h"

class FovHack
{
public:
	FovHack();
	void UpdateFOV(int SizeX, int SizeY, int FOV);
	void OverrideMemory();
private:
	int FovValue;
	char* FovValueAsChar;
	float aspectRatio = 0.0f;
};

