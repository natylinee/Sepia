#include <windows.h> 
#include <stdio.h> 
#include <iostream>
#include <thread>
//#include "Sepia_Params.h"
#include "Sepia_Projekt_Manager.h"

using namespace std;
int main()
{
    BitmapManager MyManager("nogi.bmp");
    //MyManager.printImageOnConsole();
    //MyManager.printPixels(12);
    auto start = std::chrono::high_resolution_clock::now();
    MyManager.runSepia(1, true);
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> durationBig = finish - start;
    MyManager.writeImage("nogiSEPIA.bmp");
    cout << "Loading big size image took: " << durationBig.count() << " s" << endl;

    /*MyManager.loadBMP("bmp_maly.bmp");
    start = chrono::high_resolution_clock::now();
    MyManager.runSepia(1, true);
    finish = chrono::high_resolution_clock::now();
    chrono::duration<double> durationSmall = finish - start;

    cout << "Loading big size image took: " << durationBig.count() << " s" << endl;
    cout << "Loading small size image took: " << durationSmall.count() << " s" << endl;
    */
   // cout << endl;
   // MyManager.printPixels(12);
    //MyManager.runSepia(1, true);

    cout << endl << "------------------------------------" << endl;

}