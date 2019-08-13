#include "DisplayModesHack.h"
#include <set>
#include <unordered_set>


struct DisplayMode
{
	char* DisplayModeName;

	int DisplayModeWidth;
	int DisplayModeHeight;
	float DisplayModeCustomAspect;


	DisplayMode()
	{

	};

	DisplayMode(int DisplayModeWidth, int DisplayModeHeight)
	{
		DisplayModeName = NULL;
		this->DisplayModeWidth = DisplayModeWidth;
		this->DisplayModeHeight = DisplayModeHeight;
		this->DisplayModeCustomAspect = 1.0f;
	}
	
	DisplayMode(int id, int DisplayModeWidth, int DisplayModeHeight, float DisplayModeCustomAspect)
	{
		DisplayModeName = new char[20];
		sprintf_s(DisplayModeName, 20, "Mode %d: %dx%d", id, DisplayModeWidth, DisplayModeHeight);
		this->DisplayModeWidth = DisplayModeWidth;
		this->DisplayModeHeight = DisplayModeHeight;
		this->DisplayModeCustomAspect = DisplayModeCustomAspect;
	}


	~DisplayMode()
	{
		delete DisplayModeName;
	}

	bool operator==(const DisplayMode& rhs) const
	{
		return DisplayModeWidth == rhs.DisplayModeWidth && DisplayModeHeight == rhs.DisplayModeHeight;
	}

};

DisplayMode* CustomDisplay;
DisplayMode* DisplayModeList;
static char __cdecl getDisplayModesDetour(int* width, int* height, float* aspectRatio, signed int displayModeID)
{
	DisplayMode* displayMode; // eax

	if (displayModeID < -1)
		return 0;
	if (displayModeID >= 8)
		return 0;

	if (CustomDisplay != NULL)
	{
		*width = CustomDisplay->DisplayModeWidth;
		*height = CustomDisplay->DisplayModeHeight;
		*aspectRatio = CustomDisplay->DisplayModeCustomAspect;
	}
	else if (displayModeID == -1)
	{
		//GOD IT'S AWFUL TO SOLVE POINTERS LIKE THIS
		*width = *(int*)(*((DWORD*)0x10799810) + 0x60);
		*height = *(int*)(*((DWORD*)0x1079980C) + 0x60);
		*aspectRatio = *(float*)(*((DWORD*)0x10799808) + 0x5C);;
	}
	else
	{
		DisplayModeList = (DisplayMode*)0x100EFF18;
		displayMode = &DisplayModeList[displayModeID];
		*width = displayMode->DisplayModeWidth;
		*height = displayMode->DisplayModeHeight;
		*aspectRatio = (double)(signed int)displayMode->DisplayModeWidth
			/ ((double)(signed int)displayMode->DisplayModeHeight
				* displayMode->DisplayModeCustomAspect);
	}
	return 1;
}

DisplayModesHack::DisplayModesHack(int width, int height)
{
	if (width != 0 && height != 0)
	{
		CustomDisplay = new DisplayMode(width, height);
		HookJmpTrampoline(0x1008FC40, getDisplayModesDetour, 5);
	}
}
