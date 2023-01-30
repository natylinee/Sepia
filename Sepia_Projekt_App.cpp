/* _______________________________________________________________________________________________________
 Temat projektu : sepia

 Algorytm polega na przekształceniu obrazu na odcienie szarości przemnażając odpowiednie subpiksele przez określone wartości.
 Natępnie należy specyzować współczynnik sepii z zakresu od 20 do 40 i i dodać go do zielonego subpiksela oraz jego watość
 pomnożoną przez 2 do czerwonego subpiksela, niebieski bez zmian.

 Data oddania projektu : 31.01.2023 semestr 5 rok akademicki  2022/2023 Natalia Mendera
 _______________________________________________________________________________________________________
*/

#include <windows.h> 
#include <stdio.h> 
#include <iostream>
#include <thread>
#include "Sepia_Projekt_Manager.h"

using namespace std;
int main()
{
    //ASM -> true
    //C++ ->false
    BitmapManager MyManager("maly.bmp");
    MyManager.runSepia(1, true);
    long long duration = MyManager.getDuration();
    cout << "Loading image 1 took: " << duration << " ms" << endl;
    
    /*
    BitmapManager MyManager1("sredni.bmp");
    MyManager1.runSepia(2, false);
    long long duration1 = MyManager1.getDuration();
    cout << "Loading image 2 took: " << duration1 << " ms" << endl;
    
    BitmapManager MyManager2("sredni.bmp");
    MyManager2.runSepia(4, true);
    long long duration2 = MyManager2.getDuration();
    cout << "Loading image 4 took: " << duration2 << " ms" << endl;

    BitmapManager MyManager3("sredni.bmp");
    MyManager3.runSepia(8, true);
    long long duration3 = MyManager3.getDuration();
    cout << "Loading image 8 took: " << duration3 << " ms" << endl;

    BitmapManager MyManager4("sredni.bmp");
    MyManager4.runSepia(16, true);
    long long duration4 = MyManager4.getDuration();
    cout << "Loading image 16 took: " << duration4 << " ms" << endl;

    BitmapManager MyManager5("sredni.bmp");
    MyManager5.runSepia(32, true);
    long long duration5 = MyManager5.getDuration();
    cout << "Loading image 32 took: " << duration5 << " ms" << endl;

    BitmapManager MyManager6("sredni.bmp");
    MyManager6.runSepia(64, true);
    long long duration6 = MyManager6.getDuration();
    cout << "Loading image 64 took: " << duration6 << " ms" << endl;
    */


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