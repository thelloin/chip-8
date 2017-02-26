#include "IOHandler.h"
#include <iostream>

bool IOHandler::setup()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
	std::cout << SDL_GetError() << std::endl;
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
	std::cout << SDL_GetError() << std::endl;
	return false;
    }

    screen = SDL_GetWindowSurface(window);

    palette[0] = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    palette[1] = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);

    return true;
}

bool IOHandler::handleEvent()
{

    return true;
}
void IOHandler::drawGraphics()
{
    SDL_Rect pixel = {0, 0, SCALE, SCALE};

    for (unsigned int x = 0; x < VIDEO_WIDTH; ++x)
    {
	pixel.x = x * SCALE;
	for (unsigned int y = 0; y < VIDEO_HEIGHT; ++y)
	{
	    pixel.y = y * SCALE;
	    SDL_FillRect(screen, &pixel, palette[chip8_m->gfx[x + y*VIDEO_WIDTH]]);
	}
    }
    SDL_UpdateWindowSurface(window);
    chip8_m->drawFlag = false;
}

void IOHandler::stopGraphics()
{
    std::cout << "Stopping SDL..." << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
}
