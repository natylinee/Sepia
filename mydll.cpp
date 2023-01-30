/* _______________________________________________________________________________________________________
 Temat projektu : sepia

 Algorytm polega na przekszta�ceniu obrazu na odcienie szaro�ci przemna�aj�c odpowiednie subpiksele przez okre�lone warto�ci.
 Nat�pnie nale�y specyzowa� wsp�czynnik sepii z zakresu od 20 do 40 i i doda� go do zielonego subpiksela oraz jego wato��
 pomno�on� przez 2 do czerwonego subpiksela, niebieski bez zmian.

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
	
	int W = 20; //wsp�czynnik wype�nienia


	for (int i = columns; i < range; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			//szaro��:
			char temp = input[i][j].Green * 0.587 + input[i][j].Blue * 0.114 + input[i][j].Red * 0.299;// obliczanie warto�ci szarego piksela

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

