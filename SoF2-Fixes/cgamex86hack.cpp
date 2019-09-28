#include "cgamex86hack.h"
#pragma warning(disable: 6387)


cgamex86hack::cgamex86hack()
{
	this->correctVersion = this->checkIfCorrectVersion();
	if (correctVersion)
	{
		HMODULE baseModule = GetModuleHandle(NULL);
		UnprotectModule(baseModule);

		//Initialize hacks
		CIniReader reader = CIniReader("");
		this->DesiredFOV = reader.ReadInteger("MAIN", "FOV", 80);
		this->isBorderless = reader.ReadBoolean("MAIN", "Borderless", false);
		fovHack = new FovHack();
		displayModesHack = new DisplayModesHack(reader.ReadInteger("MAIN", "Width", 0), reader.ReadInteger("MAIN", "Height", 0));
		if (reader.ReadBoolean("HudCorrection", "Enable", true))
		{
			hudCorrection = new HudCorrection(reader.ReadFloat("HudCorrection", "CustomScale", 0.0f));
		}
	}
	else
	{
		MessageBox(NULL, "Incorrect game version found. Currently supported version is 1.2 Gold (GOG release). No changes were applied.", "Error", MB_OK | MB_ICONEXCLAMATION);
	}

}

void cgamex86hack::overrideGamex86Content()
{
	if (this->correctVersion)
	{
		HMODULE baseModule = GetModuleHandle(NULL);

		int width = *(int*)((intptr_t)baseModule + 0x148094);
		int height = *(int*)((intptr_t)baseModule + 0x148098);

		this->fovHack->UpdateFOV(width, height, this->DesiredFOV);
		this->fovHack->OverrideMemory();
	}
}

void cgamex86hack::overrideMenusx86Content()
{
	if (this->correctVersion && this->hudCorrection != NULL)
	{
		HMODULE baseModule = GetModuleHandle(NULL);

		int width = *(int*)((intptr_t)baseModule + 0x148094);
		int height = *(int*)((intptr_t)baseModule + 0x148098);

		this->hudCorrection->InstallDetour(width, height);
	}
}

bool cgamex86hack::checkIfCorrectVersion()
{
	HMODULE baseModule = GetModuleHandle(NULL);
	MODULEINFO baseModuleInfo;
	K32GetModuleInformation(GetCurrentProcess(), baseModule, &baseModuleInfo, sizeof(MODULEINFO));

	if (baseModuleInfo.SizeOfImage != 8683520)
		return false;

	char path[MAX_PATH];
	GetModuleFileName(baseModule, path, MAX_PATH);
	*strrchr(path, '\\') = '\0';
	strcat_s(path, "\\cgamex86.dll");
	WIN32_FILE_ATTRIBUTE_DATA cGamexInfo;
	GetFileAttributesEx(path, GetFileExInfoStandard, &cGamexInfo);
	if (cGamexInfo.nFileSizeLow != 802816)
		return false;

	return true;
}

