#include "chip8.h"

#include <iostream>
#include <stdlib.h>

// The chip-8 font set
unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::initialize()
{
    pc      = 0x200;   // Program counter starts av 0x200
    opcode  = 0;       // Reset current opcode
    I       = 0;       // Reset index register
    sp      = 0;       // Reset stack pointer

    // Clear display
    // Clear stack
    // Clear registers V0-VF
    // Clear memory

    // Load fontset
    for (int i = 0; i < 80; ++i)
	memory[i] = chip8_fontset[i];

    // Reset timers
}

void Chip8::loadGame(std::string name)
{
    // Load the program into memory (use fopen in binary mode) and start
    // filling the memory at location 0x200 = 512

    //for (int i = 0; i < bufferSize; ++i)
    //memory[i + 512] = buffer[i];
}

void Chip8::emulateCycle()
{
    // Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // Decode Opcode
    switch (opcode & 0xF000)
    {
    case 0x0000:
	switch (opcode & 0x000F)
	{
	case 0x0000: // 0x00E0: Clears the screen
	    // Execute opcode
	    break;
	case 0x000E: // 0x00EE: Returns from subroutine
	    // Execute opcode
	    break;
	default:
	    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
	}
	break;

    case 0x8000:
	switch (opcode & 0x000F)
	{
	case 0x0004: // 0x8XY4: Add VY to VX. Set VF to 1 if carry
	    if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
		V[0xF] = 1; // Set carry
	    else
		V[0xF] = 0;
	    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
	    pc += 2;
	    break;
	default:
	    printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
	}
	break;

    case 0x2000: // 2NNN: Calls subroutine at NNN
	stack[sp] = pc;
	++sp;
	pc = opcode & 0x0FFF;
	break;

    case 0xA000: // ANNN: Sets I to the adress NNN
	// Execute Opcode
	I = opcode & 0x0FFF;
	pc += 2;
	break;

    case 0xD000:
    {
	unsigned short x = V[(opcode & 0x0F00) >> 8];
	unsigned short y = V[(opcode & 0x00F0) >> 4];
	unsigned short height = opcode & 0x000F;
	unsigned short pixel;

	V[0xF] = 0;
	for (int yline = 0; yline < height; ++yline)
	{
	    pixel = memory[I + yline];
	    for (int xline = 0; xline < 8; ++xline)
	    {
		if ((pixel & (0x80 >> xline)) != 0)
		{
		    if (gfx[(x + xline + ((y + yline) * 64))] == 1)
			V[0xF] = 1;
		    gfx[x + xline + ((y + yline) * 64)] ^= 1;
		}
	    }
	}
	drawFlag = true;
	pc += 2;
    }
    break;

    case 0xE000:
	switch (opcode & 0x00FF)
	{
	case 0x009E: // 0xEX9E: Skips the next instruction if the key in VX is pressed
	    if (key[V[(opcode & 0x0F00) >> 8]] != 0)
		pc += 4;
	    else
		pc += 2;
	    break;
	}
	break;

    case 0xF000:
	switch (opcode % 0x00FF)
	{
	case 0x0033: // 0xFX33: Store the binary-coded decimal representation of VX
	    memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
	    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 100) % 10;
	    memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
	    pc += 2;
	    break;
	}
	break;

    default:
	printf("Unknown opcode: 0x%X\n", opcode);
    }


    // Update Timers
    if (delay_timer > 0)
	--delay_timer;

    if (sound_timer > 0)
    {
	if (sound_timer == 1)
	    printf("BEEP!\n");
	--sound_timer;
    }

    std::cout << "In emulate cycle" << std::endl;
}
