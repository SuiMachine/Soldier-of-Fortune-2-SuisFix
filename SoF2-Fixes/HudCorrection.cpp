#include "HudCorrection.h"
#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include <set>

DWORD returnCorrectHudConfig;
float ScaleMultiplier = 1.5f;
char** ConfigContent;
void __fastcall PerformHudCorrectionAmmoCountersHighLevel()
{
	char * foundHudRight = strstr(*ConfigContent, "hud_rightback");
	char * foundHudLeft = strstr(*ConfigContent, "hud_leftback");

	if (foundHudRight != NULL || foundHudLeft != NULL)
	{
		auto str_Config = (std::string)(*ConfigContent);

		std::string newConfig = "";


		std::string token;
		std::stringstream iss(str_Config);

		std::regex reg_isValidLine("hud_leftback|hud_rightback");
		std::regex removeProportional("(<image.+[hud_leftback]|[hud_rightback].+)(proportional)(.+>|>)");
		std::regex findScale("(.+scale )([+-]?([0-9]*[.])?[0-9]+)(.+)");


		while (getline(iss, token))
		{
			if (std::regex_search(token, reg_isValidLine))
			{
				token = std::regex_replace(token, removeProportional, "$1$3");
				std::smatch res;
				std::regex_search(token, res, findScale, std::regex_constants::match_default);
				if (res.length() != 0)
				{
					std::string scaleAsStr = res[2];
					auto scale = atof(scaleAsStr.c_str());
					scale *= ScaleMultiplier;
					token = std::regex_replace(token, findScale, "$1 " + std::to_string(scale) + "$4");
				}

				Sleep(0);
			}
			newConfig += token + "\n";
		}
		


		int originalLenght = strlen(*ConfigContent);
		if (newConfig.length() > originalLenght)
			MessageBox(NULL, "Error", "Original config lenght was shorter then edited one", MB_ICONERROR | MB_OK);
		else
		{
			memcpy(*ConfigContent, newConfig.c_str(), newConfig.length());
			memset(*ConfigContent + newConfig.length(), ' ', originalLenght - newConfig.length());
		}
	}
}
 
void __declspec(naked) CorrectHudAmmoCountersConfig()
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

	PerformHudCorrectionAmmoCountersHighLevel();

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
	
	HookInsideFunction((intptr_t)mod + 0x22E71, CorrectHudAmmoCountersConfig, &returnCorrectHudConfig, 0x7);
	if (this->CustomHudScale != 0)
		ScaleMultiplier = this->CustomHudScale;
	else
		ScaleMultiplier =  1.5 * (SizeY / 720.0);
}
