#pragma once
#include <Windows.h>
#include <thread>
#include "HookFunctions.h"
#include "FovHack.h"

class cgamex86hack
{
public:
	cgamex86hack();
	//void CheckPresenceAndReinstall();
	bool ModuleWasPresent = false;
	HMODULE cGamex86Reference = NULL;
	FovHack* fovHack;
};

