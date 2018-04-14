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
        bool ifTextFits(SDL_Surface *image, string text);
        string coding(string text, string key);
        void length(SDL_Surface *image, string text, bool* lengthBin);
        void decToBin(string text, bool *textBin);
        void writeLength(SDL_Surface *image, bool *lengthBin, string text);
        void steganography(SDL_Surface* image, string text, bool *textBin);
};


