#ifndef CHIP8_H
#define CHIP8_H

#include <string>
#include <random>

static const unsigned int VIDEO_WIDTH = 64;
static const unsigned int VIDEO_HEIGHT = 32;
static const unsigned int KEY_SIZE = 0x0F;
static const unsigned int MEMORY_SIZE = 4096;
static const unsigned int VREG_SIZE = 0x0F;
static const unsigned int STACK_SIZE = 0x0F;
static const unsigned int PROGRAM_START = 0x200;


class Chip8
{
public:
    Chip8() : eng(), dist(0, 255) {
	
    }

    void initialize();
    bool loadGame(std::string name);

    void emulateCycle();
    void pressKey(const uint8_t);
    void releaseKey(const uint8_t);
    void setKeys() {} // Can probably delete this eventually

    bool drawFlag;

    unsigned char gfx[VIDEO_WIDTH * VIDEO_HEIGHT];    // The pixels
    unsigned char key[KEY_SIZE];
    
private:
    unsigned short opcode;
    unsigned char memory[MEMORY_SIZE];    // Memory in chip-8
    unsigned char V[VREG_SIZE];           // CPU registers
    unsigned short I;              // Index register
    unsigned short pc;             // Program counter
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[STACK_SIZE];
    unsigned short sp;             // stack pointer
    

    std::mt19937 eng;
    std::uniform_int_distribution<> dist;
};

#endif
