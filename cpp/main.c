#include <iostream>

#include "chip8.h"

using namespace std;

int main()
{
    Chip8 chip8;
    
    // Set up render system and register input callbacks
    // setupGraphics();
    // setupInput();

    // Initialize the Chip8 system and load the game into memory
    chip8.initialize();
    chip8.loadGame("pong");

    // Emulation loop
    for (int i = 0; i < 10; ++i)
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
