#include <Windows.h>
#include <detours.h>
#include "cgamex86hack.h"

cgamex86hack* mainHack;

static HMODULE(_stdcall* TrueLoadLibrary)(LPCSTR dwLibaryName) = LoadLibrary;
HMODULE _stdcall DetourLoadLibrary(LPCSTR ModuleName)
{
	auto library = TrueLoadLibrary(ModuleName);
	if (mainHack != NULL)
	{
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
	}

	return library;
}

static HWND(__stdcall* TrueCreateWindowEx)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) = CreateWindowEx;
HWND __stdcall DetourCreateWindowEx(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	bool isMainWindow = false;
	if (lpWindowName != NULL)
		isMainWindow = strstr(lpWindowName, "Soldier of Fortune 2 : Double Helix") != NULL;

	auto result = TrueCreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	if (isMainWindow && mainHack != NULL && mainHack->isBorderless)
	{

		RECT rect;
		GetClientRect(result, &rect);
		auto width = rect.right - rect.left;
		auto height = rect.bottom - rect.top + 9;
		SetWindowLong(result, GWL_STYLE, WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		SetWindowPos(result, 0, 0, 0, width, height, 0);
	}


	return result;
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
			MODULEINFO moduleInfo;
			GetModuleInformation(GetCurrentProcess(), baseModule, &moduleInfo, sizeof(moduleInfo));
			if (moduleInfo.SizeOfImage != 8683520)
			{
				int result = MessageBox(NULL, "SOF2.exe has a different size than expected - it may be due to exe being different than the one the fix was designed for.", "Warning", MB_ICONWARNING | MB_OK);
			}
			mainHack = new cgamex86hack();
		}

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TrueLoadLibrary, DetourLoadLibrary);
		DetourAttach(&(PVOID&)TrueCreateWindowEx, DetourCreateWindowEx);
		DetourTransactionCommit();

	}
	else if (reason == DLL_PROCESS_DETACH) {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)TrueLoadLibrary, DetourLoadLibrary);
		DetourDetach(&(PVOID&)TrueCreateWindowEx, DetourCreateWindowEx);
		DetourTransactionCommit();
	}
	return TRUE;
}


