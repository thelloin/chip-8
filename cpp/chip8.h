#ifndef CHIP8_H
#define CHIP8_H

#include <string>
#include <random>

class Chip8
{
public:
    Chip8() : eng(), dist(0, 255) {
	
    }

    void initialize();
    bool loadGame(std::string name);

    void emulateCycle();
    void setKeys() {}

    bool drawFlag;

    unsigned char gfx[64 * 32];    // The pixels
    
private:
    unsigned short opcode;
    unsigned char memory[4096];    // Memory in chip-8
    unsigned char V[16];           // CPU registers
    unsigned short I;              // Index register
    unsigned short pc;             // Program counter
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned short sp;             // stack pointer
    unsigned char key[16];

    std::mt19937 eng;
    std::uniform_int_distribution<> dist;
};

#endif
