#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.h"

using namespace std;

const int SCALE = 10;

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

SDL_Window * window = nullptr;
SDL_Surface * screen = nullptr;
Chip8 chip8;
Uint32 palette[2];  // Two colors, black and white

bool setupGraphics();
void setupInput();   // TODO
void drawGraphics();
void stopGraphics();

int main()
{
    
    cout << "Initializing SDL..." << endl;
    // Set up render system and register input callbacks
    if ( !setupGraphics() )
    {
	cout << "Error setting up SDL\n" << endl;
	return 1;
    }
    // setupInput();

    cout << "Initializing chip-8" << endl;
    // Initialize the Chip8 system and load the game into memory
    chip8.initialize();
    chip8.loadGame("ROMS/PONG2");

    SDL_Event e;
    bool quit = false;

    // Emulation loop
    while (!quit)
    {
	// Emulate one cycle
	chip8.emulateCycle();

	// If the draw flag is set, update the screen
	if (chip8.drawFlag)
	    drawGraphics();

	while (SDL_PollEvent(&e))
	{
	    if (e.type == SDL_QUIT)
		quit = true;
	}

	// Store key press state (Press and Release)
	chip8.setKeys();

	// Put a simple delay. TODO: Better implementation
	SDL_Delay(3);
    }

    stopGraphics();

    return 0;
}

bool setupGraphics()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
	cout << SDL_GetError() << endl;
	return false;
    }

    window = SDL_CreateWindow("Chip-8 Interpreter",
			      SDL_WINDOWPOS_CENTERED,
			      SDL_WINDOWPOS_CENTERED,
			      640,
			      320,
			      SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
	cout << SDL_GetError() << endl;
	return false;
    }

    screen = SDL_GetWindowSurface(window);

    palette[0] = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    palette[1] = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);

    return true;
}

void drawGraphics()
{
    SDL_Rect pixel = {0, 0, SCALE, SCALE};

    for (unsigned int x = 0; x < VIDEO_WIDTH; ++x)
    {
	pixel.x = x * SCALE;
	for (unsigned int y = 0; y < VIDEO_HEIGHT; ++y)
	{
	    pixel.y = y * SCALE;
	    SDL_FillRect(screen, &pixel, palette[chip8.gfx[x + y*VIDEO_WIDTH]]);
	}
    }
    SDL_UpdateWindowSurface(window);
    chip8.drawFlag = false;
}

void stopGraphics()
{
    cout << "Stopping SDL..." << endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
}
