#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.h"

using namespace std;

const int SCALE = 10;

// Chip8 Keypad
const Uint32 KEY_0 = SDL_SCANCODE_KP_0;
const Uint32 KEY_1 = SDL_SCANCODE_KP_1;
const Uint32 KEY_2 = SDL_SCANCODE_KP_2;
const Uint32 KEY_3 = SDL_SCANCODE_KP_3;
const Uint32 KEY_4 = SDL_SCANCODE_KP_4;
const Uint32 KEY_5 = SDL_SCANCODE_KP_5;
const Uint32 KEY_6 = SDL_SCANCODE_KP_6;
const Uint32 KEY_7 = SDL_SCANCODE_KP_7;
const Uint32 KEY_8 = SDL_SCANCODE_KP_8;
const Uint32 KEY_9 = SDL_SCANCODE_KP_9;
const Uint32 KEY_A = SDL_SCANCODE_Q;
const Uint32 KEY_B = SDL_SCANCODE_A;
const Uint32 KEY_C = SDL_SCANCODE_Z;
const Uint32 KEY_D = SDL_SCANCODE_W;
const Uint32 KEY_E = SDL_SCANCODE_S;
const Uint32 KEY_F = SDL_SCANCODE_X;

SDL_Window * window = nullptr;
SDL_Surface * screen = nullptr;
Chip8 chip8;
Uint32 palette[2];  // Two colors, black and white

bool setupGraphics();
void setupInput();   // TODO
bool handleEvents(SDL_Event*);
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
    chip8.loadGame("ROMS/TICTAC");

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
	    if (handleEvents(&e))
		quit = true;

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

bool handleEvents(SDL_Event* e)
{
    if (e->type == SDL_KEYUP)
    {

    }
    else if (e->type == SDL_KEYDOWN)
    {

    }
    const Uint8 *keystates = SDL_GetKeyboardState( NULL );
    chip8.key[0x0] = keystates[KEY_0];
    chip8.key[0x1] = keystates[KEY_1];
    chip8.key[0x2] = keystates[KEY_2];
    chip8.key[0x3] = keystates[KEY_3];
    chip8.key[0x4] = keystates[KEY_4];
    chip8.key[0x5] = keystates[KEY_5];
    chip8.key[0x6] = keystates[KEY_6];
    chip8.key[0x7] = keystates[KEY_7];
    chip8.key[0x8] = keystates[KEY_8];
    chip8.key[0x9] = keystates[KEY_9];
    chip8.key[0xA] = keystates[KEY_A];
    chip8.key[0xB] = keystates[KEY_B];
    chip8.key[0xC] = keystates[KEY_C];
    chip8.key[0xD] = keystates[KEY_D];
    chip8.key[0xE] = keystates[KEY_E];
    chip8.key[0xF] = keystates[KEY_F];
    return false;
}

void stopGraphics()
{
    cout << "Stopping SDL..." << endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
}
