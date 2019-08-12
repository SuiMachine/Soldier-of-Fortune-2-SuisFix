#include "cgamex86hack.h"

DWORD __stdcall CheckPresenceAndReinstall(LPVOID arg)
{
	cgamex86hack* ths = (cgamex86hack*)arg;
	Sleep(20000);

	while (true)
	{
		if (ths->cGamex86Reference == NULL)
		{
			ths->cGamex86Reference = GetModuleHandle("cgamex86.dll");
			ths->ModuleWasPresent = false;
		}
		else
		{
			if (!ths->ModuleWasPresent)
			{
				HMODULE baseModule = GetModuleHandle(NULL);
				int width = *(int*)((intptr_t)baseModule + 0x148094);
				int height = *(int*)((intptr_t)baseModule + 0x148098);

				ths->fovHack->UpdateFOV(width, height, ths->DesiredFOV);
				ths->fovHack->OverrideMemory();
				ths->ModuleWasPresent = true;
			}
			else
			{
				//This is probably slow. Needs replacing.
				char path[1];
				DWORD moduleName = GetModuleFileName(ths->cGamex86Reference, path, 1);
				if (moduleName == 0x0)
					ths->cGamex86Reference = NULL;
			}
		}
		Sleep(100);
	}

}


cgamex86hack::cgamex86hack()
{
	HMODULE baseModule = GetModuleHandle(NULL);
	UnprotectModule(baseModule);

	//Initialize hacks
	CIniReader reader = CIniReader("");
	this->DesiredFOV = reader.ReadInteger("MAIN", "FOV", 80);
	fovHack = new FovHack();

	CreateThread(NULL, NULL, CheckPresenceAndReinstall, this, 0, NULL);
}

