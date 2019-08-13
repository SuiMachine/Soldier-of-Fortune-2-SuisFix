#include "cgamex86hack.h"


cgamex86hack::cgamex86hack()
{
	HMODULE baseModule = GetModuleHandle(NULL);
	UnprotectModule(baseModule);

	//Initialize hacks
	CIniReader reader = CIniReader("");
	this->DesiredFOV = reader.ReadInteger("MAIN", "FOV", 80);
	fovHack = new FovHack();
}

void cgamex86hack::overrideGamex86Content()
{
	HMODULE baseModule = GetModuleHandle(NULL);
	int width = *(int*)((intptr_t)baseModule + 0x148094);
	int height = *(int*)((intptr_t)baseModule + 0x148098);

	this->fovHack->UpdateFOV(width, height, this->DesiredFOV);
	this->fovHack->OverrideMemory();
	this->ModuleWasPresent = true;
}

