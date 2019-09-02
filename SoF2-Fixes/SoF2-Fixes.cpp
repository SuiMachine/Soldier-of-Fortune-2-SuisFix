#include <Windows.h>
#include <detours.h>
#include "cgamex86hack.h"

cgamex86hack* mainHack;

static HMODULE(_stdcall* TrueLoadLibrary)(LPCSTR dwLibaryName) = LoadLibrary;
HMODULE _stdcall DetourLoadLibrary(LPCSTR ModuleName)
{
	auto library = TrueLoadLibrary(ModuleName);
	auto ModuleAsString = (std::string)ModuleName;
	std::transform(ModuleAsString.begin(), ModuleAsString.end(), ModuleAsString.begin(), [](unsigned char c) { return std::tolower(c); });
	if (ModuleAsString._Equal("cgamex86.dll"))
	{
		mainHack->overrideGamex86Content();
	}
	else if (ModuleAsString._Equal("menusx86.dll"))
	{
		mainHack->overrideMenusx86Content();
	}

	return library;
}





BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		HMODULE baseModule = GetModuleHandle(NULL);
		char baseModuleName[MAX_PATH];
		GetModuleFileName(baseModule, baseModuleName, sizeof(baseModuleName));
		int indexOfLastPathNode = StrEndsWith(baseModuleName, sizeof(baseModuleName), '\\') + 1;
		char exeName[MAX_PATH];
		strcpy_s(exeName, baseModuleName + indexOfLastPathNode);
		StrToLower(exeName, sizeof(exeName));
		
		if (std::strstr((const char*)&exeName, "sof2.exe"))
		{
			mainHack = new cgamex86hack();
		}

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TrueLoadLibrary, DetourLoadLibrary);
		DetourTransactionCommit();

	}
	else if (reason == DLL_PROCESS_DETACH) {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)TrueLoadLibrary, DetourLoadLibrary);
		DetourTransactionCommit();
	}
	return TRUE;
}


