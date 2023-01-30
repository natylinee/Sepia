/* _______________________________________________________________________________________________________
 Temat projektu : sepia

 Algorytm polega na przekszta³ceniu obrazu na odcienie szaroœci przemna¿aj¹c odpowiednie subpiksele przez okreœlone wartoœci.
 Natêpnie nale¿y specyzowaæ wspó³czynnik sepii z zakresu od 20 do 40 i i dodaæ go do zielonego subpiksela oraz jego watoœæ
 pomno¿on¹ przez 2 do czerwonego subpiksela, niebieski bez zmian.

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
    //Za³aduj bitmapê
    //this->loadBMP(filename);
    this->runtime = 0;

    image = loadBMP(filename); //TO CHYBA NIE W TYM MIEJSCU


    //Wygeneruj uchwyty do DLL

    this->hinstLibAsm = LoadLibrary(TEXT("Dll_ASM.dll"));
    this->hinstLibCPP = LoadLibrary(TEXT("Dll_CPP.dll"));

    //Jeœli uchwyt wskazuje na bibliotekê, wygeneruj uchwyt do odpowiedniej procedury
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
    
    if (threadsNumber > image->TellWidth())  // za du¿a liczba w¹tków w porównaniu do wielkoœci obrazu
        bool error = true; // return -1;
    int range = image->TellWidth() / threadsNumber;// w zakresie znajduje siê liczba wierszy któr¹ przyjmuje dany w¹tek
    if (image->TellWidth() % threadsNumber)
        range += 1;//dodanie wiersza do ka¿dego w¹tku, gdy dzielene z reszt¹- w ostanim w¹tku bêdzie o jeden mniej wiersz

    int rows = image->TellHeight();

    //W zale¿noœci od wyboru u¿yj odpowiedniego uchwytu procedury
    if (useASM == true) {
        if (handleToAsmSepia == NULL) { // Jeœli nie uda³o siê za³adowaæ procedury
           cout << "Error: Have not found the proper function." << endl;
        }
        else {
            
            int counter = 1;
            std::vector<std::thread> threads;
            //utworzneie tablicy w¹tków, ka¿dy z nich zmienia okreœlony zakres tablicy. Indeks poczatkowy zmienianej komórki bitmapy jest w zmiennej i,
             //zmienna range okreœla ile wierszy bêdzie przetworzone orzez dany w¹tek
            //Dodatkowa zmienna counter pomaga w przesuniêciu indeksu do którego wykonuje swoj¹ prace dany w¹tek
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < image->TellWidth(); i+=range) //Utwórz tyle w¹tków, ile zosta³o podane
            {   
                int progress = range * counter; // zmienna przechowuje indeks koñcowej kolumny dla danego w¹tku

                //warunek w przypadku, gdzie progress przekroczy wielkoœæ tablicy- stanie siê tak kiedy po podzieleniu liczby kolumn na w¹tki bêdzie reszta
                if (progress >= image->TellWidth())
                    threads.push_back(std::thread(handleToAsmSepia, rows, image->GetTable(), i, image->TellWidth()));
                else
                    threads.push_back(std::thread(handleToAsmSepia, rows, image->GetTable(), i, progress));
                counter++;
            }
             
                
            for (auto& t : threads) 
            {//Zaczekaj, a¿ wszystkie w¹tki zakoñcz¹ pracê
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
                int progress = range * counter; // zmienna przechowuje indeks koñcowej kolumny dla danego w¹tku

                //warunek w przypadku, gdzie progress przekroczy wielkoœæ tablicy- stanie siê tak kiedy po podzieleniu liczby kolumn na w¹tki bêdzie reszta
                if (progress >= image->TellWidth())
                    // nie tu porównania takiego jak dla w¹tków dla asm, poniewa¿ to porównanie zosta³o zeralizowane w funkcji Sepia
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
    image->WriteToFile("sepia.bmp");// zapis zmienionego obrazu do pliku ustalonego z góry
    image->~BMP();
    // zapis zmienionego obrazu do pliku ustalonego z góry
}

BitmapManager::~BitmapManager()
{
    //Zwolnij uchwyty do bibliotek
    FreeLibrary(this->hinstLibAsm);
    FreeLibrary(this->hinstLibCPP);
    //delete[] this->imageData;
   // delete[] this->sepiaImageData;
}
