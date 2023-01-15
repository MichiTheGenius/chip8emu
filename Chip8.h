#include <cstdint>
#include <iostream>
class Chip8
{
    public:
        uint8_t V[16];         // 16 8-bit registers
        uint8_t memory[4096];  // 4096 memory locations
        uint8_t video[64 * 32];// display resolution of chip8 is 64*32
        uint16_t opcode;       // opcode 2 bytes big
        uint16_t I;            // address register
        uint16_t pc;           // program counter
        uint16_t stack[16];    // stack for storing addresses when calling functions
        uint8_t sp;            // stack pointer

        void emulate_cycle();
        void init();
};
