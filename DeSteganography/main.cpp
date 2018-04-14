#include "Desteg.hpp"

int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // load an image
    SDL_Surface *image = SDL_LoadBMP("image2.bmp");
    if (!image)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    algorithm desteg;

    //input key
    string key;
    cout<<"enter the key: "<<endl;
    cin>>key;

    //input text
    fstream file;
    file.open("file.txt",ios::out);

    if(!file){
        cout<<"can't load file"<<endl;
        return 1;
    }

    bool lengthBin[64];

    desteg.readLength(image,lengthBin);

    unsigned int textSize = desteg.length(image,lengthBin);

    bool *textBin=new bool[3*image->w*image->h-64];

    desteg.desteganography(image,textBin,textSize);

    string text=desteg.binToDec(textBin,image,textSize);

    string decoded=desteg.coding(textSize,text,key,image);
    cout<<decoded;

    file<<decoded;

    //terminate
    delete[] textBin;
    SDL_FreeSurface(image);
    SDL_Quit();
    file.close();
    system("pause >nul");

    return 0;
}
