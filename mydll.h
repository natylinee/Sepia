#pragma once
#include"EasyBMP_DataStructures.h"
#define EXPORT extern "C" __declspec(dllexport)

EXPORT void Sepia (RGBApixel**, int , int, int );