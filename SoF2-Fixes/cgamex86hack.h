#pragma once
#include <Windows.h>
#include <thread>
#include "HookFunctions.h"
#include "FovHack.h"
#include "../externals//inireader/IniReader.h"


class cgamex86hack
{
public:
	cgamex86hack();
	void overrideGamex86Content();
	//void CheckPresenceAndReinstall();
	bool ModuleWasPresent = false;
	HMODULE cGamex86Reference = NULL;
	FovHack* fovHack;
	int DesiredFOV = 80;
};

