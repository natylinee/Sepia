/* _______________________________________________________________________________________________________
 Temat projektu : sepia

 Algorytm polega na przekszta�ceniu obrazu na odcienie szaro�ci przemna�aj�c odpowiednie subpiksele przez okre�lone warto�ci.
 Nat�pnie nale�y specyzowa� wsp�czynnik sepii z zakresu od 20 do 40 i i doda� go do zielonego subpiksela oraz jego wato��
 pomno�on� przez 2 do czerwonego subpiksela, niebieski bez zmian.

 Data oddania projektu : 31.01.2023 semestr 5 rok akademicki 2022/2023 Natalia Mendera
 _______________________________________________________________________________________________________
*/

#include "Sepia_Projekt_Manager.h"
#include"EasyBMP.h"
//#include"EasyBMP_VariousBMPutilities.h"
#include"EasyBMP.cpp"
#include"EasyBMP_BMP.h"
#include"EasyBMP_DataStructures.h"
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

void BitmapManager::setDuration(long long duration)
{
    this->runtime = duration;
}

long long BitmapManager::getDuration()
{
    return this->runtime;
}



BitmapManager::BitmapManager(const char* filename)
{
    //Za�aduj bitmap�
    //this->loadBMP(filename);
    this->runtime = 0;

    image = loadBMP(filename); //TO CHYBA NIE W TYM MIEJSCU


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
            //utworzneie tablicy w�tk�w, ka�dy z nich zmienia okre�lony zakres tablicy. Indeks poczatkowy zmienianej kom�rki bitmapy jest w zmiennej i,
             //zmienna range okre�la ile wierszy b�dzie przetworzone orzez dany w�tek
            //Dodatkowa zmienna counter pomaga w przesuni�ciu indeksu do kt�rego wykonuje swoj� prace dany w�tek
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < image->TellWidth(); i+=range) //Utw�rz tyle w�tk�w, ile zosta�o podane
            {   
                int progress = range * counter; // zmienna przechowuje indeks ko�cowej kolumny dla danego w�tku

                //warunek w przypadku, gdzie progress przekroczy wielko�� tablicy- stanie si� tak kiedy po podzieleniu liczby kolumn na w�tki b�dzie reszta
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
            auto finish = chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = finish-start;
            this->setDuration(duration.count());
            
        }
    }
    else {
        if (handleToCPPSepia == NULL) {
            cout << "Error: Have not found the proper function." << endl;
        }
        else {
            
            int counter = 1;
            std::vector<std::thread> threads;
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < image->TellWidth(); i += range)
            {
                int progress = range * counter; // zmienna przechowuje indeks ko�cowej kolumny dla danego w�tku

                //warunek w przypadku, gdzie progress przekroczy wielko�� tablicy- stanie si� tak kiedy po podzieleniu liczby kolumn na w�tki b�dzie reszta
                if (progress >= image->TellWidth())
                    // nie tu por�wnania takiego jak dla w�tk�w dla asm, poniewa� to por�wnanie zosta�o zeralizowane w funkcji Sepia
                    threads.push_back(std::thread(handleToCPPSepia, image->GetTable(), i, image->TellWidth(), image->TellHeight()));
                else
                    threads.push_back(std::thread(handleToCPPSepia, image->GetTable(), i, progress, image->TellHeight()));
                counter++;
            }
            for (auto& t : threads)
            {
                t.join();
            }
            auto finish = chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = finish - start;
            this->setDuration(duration.count());
        }
        //writeImage("sepia.bmp");
    }
    image->WriteToFile("sepia.bmp");// zapis zmienionego obrazu do pliku ustalonego z g�ry
    image->~BMP();
    // zapis zmienionego obrazu do pliku ustalonego z g�ry
}

BitmapManager::~BitmapManager()
{
    //Zwolnij uchwyty do bibliotek
    FreeLibrary(this->hinstLibAsm);
    FreeLibrary(this->hinstLibCPP);
    //delete[] this->imageData;
   // delete[] this->sepiaImageData;
}
