#include "cgamex86hack.h"

DWORD dwStyleOverride = WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;;
DWORD dwStyleDetourReturn;
__declspec(naked) void dwStyleDetour()
{
	__asm
	{
		push    ebx //nHeight
		push    ebp //nWidth
		push    edi //Y
		push    esi //X
		push    DS:[dwStyleOverride] //style
		jmp dwStyleDetourReturn
	}

}

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
		fovHack = new FovHack();
		displayModesHack = new DisplayModesHack(reader.ReadInteger("MAIN", "Width", 0), reader.ReadInteger("MAIN", "Height", 0));

		if (reader.ReadBoolean("MAIN", "Borderless", 0))
			HookInsideFunction(0x100B91FA, dwStyleDetour, &dwStyleDetourReturn, 5);
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

