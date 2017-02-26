#include <iostream>
#include <SDL2/SDL.h>
#include <map>

#include "chip8.h"
#include "IOHandler.h"

using namespace std;

//const int SCALE = 10;


Chip8 chip8;


//bool setupGraphics();
//void setupInput();   // TODO
//bool handleEvents(SDL_Event*);
//void drawGraphics();
//void stopGraphics();

int main()
{

    IOHandler iohandler{&chip8};
    
    cout << "Initializing SDL..." << endl;
    // Set up render system and register input callbacks
    if ( !iohandler.setup() )
    {
	cout << "Error setting up SDL\n" << endl;
	return 1;
    }
    // setupInput();

    cout << "Initializing chip-8" << endl;
    // Initialize the Chip8 system and load the game into memory
    chip8.initialize();
    chip8.loadGame("ROMS/PONG2");

    map<SDL_Keycode, uint8_t> keymap;
    keymap.insert({SDLK_1, 0x1});
    keymap.insert({SDLK_2, 0x2});
    keymap.insert({SDLK_3, 0x3});
    keymap.insert({SDLK_4, 0xc});
    keymap.insert({SDLK_q, 0x4});
    keymap.insert({SDLK_w, 0x5});
    keymap.insert({SDLK_e, 0x6});
    keymap.insert({SDLK_r, 0xd});
    keymap.insert({SDLK_a, 0x7});
    keymap.insert({SDLK_s, 0x8});
    keymap.insert({SDLK_d, 0x9});
    keymap.insert({SDLK_f, 0xe});
    keymap.insert({SDLK_z, 0xa});
    keymap.insert({SDLK_x, 0x0});
    keymap.insert({SDLK_c, 0xb});
    keymap.insert({SDLK_v, 0xf});

    SDL_Event e;
    bool quit = false;

    // Emulation loop
    while (!quit)
    {
	// Emulate one cycle
	chip8.emulateCycle();

	// If the draw flag is set, update the screen
	if (chip8.drawFlag)
	    iohandler.drawGraphics();

	while (SDL_PollEvent(&e))
	{
	    switch(e.type) {
	    case SDL_QUIT:
		quit = true;
		break;
	    case SDL_KEYDOWN:
	    {
		const auto key = e.key.keysym.sym;
		if (key == SDLK_ESCAPE) quit = true;
		if(keymap.find(key) != keymap.end()) chip8.pressKey(keymap[key]);
	    }
	    break;
	    case SDL_KEYUP:
	    {
		const auto key = e.key.keysym.sym;
		if (keymap.find(key) != keymap.end()) chip8.releaseKey(keymap[key]);
	    }
	    break;

	    }
	}

	// Put a simple delay. TODO: Better implementation
	SDL_Delay(2);
    }

    iohandler.stopGraphics();

    return 0;
}

/*bool setupGraphics()
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
*/
