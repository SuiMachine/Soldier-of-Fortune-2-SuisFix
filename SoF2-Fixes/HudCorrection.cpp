#include "HudCorrection.h"
#include <string>

DWORD returnCorrectHudConfig;
float ScaleMultiplier = 1.5f;
char** ConfigContent;
void __fastcall PerformHudCorrectionHighLevel()
{
	char * foundHudRight = strstr(*ConfigContent, "hud_rightback");
	char * foundHudLeft = strstr(*ConfigContent, "hud_leftback");

	if (foundHudRight != NULL || foundHudLeft != NULL)
	{
		char* hudPos = foundHudRight != NULL ? foundHudRight : foundHudLeft;
		char* Proportional = strstr(hudPos, "proportional");

		if (Proportional != NULL)
		{
			char* scale = strstr(Proportional - 6, " ") + 1;

			if (Proportional != NULL)
			{
				strcpy_s(Proportional, 12, "           ");
			}

			if (scale != NULL)
			{
				double scaleD = atof(scale);
				if (scaleD > 0)
				{
					scaleD = scaleD * ScaleMultiplier;
					auto temp = std::to_string(scaleD);
					strcpy_s(scale, temp.length() + 1, temp.c_str());
				}
				Sleep(0);
			}
		}
	}
}
 
void __declspec(naked) CorrectHudConfig()
{
	__asm
	{
		mov [esi + 0x4], eax
		mov eax, [esp + 0x7C]
		push eax
		push ecx
		push edx
		push esi
		mov[ConfigContent],esi
	}

	PerformHudCorrectionHighLevel();

	__asm
	{
		pop esi
		pop edx
		pop ecx
		pop eax
		jmp returnCorrectHudConfig
	}
}

HudCorrection::HudCorrection(float HudScale)
{
	this->CustomHudScale = HudScale;
}

void HudCorrection::InstallDetour(float SizeX, float SizeY)
{
	//Actually, don't use SizeX
	HMODULE mod = GetModuleHandle("Menusx86.dll");
	UnprotectModule(mod);
	

	HookInsideFunction((DWORD)mod + 0x22E71, CorrectHudConfig, &returnCorrectHudConfig, 0x7);
	if (this->CustomHudScale != 0)
		ScaleMultiplier = this->CustomHudScale;
	else
		ScaleMultiplier =  1.5 * (SizeY / 720.0);
}
