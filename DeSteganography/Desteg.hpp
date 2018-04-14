#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include <sdl/SDL.h>

#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

using namespace std;

class algorithm
{
    public:
        algorithm();
        ~algorithm();
        void setPixel(int y, int x, Uint8 R, Uint8 G, Uint8 B, SDL_Surface *image);
        SDL_Color getPixel (int y, int x, SDL_Surface* image);
        string coding(int textSize, string text, string key, SDL_Surface *image);
        unsigned int length(SDL_Surface *image, bool *lengthBin);
        string binToDec(bool *textBin, SDL_Surface *image, unsigned int textSize);
        void readLength(SDL_Surface* image, bool *lengthBin);
        void desteganography(SDL_Surface* image, bool *textBin, unsigned int textSize);
};



