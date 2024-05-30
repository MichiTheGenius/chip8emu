#include <cstdint>
#include <iostream>
#include <random>
#include <unistd.h>
#include <cstdint>
#include <cstdio>
#include <string.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#define CLOCK_SPEED 600 // Instructions per second
#define CPU_CYCLES_PER_TIMER_CYCLE CLOCK_SPEED / 60

class Chip8
{
public:
    uint8_t V[16];          // 16 8-bit registers
    uint8_t memory[4096];   // 4096 memory locations
    uint8_t video[64 * 32]; // display resolution of chip8 is 64*32
    uint16_t opcode;        // opcode 2 bytes big
    uint16_t I;             // address register
    uint16_t pc;            // program counter
    uint16_t stack[16];     // stack for storing addresses when calling functions
    uint8_t sp;             // stack pointer
    uint8_t delay_timer;    // delay timer
    uint8_t sound_timer;    // sound timer
    uint8_t draw_flag;      // boolean flag if something needs to be redrawn
    //uint8_t clear_flag;     // boolean flag if screen needs to be cleared
    uint8_t keypad[16];       // hex keypad with 16 keys
    sf::Keyboard::Key keybinds[16]; 
    uint8_t cycles;

    void print_registers();
    void print_keys();
    void emulate_cycle();
    void init(char const* ROM);
    void get_keys();
};
