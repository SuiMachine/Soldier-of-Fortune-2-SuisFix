#pragma once
#include "HookFunctions.h"

class HudCorrection
{
public:
	HudCorrection(float HudScale);
	void InstallDetour(float SizeX, float SizeY);
private:
	float CustomHudScale = 0.5f;
};

