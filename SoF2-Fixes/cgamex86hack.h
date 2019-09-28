#pragma once
#include <Windows.h>
#include <thread>
#include <psapi.h>
#include "HookFunctions.h"
#include "FovHack.h"
#include "../externals//inireader/IniReader.h"
#include "DisplayModesHack.h"
#include "HudCorrection.h"


class cgamex86hack
{
public:
	cgamex86hack();
	void overrideGamex86Content();
	void overrideMenusx86Content();
	bool checkIfCorrectVersion();
	FovHack* fovHack;
	DisplayModesHack* displayModesHack;
	HudCorrection* hudCorrection;
	int DesiredFOV = 80;
	bool correctVersion = false;
	bool isBorderless = false;
};

