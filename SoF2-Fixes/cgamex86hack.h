#pragma once
#include <Windows.h>
#include <thread>
#include <psapi.h>
#include "HookFunctions.h"
#include "FovHack.h"
#include "../externals//inireader/IniReader.h"
#include "DisplayModesHack.h"


class cgamex86hack
{
public:
	cgamex86hack();
	void overrideGamex86Content();
	bool checkIfCorrectVersion();
	FovHack* fovHack;
	DisplayModesHack* displayModesHack;
	int DesiredFOV = 80;
	bool correctVersion = false;
};

