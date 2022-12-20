#ifndef BITMAPMANAGER_H
#define BITMAPMANAGER_H

#include "Sepia_Params.h"
#include <chrono>
#include <iomanip>
#include <vector>
#include <thread>

typedef int(_stdcall* ASM_PROC)(DWORD, unsigned char*, DWORD, DWORD); //imageData,
typedef int(__cdecl* CPP_FUNC)(unsigned char*, DWORD, DWORD, DWORD);

class BitmapManager {
public:
	//BitmapFileHeader fileHeader;
	//BitmapInfoHeader infoHeader;

	bool isFileLoaded;
	unsigned char* imageData;
	HINSTANCE hinstLibCPP; //obiekt przechowujacy biblioteke z ktorej przechwycimy procedure
	HINSTANCE hinstLibAsm;
	CPP_FUNC handleToCPPSepia; //typ wskaznika na funkcje z argumentami wyzej
	ASM_PROC handleToAsmSepia;

	Image* image;

	//void loadBMP(const char* filename);
	static Image* loadBMP(const char* filename);
	RGBApixel** GetTable();
	void writeImage(const char* filename);
	BitmapManager(const char* filename);
	void printImageOnConsole();
	void printPixels(int amount);
	void runSepia(int threadNumber, bool choice);
	~BitmapManager();
};

#endif
