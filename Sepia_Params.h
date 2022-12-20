#ifndef SEPIA_PARAMS_H
#define SEPIA_PARAMS_H

#include<windows.h>
#include <iostream>
using namespace std;
#pragma pack(2)

#pragma once
typedef unsigned char byte;
typedef unsigned int int32;
typedef short int16;
typedef unsigned char  ebmpBYTE;

typedef struct RGBApixel {
    ebmpBYTE Blue;
    ebmpBYTE Green;
    ebmpBYTE Red;
    ebmpBYTE Alpha;
} RGBApixel;

struct Image {
    byte* pixels;
    int32 width;
    int32 height;
    int32 bytesPerPixel;
    RGBApixel** Pixels;

    ~Image() {
        free(pixels);
    }
};



//another one
/*
struct SepiaParams {
    uint8_t* Input;         // tablica wejœciowa z wartoœciami pikseli
    uint8_t* Output;        // tablica wyjœciowa z wartoœciami pikseli
    uint32_t StartXCoord;   // pozycja startowa na tablicy wej. na osi X
    uint32_t StartYCoord;   // pozycja startowa na tablicy wej. na osi Y
    uint32_t AreaWidth;     // szerokoœæ czêœci obrazu do przetworzenia
    uint32_t AreaHeight;    // wysokoœæ czêœci obrazu do przetworzenia
    uint8_t  BytesPerPixel; // liczba bajtów na kolor piksela (3 lub 4)
    uint32_t FullWidth;     // szerokoœæ ca³ego obrazu
    uint32_t FullHeight;    // wysokoœæ ca³ego obrazu
    uint32_t Factor;		// wpsó³czynnik sepii
};*/

#endif

