#include "Sepia_Projekt_Manager.h"
//#include "Sepia_Params.h"

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

            Image* imageData = new Image();
            

            FILE* imageFile = fopen(filename, "rb");
            int32 dataOffset;
            fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
            fread(&dataOffset, 4, 1, imageFile);
            fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
            fread(&imageData->width, 4, 1, imageFile);
            fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
            fread(&imageData->height, 4, 1, imageFile);
            int16 bitsPerPixel;
            fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
            fread(&bitsPerPixel, 2, 1, imageFile);
            imageData->bytesPerPixel = ((int32)bitsPerPixel) / 8;

            int paddedRowSize = (int)(4 * ceil((float)(imageData->width) / 4.0f)) * (imageData->bytesPerPixel);
            int unpaddedRowSize = (imageData->width) * (imageData->bytesPerPixel);
            int totalSize = unpaddedRowSize * (imageData->height);
            imageData->pixels = (byte*)malloc(totalSize);
            int i = 0;
            byte* currentRowPointer = imageData->pixels + ((imageData->height - 1) * unpaddedRowSize);
            for (i = 0; i < imageData->height; i++)
            {
                fseek(imageFile, dataOffset + (i * paddedRowSize), SEEK_SET);
                fread(currentRowPointer, 1, unpaddedRowSize, imageFile);
                currentRowPointer -= unpaddedRowSize;
            }
            
            
            //this -> sepiaImageData = new unsigned char[totalSize];
            

            fclose(imageFile);

            imageData->Pixels = new RGBApixel * [imageData->width];
            imageData->Pixels[0] = new RGBApixel[imageData->height];
            Image& Input;
            for (int j = 0; j < imageData->height; j++)
            {
                for (int i = 0; i < imageData->width; i++)
                {
                    imageData->Pixels[i][j] = *Input(i, j);
                    //   Pixels[i][j] = Input.GetPixel(i,j); // *Input(i,j);
                }
            }
            /*for (int i = 0; i < amount; i += 3)
            {
                // caly pixel
                cout << (int)imageData->pixels[i]; //b
                cout << " ";
                cout << (int)imageData->pixels[i + 1];  //g
                cout << " ";
                cout << (int)imageData->pixels[i + 2];  //r 
                cout << endl;
            }*/
            return imageData;
  
}

RGBApixel** BitmapManager::GetTable()
{
   
    return this->Pixels;

}

void BitmapManager::writeImage(const char* fileName)
{
    FILE* outputFile = fopen(fileName, "wb");
    //*****HEADER************//
    const char* BM = "BM";
    fwrite(& BM[0], 1, 1, outputFile);
    fwrite(& BM[1], 1, 1, outputFile);
    int paddedRowSize = (int)(4 * ceil((float)image->width / 4.0f)) * image->bytesPerPixel;
    int32 fileSize = paddedRowSize * image->height + HEADER_SIZE + INFO_HEADER_SIZE;
    fwrite(& fileSize, 4, 1, outputFile);
    int32 reserved = 0x0000;
    fwrite(& reserved, 4, 1, outputFile);
    int32 dataOffset = HEADER_SIZE + INFO_HEADER_SIZE;
    fwrite(& dataOffset, 4, 1, outputFile);

    //*******INFO*HEADER******//
    int32 infoHeaderSize = INFO_HEADER_SIZE;
    fwrite(& infoHeaderSize, 4, 1, outputFile);
    fwrite(& image->width, 4, 1, outputFile);
    fwrite(& image->height, 4, 1, outputFile);
    int16 planes = 1; //always 1
    fwrite(& planes, 2, 1, outputFile);
    int16 bitsPerPixel = image->bytesPerPixel * 8;
    fwrite(& bitsPerPixel, 2, 1, outputFile);
    //write compression
    int32 compression = NO_COMPRESION;
    fwrite(& compression, 4, 1, outputFile);
    //write image size(in bytes)
        int32 imageSize = image->width * image->height * image->bytesPerPixel;
    fwrite(& imageSize, 4, 1, outputFile);
    int32 resolutionX = 11811; //300 dpi
    int32 resolutionY = 11811; //300 dpi
    fwrite(& resolutionX, 4, 1, outputFile);
    fwrite(& resolutionY, 4, 1, outputFile);
    int32 colorsUsed = MAX_NUMBER_OF_COLORS;
    fwrite(& colorsUsed, 4, 1, outputFile);
    int32 importantColors = ALL_COLORS_REQUIRED;
    fwrite(& importantColors, 4, 1, outputFile);
    int i = 0;
    int unpaddedRowSize = image->width * image->bytesPerPixel;
    for (i = 0; i < image->height; i++)
    {
        int pixelOffset = ((image->height - i) - 1) * unpaddedRowSize;
        fwrite(&image->pixels[pixelOffset], 1, paddedRowSize, outputFile);
    }
    fclose(outputFile);
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
   
//    auto bytesPerRow = this->infoHeader.biSizeImage / this->infoHeader.biHeight; 
    int range = image->width / threadsNumber; //w zakresie znajduje sie liczba wierszy któr¹ przyjmuje dany w¹tek
    if (image->width % threadsNumber)
        range += 1; //dodanie wiersza do ka¿dego w¹tku, kiedy dzielenie z reszt¹- w ostatnim w¹tku bêdzie o 1 mniej wiersz/o 2?

    int rows = image->height;
   //int sizeBt = image->width * image->height;
   // int step = (sizeBt) / 3 / threadsNumber; //krok do watku
    
   // int bytesPerRow = 0;
    //unsigned char* a = nullptr;
    //W zale¿noœci od wyboru u¿yj odpowiedniego uchwytu procedury
    if (useASM == true) {
        if (handleToAsmSepia == NULL) { // Jeœli nie uda³o siê za³adowaæ procedury
           cout << "Error: Have not found the proper function." << endl;
        }
        else {
            int counter = 1;
            std::vector<std::thread> threads;
            for (int i = 0; i < image->width; i+=range) //Utwórz tyle w¹tków, ile zosta³o podane
            {   
                int progress = range * counter; //indeks koñcowej kolumny
                if (progress >= image->width)
                    threads.push_back(std::thread(handleToAsmSepia, rows, image->pixels, i, image->width));
                else
                    threads.push_back(std::thread(handleToAsmSepia, rows, image->pixels, i, progress));
                counter++;
            }
             
                
            for (auto& t : threads) 
            {//Zaczekaj, a¿ wszystkie w¹tki zakoñcz¹ pracê
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
            for (int i = 0; i < image->width; i += range)
            {
                int progress = range * counter;

                if (progress >= image->width)
                    threads.push_back(std::thread(handleToCPPSepia, image->pixels, i, image->width, image->height));
                else
                    threads.push_back(std::thread(handleToCPPSepia, image->pixels, i, progress, image->height));
            }
            for (auto& t : threads)
            {
                t.join();
            }
            //koniec czasu
        }
    }
  
    // zapis zmienionego obrazu do pliku ustalonego z góry
}

BitmapManager::~BitmapManager()
{
    //Zwolnij uchwyty do bibliotek
    FreeLibrary(this->hinstLibAsm);
    FreeLibrary(this->hinstLibCPP);
    delete[] this->imageData;
   // delete[] this->sepiaImageData;
}
