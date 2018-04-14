#include "Desteg.hpp"

algorithm::algorithm() {}

algorithm::~algorithm() {}

void algorithm::setPixel(int y, int x, Uint8 R, Uint8 G, Uint8 B, SDL_Surface *image)
{
  if ((x>=0) && (x<image->w) && (y>=0) && (y<image->h))
  {
    /* Zamieniamy poszczególne sk?adowe koloru na format koloru pixela */
    Uint32 pixel = SDL_MapRGB(image->format, R, G, B);

    /* Pobieramy informacji ile bajtów zajmuje jeden pixel */
    int bpp = image->format->BytesPerPixel;

    /* Obliczamy adres pixela */
    Uint8 *p = (Uint8 *)image->pixels + y * image->pitch + x * bpp;

    /* Ustawiamy warto?? pixela, w zale?no?ci od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;

    }
  }
}

SDL_Color algorithm::getPixel (int y, int x, SDL_Surface* image)
{
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<image->w) && (y>=0) && (y<image->h))
    {
        //determine position
        char* pPosition=(char*)image->pixels ;
        //offset by y
        pPosition+=(image->pitch*y) ;
        //offset by x
        pPosition+=(image->format->BytesPerPixel*x);
        //copy pixel data
        memcpy(&col, pPosition, image->format->BytesPerPixel);
        //convert color
        SDL_GetRGB(col, image->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}

string algorithm::coding(int textSize, string text, string key, SDL_Surface *image)
{ //xor
    for(int i=0;i<textSize;i++)
    {
        text[i]^=key[i%key.size()];
    }
    return text;
}

unsigned int algorithm::length(SDL_Surface *image, bool lengthBin[64])
{
    unsigned int textSize = 0;
    for(int j=0;j<64;++j)
    {
        if(lengthBin[j])
        textSize+=pow(2,(63-j));
    }
    return textSize;
}

string algorithm::binToDec(bool *textBin, SDL_Surface *image, unsigned int textSize)
{
    char temp;
    string text="";

    for(unsigned int i=0;i<8*textSize;i+=8)
    {
        temp=0;
        for(int j=0;j<8;j++)
        {
            temp=temp<<1;
            temp+=textBin[j+i];
        }
        text+=temp;
    }
    return text;
}

void algorithm::readLength(SDL_Surface* image, bool lengthBin[64])
{
    SDL_Color color;
    unsigned int i = 0;

    for(int x=0;x<image->w;x++)
    {
        for(int y=0;y<image->h;y++)
        {
            color=getPixel(x,y,image);

            color.r=color.r<<7;
            color.r=color.r>>7;
            lengthBin[i]=color.r;

            color.g=color.g<<7;
            color.g=color.g>>7;
            lengthBin[i+1]=color.g;

            if(i<63)
            {
                color.b=color.b<<7;
                color.b=color.b>>7;
                lengthBin[i+2]=color.b;
            }
            if(i>=63)
                return;

            i+=3;
        }
    }
}

void algorithm::desteganography(SDL_Surface* image, bool *textBin, unsigned int textSize)
{
    SDL_Color color;
    unsigned int i=0, j=0;

    for(int x=0;x<image->w;x++)
    {
        for(int y=0;y<image->h;y++)
        {
            if(i<63)
            {
                i+=3;
                continue;
            }

            color=getPixel(x,y,image);

            if(i>63)
            {
                color.r=color.r<<7;
                color.r=color.r>>7;
                textBin[j]=color.r;

                color.g=color.g<<7;
                color.g=color.g>>7;
                textBin[j+1]=color.g;
            }

            if(i>=63)
            {
                color.b=color.b<<7;
                color.b=color.b>>7;
                textBin[j+2]=color.b;
            }

            j+=3;
            i+=3;

            if(i>8*3*textSize+63)
                return;
        }
    }
}

