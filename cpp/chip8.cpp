#include "chip8.h"

#include <iostream>
#include <stdlib.h>

/*
 * Opcodes to implement:
 * 0x0NNN
 * 0x00E0 *
 * 0x00EE *
 * 0x1NNN *
 * 0x2NNN *
 * 0x3XNN *
 * 0x4XNN *
 * 0x5XY0 *
 * 0x6XNN *
 * 0x7XNN *
 * 0x8XY0 *
 * 0x8XY1 *
 * 0x8XY2 *
 * 0x8XY3 *
 * 0x8XY4 *
 * 0x8XY5 *
 * 0x8XY6 *
 * 0x8XY7 *
 * 0x8XYE *
 * 0x9XY0 *
 * 0xANNN *
 * 0xBNNN *
 * 0xCXNN *
 * 0xDXYN *
 * 0xEX9E *
 * 0xEXA1 *
 * 0xFX07 *
 * 0xFX0A *
 * 0xFX15 *
 * 0xFX18 *
 * 0xFX1E *
 * 0xFX29 *
 * 0xFX33 *
 * 0xFX55 *
 * 0xFX65 *
 */

//#define DEBUG

#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

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
    for(unsigned int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; ++i)
	gfx[i] = 0;
    // Clear stack
    for(unsigned int i = 0; i < STACK_SIZE; ++i)
	stack[i] = 0;
    // Clear registers V0-VF
    for (unsigned int i = 0; i < VREG_SIZE; ++i)
	V[i] = 0;
    // Clear memory
    for (unsigned int i = 0; i < MEMORY_SIZE; ++i)
	memory[i] = 0;

    // Load fontset
    for (int i = 0; i < 80; ++i)
	memory[i] = chip8_fontset[i];

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;

    drawFlag = true;
}

bool Chip8::loadGame(char* name)
{
    // Load the program into memory starting at location 0x200 = 512
    printf("Loading game : %s\n", name);

    // Open file
    FILE * pFile = fopen(name, "rb");
    if (pFile == NULL)
    {
	fputs("File error", stderr);
	return false;
    }

    // Check file size
    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);
    printf("Filesize: %d\n", (int)lSize);

    // Allocate memory to contain the whole file
    char * buffer = (char*)malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
	fputs("Memory error", stderr);
	return false;
    }

    // Copy the file into the buffer
    size_t result = fread(buffer, 1, lSize, pFile);
    if ((int)result != lSize)
    {
	fputs("Reading error", stderr);
	return false;
    }

    // Copy buffer to chip-8 memory
    if ((MEMORY_SIZE - PROGRAM_START) > lSize)
    {
	for (int i = 0; i < lSize; ++i)
	    memory[i + PROGRAM_START] = buffer[i];
    }
    else
    {
	printf("Error: ROM to big for memory");
	return false;
    }

    // Close file, free buffer
    fclose(pFile);
    free(buffer);

    return true;
}

void Chip8::pressKey(const uint8_t k)
{
    key[k] = 1;
}

void Chip8::releaseKey(const uint8_t k)
{
    key[k] = 0;
}

void Chip8::emulateCycle()
{
    // Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    DEBUG_PRINT(("opcode: 0x%X\n", opcode));
    // Decode Opcode
    switch (opcode & 0xF000)
    {
    case 0x0000:
	switch (opcode & 0x000F)
	{
	case 0x0000: // 0x00E0: Clears the screen
	{
	    for (int i = 0; i < 2048; ++i)
		gfx[i] = 0x0;
	    drawFlag = true;
	    pc += 2;
	}
	break;

	case 0x000E: // 0x00EE: Returns from subroutine
	    pc = stack[--sp];
	    pc += 2;   
	    break;

	default:
	    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
	}
	break;

    case 0x1000: // 1NNN: Jumps to address NNN
	pc = opcode & 0x0FFF;
	break;

    case 0x2000: // 2NNN: Calls subroutine at NNN
	stack[sp] = pc;
	++sp;
	pc = opcode & 0x0FFF;
	break;

    case 0x3000: // 0x3XNN: Skips the next instruction if VX equals NN
	DEBUG_PRINT(("VX : %X\n", V[(opcode & 0x0F00) >> 8]));
	if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
	    pc += 4;
	else
	    pc += 2;
	break;

    case 0x4000: // 0x4XNN: Skips the next instruction if VX doesn't equal NN
	if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
	    pc += 4;
	else
	    pc += 2;
	break;

    case 0x5000: // 0x5XY0: Skips the next instruction if VX == VY
	if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
	    pc += 4;
	else
	    pc += 2;
	break;

    case 0x6000: // 6XNN: Sets VX to NN
	DEBUG_PRINT(("Set V[%X] to %X\n", (opcode & 0x0F00) >> 8, opcode & 0x00FF));
	V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
	pc += 2;
	break;

    case 0x7000: // 7XNN: Adds NN to VX
	V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
	pc += 2;
	break;

    case 0x8000:
	switch (opcode & 0x000F)
	{
	case 0x0000: // 0x8XY0: Sets VX to the value of VY
	    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
	    pc += 2;
	    break;

	case 0x0001: // 0x8XY1: Sets VX to VX or VY (Bitwise OR)
	    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
	    pc += 2;
	    break;

	case 0x0002: // 0x8XY2: Sets VX to VX and(bitOp) VY
	    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
	    pc += 2;
	    break;

	case 0x0003: // 0x8XY3: Sets VX to VX xor VY
	    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
	    pc += 2;
	    break;

	case 0x0004: // 0x8XY4: Add VY to VX. Set VF to 1 if carry
	    if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
		V[0xF] = 1; // Set carry
	    else
		V[0xF] = 0;
	    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
	    pc += 2;
	    break;

	case 0x0005: // 0x8XY5: VY is subtracted from VX. VF is set to 0 when there is a borrow and 1 when there isn't
	    if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
		V[0xF] = 0;
	    else
		V[0xF] = 1;
	    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
	    pc += 2;
	    break;

	case 0x0006: // 0x8XY6: Shifts VX right by one, VF is set to the least significant bit of VX before the shift
	    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
	    V[(opcode & 0x0F00) >> 8] >>= 0x1;
	    pc += 2;
	    break;

	case 0x0007: // 0x8XY7: Sets VX to VY minus VX. Set VF to 0 if there is a borrow
	    if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
		V[0xF] = 0;
	    else
		V[0xF] = 1;
	    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
	    pc += 2;
	    break;

	case 0x000E: // 0x8XYE: Shifts VX left by one. VF is set to the most significant bit before the shift
	    V[0xF] = V[(opcode & 0x0F00) >> 8] >> 0x7;
	    V[(opcode & 0x0F00) >> 8] <<= 0x1;
	    pc += 2;
	    break;

	default:
	    printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
	}
	break;

    case 0x9000: // 9XY0: Skips the next instruction if VX != VY
	if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
	    pc += 4;
	else
	    pc += 2;
	break;

    case 0xA000: // ANNN: Sets I to the adress NNN
	// Execute Opcode
	I = opcode & 0x0FFF;
	pc += 2;
	break;

    case 0xB000: // 0xBNNN: Jumps to the adress NNN plus V0
	pc = (opcode & 0x0FFF) + V[0x0];
	break;

    case 0xC000: // CXNN: Sets VX to the result of a bitwise and on a rand(0-255) and NN
	// TODO: Make sure this works as expected
	V[(opcode & 0x0F00) >> 8] = dist(eng) & (opcode & 0x0FF);
	pc += 2;
	break;

    case 0xD000: // DXYN: Draws a sprite at coordinate (VX, VY), height N
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
		    if (gfx[(x + xline + ((y + yline) * VIDEO_WIDTH))] == 1)
			V[0xF] = 1;
		    gfx[x + xline + ((y + yline) * VIDEO_WIDTH)] ^= 1;
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

	case 0x00A1: // 0xEXA1: Skips the next instruction if the key in VX isn't pressed
	    if (key[V[(opcode & 0x0F00) >> 8]] == 0)
		pc += 4;
	    else
		pc += 2;
	    break;
	    
	default:
	    printf("Unknown opcode: 0x%X\n", opcode);
	}
	break;

    case 0xF000:
	switch (opcode & 0x00FF)
	{
	case 0x0007: // 0xFX07: Sets VX to the value of delay timer
	    V[(opcode & 0x0F00) >> 8] = delay_timer;
	    pc += 2;
	    break;

	case 0x000A: // 0xFX0A: A key press is awaited and then stored in VX. Blocking operation
	{
	    for (unsigned int i = 0; i < KEY_SIZE; ++i)
	    {
		if (key[i] == 1)
		{
		    V[(opcode & 0x0F00) >> 8] = i;
		    pc += 2;
		}
	    }
	}
	break;

	case 0x0015: // 0xFX15: Set the delay timer to VX
	    DEBUG_PRINT(("Sets the delay timer to : %X\n", V[(opcode & 0x0F00) >> 8]));
	    delay_timer = V[(opcode & 0x0F00) >> 8];
	    pc += 2;
	    break;

	case 0x0018: // 0xFX18: Sets the sound timer to VX
	    sound_timer = V[(opcode & 0x0F00) >> 8];
	    pc += 2;
	    break;

	case 0x001E: // FX1E: Adds VX to I
	    if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF) // Check for overflow
		V[0xF] = 1;
	    else
		V[0xF] = 0;
	    I += V[(opcode & 0x0F00) >> 8];
	    pc += 2;
	    break;

	case 0x0029: // 0xFX29: Sets I to the location of the sprite for the char in VX
	    I = V[(opcode & 0x0F00) >> 8] * 0x5;
	    pc += 2;
	    break;

	case 0x0033: // 0xFX33: Store the binary-coded decimal representation of VX
	    DEBUG_PRINT(("DONE\n"));
	    memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
	    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 100) % 10;
	    memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
	    pc += 2;
	    break;
	    
	case 0x0055: // 0xFX55: Stores V0 to VX, (including VX) in memory starting at addr I
	{
	    for (int i = 0; i < ((opcode & 0x0F00) >> 8); ++i)
		memory[I + i] = V[i];

	    // On the original interpretet, when the operation is done, I = I + X 1
	    I += ((opcode & 0x0F00) >> 8) + 1;
	    pc += 2;
	}
	break;


	case 0x0065: // 0xFX65: Fills V0 to VX with values from memory starting at addr I
	{
	    DEBUG_PRINT(("Done\n"));
	    unsigned short x = (opcode & 0x0F00) >> 8;
	    for (unsigned short i = 0; i <= x; ++i)
		V[i] = memory[I + i];

	    // On the original interpreter, when the operation is done, I = I + X + 1
	    I += ((opcode & 0x0F00) >> 8) + 1;
	    pc += 2;
	}   
	break;

	default:
	    printf("Unknown opcode: 0x%X\n", opcode);
	}
	break;

    default:
	printf("Unknown opcode: 0x%X\n", opcode);
    }

    //DEBUG_PRINT(("Delay timer is : %d\n", delay_timer));
    // Update Timers
    if (delay_timer > 0)	
	--delay_timer;

    if (sound_timer > 0)
    {
	if (sound_timer == 1)
	    DEBUG_PRINT(("BEEP!\n"));
	--sound_timer;
    }

}
