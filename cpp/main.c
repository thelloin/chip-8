#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.h"

using namespace std;

static int keymap[0x10] = {
    SDLK_0,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_5,
    SDLK_6,
    SDLK_7,
    SDLK_8,
    SDLK_9,
    SDLK_a,
    SDLK_b,
    SDLK_c,
    SDLK_d,
    SDLK_e,
    SDLK_f
};

SDL_Window * screen;

void setupGraphics()
{
    SDL_Init(SDL_INIT_VIDEO);
    //SDL_SetVideoMode(640, 320, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    screen = SDL_CreateWindow("Chip-8 Interpreter",
			      SDL_WINDOWPOS_CENTERED,
			      SDL_WINDOWPOS_CENTERED,
			      640,
			      320,
			      0);
}


int main()
{
    Chip8 chip8;
    
    // Set up render system and register input callbacks
    setupGraphics();
    // setupInput();

    // Initialize the Chip8 system and load the game into memory
    chip8.initialize();
    chip8.loadGame("ROMS/PONG");

    // Emulation loop
    for (int i = 0; i < 8000; ++i)
    {
	// Emulate one cycle
	chip8.emulateCycle();

	// If the draw flag is set, update the screen
	if (chip8.drawFlag)
	    // drawGraphics();

	// Store key press state (Press and Release)
	chip8.setKeys();
    }

    return 0;
}
