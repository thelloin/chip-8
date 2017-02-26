#ifndef IOHANDLER_H
#define IOHANDLER_H

#include "chip8.h"
#include <SDL2/SDL.h>

class IOHandler {
public:
    IOHandler(Chip8* chip8) : chip8_m{chip8} {}

    bool setup();
    bool handleEvent();
    void drawGraphics();
    void stopGraphics();

    

private:
    Chip8* chip8_m;

    SDL_Window * window = nullptr;
    SDL_Surface * screen = nullptr;
    Uint32 palette[2];  // Two colors, black and white
    const int SCALE = 10;
};

#endif
