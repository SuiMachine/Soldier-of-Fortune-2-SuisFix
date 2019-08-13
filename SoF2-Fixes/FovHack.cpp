#include "FovHack.h"
# define M_PI           3.14159265358979323846  /* pi */


struct PredefinedParsedQuake3Setting
{
	float v1;
	float v2;
	float parsedValueAsFloat;
	int parsedValue;
};

struct PredefinedQuake3Setting
{
	PredefinedParsedQuake3Setting* parse;
	char* cvar;
	char* attribute;
};

FovHack::FovHack()
{
}

void FovHack::UpdateFOV(int SizeX, int SizeY, int FOV)
{
	float newAspect = SizeX * 1.0f / SizeY;
	if (newAspect != this->aspectRatio)
	{
		double dHorizontalRadians = M_PI * FOV / 180;
		double dVerticalRadians = 2 * atan(tan(dHorizontalRadians / 2) * (3.0 / 4.0));
		//printf("Initial FOV calculation (4:3): %.3d (hor) %.3d (vert)\n", dHorizontalRadians, dVerticalRadians);
		dHorizontalRadians = 2 * atan(tan(dVerticalRadians / 2) * newAspect);
		//printf("Recalculated FOV (current asp): %.3d (hor) %.3d (vert)\n", dHorizontalRadians, dVerticalRadians);
		FOV = (int)round(dHorizontalRadians * (180.0 / M_PI));

		if (FOV > 179)
			FOV = 179;
		else if (FOV < 60)
			FOV = 60;

		this->FovValue = FOV;
		this->FovValueAsChar = new char[4];
		_itoa_s(FOV, this->FovValueAsChar, sizeof(this->FovValueAsChar), 10);

		this->aspectRatio = newAspect;

	}
}


void FovHack::OverrideMemory()
{
	HMODULE handle = GetModuleHandle("cgamex86.dll");
	UnprotectModule(handle);
	PredefinedQuake3Setting* setting = (PredefinedQuake3Setting*)((intptr_t)handle + 0xADF68);
	setting->parse->parsedValue = this->FovValue;
	setting->parse->parsedValueAsFloat = (float)this->FovValue;
	setting->attribute = this->FovValueAsChar;

	HMODULE baseModule = GetModuleHandle(NULL);
	*(int*)((intptr_t)baseModule + 0x3F895C) = this->FovValue;
	*(float*)((intptr_t)baseModule + 0x3F8958) = (float)this->FovValue;
	
	OutputDebugString("Overriding FOV\n");
}
