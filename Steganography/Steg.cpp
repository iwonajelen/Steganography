#include "Steg.hpp"

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

bool algorithm::ifTextFits(SDL_Surface *image, string text)
{
    if(text.size()*8+64>image->h*image->w*3)
        return false;
    return true;
}

string algorithm::coding(string text, string key)
{ //xor
    for(unsigned int i=0;i<text.size();++i)
        text[i]^=key[i%key.size()];
    return text;
}


void algorithm::length(SDL_Surface *image, string text, bool lengthBin[64])
{
     unsigned int length=text.size();
    for(int k=0;k<64;++k)
    {
        if((int)(length/pow(2,(63-k))))
        {
            lengthBin[k]=true;
            length-=pow(2,(63-k));
        }
        else
            lengthBin[k]=false;
    }
}

void algorithm::decToBin(string text, bool *textBin)
{
    for(unsigned int i=0;i<text.size();i++)
    {
        for(int j=0;j<8;j++)
        {
            if( (int)(text[i]/pow( 2,(7-j) ) ) == true )
            {
                textBin[i*8+j]=true;
                text[i]-=pow( 2,(7-j) );
            }
            else
                textBin[i*8+j]=false;
        }
    }
}

void algorithm::writeLength(SDL_Surface *image, bool *lengthBin, string text)
{
    SDL_Color color;
    unsigned int i = 0;

    for(int x=0;x<image->w;x++)
    {
        for(int y=0;y<image->h;y++)
        {
            color=getPixel(x,y,image);

            color.r=color.r>>1;
            color.r=color.r<<1;
            if(lengthBin[i])
                color.r++;

            color.g=color.g>>1;
            color.g=color.g<<1;
            if(lengthBin[i+1])
                color.g++;

            if(i<63)
            {
                color.b=color.b>>1;
                color.b=color.b<<1;
                if(lengthBin[i+2])
                    color.b++;
            }

            setPixel(x,y,color.r,color.g,color.b,image);

            if(i>=63)
                return;

            i+=3;
        }
    }
}

void algorithm::steganography(SDL_Surface* image, string text, bool *textBin)
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
                    color.r=color.r>>1;
                    color.r=color.r<<1;
                    if(textBin[j])
                        color.r++;

                    color.g=color.g>>1;
                    color.g=color.g<<1;
                    if(textBin[j+1])
                        color.g++;
                }

                if(i>=63)
                {
                    color.b=color.b>>1;
                    color.b=color.b<<1;
                    if(textBin[j+2])
                        color.b++;
                }

            setPixel(x,y,color.r,color.g,color.b,image);

            j+=3;
            i+=3;

            if(i>text.size()*8+63)
                return;
        }
    }
}
