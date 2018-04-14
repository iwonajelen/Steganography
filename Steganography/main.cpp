#include "Steg.hpp"

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
    SDL_Surface *image = SDL_LoadBMP("image.bmp");
    if (!image)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    algorithm steg;

    //input key
    string key;
    cout<<"enter the key: "<<endl;
    cin>>key;

    //input text
    string text;
    fstream file;
    file.open("file.txt",ios::in);
    getline(file,text);

    if(!file)
    {
        cout<<"can't load file"<<endl;
        return 1;
    }

    if(!steg.ifTextFits(image,text)){
        cout<<"the text can't fit in this image";
        return 2;
    }

    string encoded=steg.coding(text,key);

    bool lengthBin[64];

    steg.length(image,text,lengthBin);

    steg.writeLength(image,lengthBin,text);

    bool *textBin=new bool[text.size()*8];

    steg.decToBin(encoded,textBin);

    steg.steganography(image,encoded,textBin);

    SDL_SaveBMP(image,"image2.bmp");

    //terminate
    delete[] textBin;
    SDL_FreeSurface(image);
    SDL_Quit();
    file.close();
    system("pause>nul");

    return 0;
}
