#ifndef CHIP8_H
#define CHIP8_H

#include <string>

class Chip8
{
public:
    void initialize();
    void loadGame(std::string name) {}

    void emulateCycle();
    void setKeys() {}

    bool drawFlag;
    
private:
    unsigned short opcode;
    unsigned char memory[4096];    // Memory in chip-8
    unsigned char V[16];           // CPU registers
    unsigned short I;              // Index register
    unsigned short pc;             // Program counter
    unsigned char gfx[64 * 32];    // The pixels
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned short sp;             // stack pointer
};

#endif
