#ifndef BITMAPMANAGER_H
#define BITMAPMANAGER_H
//#include "Sepia_Params.h"
#include <chrono>
#include <iomanip>
#include <vector>
#include <thread>
#include<windows.h>
#include"EasyBMP.h"

//typedef int(_stdcall* ASM_PROC)(DWORD, unsigned char*, DWORD, DWORD); //imageData,
typedef int(__cdecl* ASM_PROC)(int width, RGBApixel** image, int, int); //(imageData, bytesPerRow, linesToProcess)

//typedef int(__cdecl* CPP_FUNC)(unsigned char*, DWORD, DWORD, DWORD);

typedef int(__cdecl* CPP_FUNC)(RGBApixel** image, int, int, int width);

class BitmapManager {
public:
	//BitmapFileHeader fileHeader;
	//BitmapInfoHeader infoHeader;

	bool isFileLoaded;
	//unsigned char* imageData;
	HINSTANCE hinstLibCPP; //obiekt przechowujacy biblioteke z ktorej przechwycimy procedure
	HINSTANCE hinstLibAsm;
	CPP_FUNC handleToCPPSepia; //typ wskaznika na funkcje z argumentami wyzej
	ASM_PROC handleToAsmSepia;

	BMP* image;
	long long runtime;

	//void loadBMP(const char* filename);
	static BMP* loadBMP(const char* filename);
	void setDuration(long long duration);
	long long getDuration();

	BitmapManager(const char* filename);
	void printImageOnConsole();
	void printPixels(int amount);
	void runSepia(int threadNumber, bool choice);
	~BitmapManager();
};

#endif
