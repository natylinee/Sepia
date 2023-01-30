/* _______________________________________________________________________________________________________
 Temat projektu : sepia

 Algorytm polega na przekszta³ceniu obrazu na odcienie szaroœci przemna¿aj¹c odpowiednie subpiksele przez okreœlone wartoœci.
 Natêpnie nale¿y specyzowaæ wspó³czynnik sepii z zakresu od 20 do 40 i i dodaæ go do zielonego subpiksela oraz jego watoœæ
 pomno¿on¹ przez 2 do czerwonego subpiksela, niebieski bez zmian.

 Data oddania projektu : 31.01.2023 semestr 5 rok akademicki  2022/2023 Natalia Mendera
 _______________________________________________________________________________________________________
*/
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

void Sepia (RGBApixel** input, int columns, int range, int rows) {
	
	int W = 20; //wspó³czynnik wype³nienia


	for (int i = columns; i < range; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			//szaroœæ:
			char temp = input[i][j].Green * 0.587 + input[i][j].Blue * 0.114 + input[i][j].Red * 0.299;// obliczanie wartoœci szarego piksela

			RGBApixel newPixel = input[i][j];
			newPixel.Alpha = input[i][j].Alpha;
			newPixel.Blue = temp;
			newPixel.Green = temp;
			newPixel.Red = temp;
			//sepia:
			newPixel.Green += W;
			if (newPixel.Green <= (W - 1))
				newPixel.Green = 255;
			newPixel.Red += (2 * W);
			if (newPixel.Red <= ((2 * W) - 1))
				newPixel.Red = 255;

			input[i][j] = newPixel;
		}

	}
	return;
	
}

