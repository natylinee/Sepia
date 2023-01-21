#include "Sepia_Projekt_Manager.h"
#include"EasyBMP.h"
//#include"EasyBMP_VariousBMPutilities.h"
#include"EasyBMP.cpp"
#include"EasyBMP_BMP.h"
#include"EasyBMP_DataStructures.h"
//#include "Sepia_Params.h"
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0



BMP* BitmapManager::loadBMP(const char* filename)
{
            
            BMP* image = new BMP();
            image->ReadFromFile(filename);
            return image;
  
}

void BitmapManager::writeImage(const char* fileName)
{
    image->WriteToFile("sepia.bmp");// zapis zmienionego obrazu do pliku ustalonego z g�ry
}

BitmapManager::BitmapManager(const char* filename)
{
    //Za�aduj bitmap�
    //this->loadBMP(filename);
    image = loadBMP(filename);


    //Wygeneruj uchwyty do DLL

    this->hinstLibAsm = LoadLibrary(TEXT("Dll_ASM.dll"));
    this->hinstLibCPP = LoadLibrary(TEXT("Dll_CPP.dll"));

    //Je�li uchwyt wskazuje na bibliotek�, wygeneruj uchwyt do odpowiedniej procedury
    if (this->hinstLibAsm != NULL) {
        this->handleToAsmSepia = (ASM_PROC)GetProcAddress(this->hinstLibAsm, "Sepia");
        //ASM_PROC a = (ASM_PROC)GetProcAddress(hinstLibAsm, "Sepia");
    }
    if (this->hinstLibCPP != NULL) {
        this->handleToCPPSepia = (CPP_FUNC)GetProcAddress(this->hinstLibCPP, "Sepia");
    }

}



void BitmapManager::runSepia(int threadsNumber, bool useASM)
{
    
    if (threadsNumber > image->TellWidth())  // za du�a liczba w�tk�w w por�wnaniu do wielko�ci obrazu
        bool error = true; // return -1;
    int range = image->TellWidth() / threadsNumber;// w zakresie znajduje si� liczba wierszy kt�r� przyjmuje dany w�tek
    if (image->TellWidth() % threadsNumber)
        range += 1;//dodanie wiersza do ka�dego w�tku, gdy dzielene z reszt�- w ostanim w�tku b�dzie o jeden mniej wiersz

    int rows = image->TellHeight();
    //W zale�no�ci od wyboru u�yj odpowiedniego uchwytu procedury
    if (useASM == true) {
        if (handleToAsmSepia == NULL) { // Je�li nie uda�o si� za�adowa� procedury
           cout << "Error: Have not found the proper function." << endl;
        }
        else {
            int counter = 1;
            std::vector<std::thread> threads;
            for (int i = 0; i < image->TellWidth(); i+=range) //Utw�rz tyle w�tk�w, ile zosta�o podane
            {   
                int progress = range * counter; //indeks ko�cowej kolumny
                if (progress >= image->TellWidth())
                    threads.push_back(std::thread(handleToAsmSepia, rows, image->GetTable(), i, image->TellWidth()));
                else
                    threads.push_back(std::thread(handleToAsmSepia, rows, image->GetTable(), i, progress));
                counter++;
            }
             
                
            for (auto& t : threads) 
            {//Zaczekaj, a� wszystkie w�tki zako�cz� prac�
                t.join();
            }
            //koniec liczenia czasu tu dac
            
        }
    }
    else {
        if (handleToCPPSepia == NULL) {
            cout << "Error: Have not found the proper function." << endl;
        }
        else {
            int counter = 1;
            std::vector<std::thread> threads;
            for (int i = 0; i < image->TellWidth(); i += range)
            {
                int progress = range * counter;

                if (progress >= image->TellWidth())
                    threads.push_back(std::thread(handleToCPPSepia, image->GetTable(), i, image->TellWidth(), image->TellHeight()));
                else
                    threads.push_back(std::thread(handleToCPPSepia, image->GetTable(), i, progress, image->TellHeight()));
                counter++;
            }
            for (auto& t : threads)
            {
                t.join();
            }
            //koniec czasu
        }
        //writeImage("sepia.bmp");
    }
  
    // zapis zmienionego obrazu do pliku ustalonego z g�ry
}

BitmapManager::~BitmapManager()
{
    //Zwolnij uchwyty do bibliotek
    FreeLibrary(this->hinstLibAsm);
    FreeLibrary(this->hinstLibCPP);
    delete[] this->imageData;
   // delete[] this->sepiaImageData;
}
