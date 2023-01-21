#include "pch.h"
#include "mydll.h"
#include"framework.h"
#include<stdio.h>
#include<Windows.h>
#include<fstream>
#include"EasyBMP.h"
#include"EasyBMP_VariousBMPutilities.h"
#include"EasyBMP.cpp"
#include"EasyBMP_BMP.h"
#include"EasyBMP_DataStructures.h"

void Sepia (RGBApixel** input, int kolumny, int skok, int wiersze) {
	
		int W = 20; //wspó³czynnik wype³nienia


		for (int i = kolumny; i < skok; i++)
		{
			for (int j = 0; j < wiersze; j++)
			{
				//szaroœæ:
				char temp = input[i][j].Green * 0.587 + input[i][j].Blue * 0.114 + input[i][j].Red * 0.299;// obliczanie wartoœci szarego piksela

				RGBApixel nowy = input[i][j];
				nowy.Alpha = input[i][j].Alpha;
				nowy.Blue = temp;
				nowy.Green = temp;
				nowy.Red = temp;
				//sepia:
				nowy.Green += W;
				if (nowy.Green <= (W - 1))
					nowy.Green = 255;
				nowy.Red += (2 * W);
				if (nowy.Red <= ((2 * W) - 1))
					nowy.Red = 255;

				input[i][j] = nowy;
			}

		}
		return;
	
}

