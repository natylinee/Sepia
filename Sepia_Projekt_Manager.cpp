#include "Sepia_Projekt_Manager.h"

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

Image* BitmapManager::loadBMP(const char* filename)
{

            Image* image = new Image();

            FILE* imageFile = fopen(filename, "rb");
            int32 dataOffset;
            fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
            fread(&dataOffset, 4, 1, imageFile);
            fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
            fread(&image->width, 4, 1, imageFile);
            fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
            fread(&image->height, 4, 1, imageFile);
            int16 bitsPerPixel;
            fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
            fread(&bitsPerPixel, 2, 1, imageFile);
            image->bytesPerPixel = ((int32)bitsPerPixel) / 8;

            int paddedRowSize = (int)(4 * ceil((float)(image->width) / 4.0f)) * (image->bytesPerPixel);
            int unpaddedRowSize = (image->width) * (image->bytesPerPixel);
            int totalSize = unpaddedRowSize * (image->height);
            image->pixels = (byte*)malloc(totalSize);
            int i = 0;
            byte* currentRowPointer = image->pixels + ((image->height - 1) * unpaddedRowSize);
            for (i = 0; i < image->height; i++)
            {
                fseek(imageFile, dataOffset + (i * paddedRowSize), SEEK_SET);
                fread(currentRowPointer, 1, unpaddedRowSize, imageFile);
                currentRowPointer -= unpaddedRowSize;
            }
            
            fclose(imageFile);
            return image;
  
}

BitmapManager::BitmapManager(const char* filename)
{
    //Za³aduj bitmapê
    //this->loadBMP(filename);
    image = loadBMP(filename);


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

void BitmapManager::printImageOnConsole()
{
    auto start = chrono::high_resolution_clock::now();

    //Wypisz ca³y obraz
    for (int i = 0; i < image->height * image->width; i += 3)
    {
        // caly pixel
        cout << (int)image->pixels[i]; //b
        cout << " ";
        cout << (int)image->pixels[i + 1];  //g
        cout << " ";
        cout << (int)image->pixels[i + 2];  //r 
        cout << endl;
    }

    auto finish = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = finish - start;

    //Wypisz w konsoli czas wyœwietlania obrazu
    cout << endl;
    cout << "Displaying image on the screen took " << duration.count() << " seconds." << endl;
    cout << endl;
}

void BitmapManager::printPixels(int amount)
{
    for (int i = 0; i <amount; i += 3)
    {
        // caly pixel
        cout << (int)image->pixels[i]; //b
        cout << " ";
        cout << (int)image->pixels[i + 1];  //g
        cout << " ";
        cout << (int)image->pixels[i + 2];  //r 
        cout << endl;
    }
}

void BitmapManager::runSepia(int threadsNumber, bool useASM)
{
    vector<thread> threads;
//    auto bytesPerRow = this->infoHeader.biSizeImage / this->infoHeader.biHeight; 
//
    int step = (image->width * image->height) / 3 / threadsNumber; //krok do watku
    int bytesPerRow;
    unsigned char* a;
    //W zale¿noœci od wyboru u¿yj odpowiedniego uchwytu procedury
    if (useASM == true) {
        if (handleToAsmSepia == NULL) { // Jeœli nie uda³o siê za³adowaæ procedury
           cout << "Error: Have not found the proper function." << endl;
        }
        else {
            for (int i = 0; i < threadsNumber; i++) //Utwórz tyle w¹tków, ile zosta³o podane
            {
                //thread t(this->handleToAsmSepia, image->pixels, image->pixels[i * step * 3], image->pixels[(i + 1) * step * 3]);

                threads.push_back(thread(this->handleToAsmSepia, image->pixels, image->pixels[i * step * 3], image->pixels[(i + 1) * step * 3]));
            }
                // start index image->pixels[i*step*3]
                //end index image.pixels[(i+1)*step*3]
                // if i=threads-1 to endindex=  (image->width * image->height)
                
            for (auto& t : threads) //Zaczekaj, a¿ wszystkie w¹tki zakoñcz¹ pracê
                t.join();
            
        }
    }
    else {
        if (handleToCPPSepia == NULL) {
            cout << "Error: Have not found the proper function." << endl;
        }
        else {
            for (int i = 0; i < threadsNumber; i++)
                threads.push_back(thread([this](int elem1, int elem2) {
                cout << this->handleToCPPSepia(elem1, elem2) << " ";
                    }, 3, 5));
            for (auto& t : threads)
                t.join();
            
        }
    }

}

BitmapManager::~BitmapManager()
{
    //Zwolnij uchwyty do bibliotek
    FreeLibrary(this->hinstLibAsm);
    FreeLibrary(this->hinstLibCPP);
    delete[] this->imageData;
}
