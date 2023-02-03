#include "Chip8.h"

void Chip8::init()
{
    FILE *pFile = fopen("invaders.c8", "rb");
    char *buffer;
    // Check file size
    fseek(pFile, 0, SEEK_END);
    long lsize = ftell(pFile);
    rewind(pFile);
    printf("filesize: %d\n", lsize);
    // Allocate memory to contain the whole file
    buffer = (char *)malloc(sizeof(char) * lsize);
    // Copy the file into the buffer
    fread(buffer, 1, lsize, pFile);

    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    // program reading begins at memory position 512 (0x200)
    for (int i = 0; i < lsize; i++)
    {
        memory[i + 512] = buffer[i];
    }

    // Close file, free buffer
    fclose(pFile);
    free(buffer);
    while (pc < lsize)
    {
        emulate_cycle();
    }
}

void Chip8::emulate_cycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1]; // combine two bytes from memory for the opcode
                                               //  memory[pc]   = 0xA2 = 0b10100010
                                               //  memory[pc+1] = 0xF0 = 0b11110000
                                               //  "make place" for the second byte by bitshifting the first one 8 bits to the right
                                               //  0xA2 << 8 = 0xA200  = 0b1010001000000000
                                               //  use the bitwise OR "|" operator to combine the 2 bytes
                                               //  0b1010001000000000 (0xA200) |
                                               //  0b0000000011110000 (0x00F0)
                                               //= 0b1010001011110000 (0xA2F0) -> combined opcode

    printf("addr:0x%04x op:0x%04x ", pc, opcode);
    // print the address of the current instruction

    u_int8_t X = (opcode & 0x0F00) >> 8;
    u_int8_t Y = (opcode & 0x00F0) >> 4;
    u_int8_t F = 0xF; // carry register for setting the carry flag

    // extract the first 4 bits of the opcode because they determine the instruction
    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode & 0x000F)
        {
        case 0x0000:
            // 0x00E0
            // clears the screen
            for (int i = 0; i < 64 * 32; i++)
            {
                video[i] = 0;
            }
            pc += 2;
            break;

        case 0x000E:
            // 0x00EE
            // Returns from a subroutine
            // pop the last address from the stack
            sp--;
            pc = stack[sp];
            break;
        }
        break;

    case 0x1000:
        // 0x1NNN
        // JMP NNN (address)
        uint16_t NNN = opcode & 0x0FFF;
        pc = NNN;
        break;

    case 0x2000:
        // 0x2NNN
        // calls subroutine at address NNN
        // store the current pc address on the stack for jumping back
        stack[sp] = pc;
        sp++;

        // jump to the call by setting the pc to the needed address
        uint16_t NNN = opcode & 0x0FFF;
        pc = NNN;
        break;

    case 0x3000:
        // 0x3XNN
        // Skips the next instruction if VX equals NN
        if (V[X] == (opcode & 0x00FF))
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;

    case 0x4000:
        // 0x4XNN
        // Skips the next instruction if VX does not equal NN
        if (V[X] != (opcode & 0x00FF))
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;

    case 0x5000:
        // 0x5XY0
        // Skips the next instruction if VX equals VY
        if (V[X] == V[Y])
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;

    case 0x6000:
        // 0x6XNN
        // Sets VX to NN -> VX = NN
        V[X] = opcode & 0x00FF;
        break;

    case 0x7000:
        // 0x7XNN
        // Adds NN to VX (carry flag is not changed)
        V[X] += opcode & 0x00FF;
        break;

    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000:
            // 0x8XY0
            // sets VX to value of VY -> VX = VY
            V[X] = V[Y];
            pc += 2;
            break;

        case 0x0001:
            // 0x8XY1
            // Sets VX to VX or VY. (bitwise OR operation)
            V[X] | V[Y];
            break;

        case 0x0002:
            // 0x8XY2
            // Sets VX to VX and VY. (bitwise AND operation)
            V[X] & V[Y];
            break;

        case 0x0004:
            // 0x8XY4
            // Adds VY to VX -> VX += VY
            //
            // if the sum of register x and y is greater than 255
            // we set the carry flag to 1 to let the system know
            // reason: registers are only 8 bits big -> maximum number is 255
            // if (V[Y] > (0xFF - V[X]))
            // {
            //     V[F] = 1;
            // }
            if (V[X] + V[Y] > 0xFF)
            {
                V[F] = 1;
            }
            else
            {
                V[F] = 0;
            }
            V[X] += V[Y];
            pc += 2;
            break;

        case 0x0005:
            // 0x8XY5
            // subtracts VY from VX -> VX -= VY
            // if the difference of register x and y is less than 0
            // we set the carry flag to 1 to let the system know
            // reason: registers are only 8 bits big -> numbers are unsigned and 8 bit -> range from 0 to 255
            if (V[Y] > V[X])
            {
                V[F] = 1;
            }
            else
            {
                V[F] = 0;
            }
            V[X] -= V[Y];
            pc += 2;
            break;
        }
        break;

    case 0x9000:
        // 0x9XY0
        // Skips the next instruction if VX does not equal VY
        if (V[X] != V[Y])
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;

    case 0xA000:
        // 0xANNN
        // Sets I to the address NNN
        I = opcode & 0x0FFF;
        pc += 2;
        break;

    case 0xB000:
        // 0xBNNN
        // Jumps to the address NNN plus V0
        uint16_t NNN = opcode & 0x0FFF;
        pc = (opcode & NNN) + V[0];
        break;

    case 0xC000:
        // 0xCXNN
        // Sets VX to the result of a bitwise AND operation on a random number (Typically: 0 to 255) and NN
        uint8_t NN = opcode & 0x00FF;
        uint8_t random_number = rand() % 256;
        V[X] = random_number & NN;
        break;

    case 0xE000:
        switch (opcode & 0x000F)
        {
        case 0x000E:
            // 0xEX9E
            // TODO: Skips the next instruction if the key stored in VX is pressed (usually the next instruction is a jump to skip a code block)
            break;

        case 0x0001:
            // 0xEXA1
            // TODO: Skips the next instruction if the key stored in VX is not pressed (usually the next instruction is a jump to skip a code block)
            break;
        }
        break;

    case 0xF000:
        switch (opcode & 0x000F)
        {
        case 0x0007:
            // 0xFX07
            // TODO: Sets VX to the value of the delay timer
            break;

        case 0x000A:
            // 0xFX0A
            // TODO: A key press is awaited, and then stored in VX (blocking operation, all instruction halted until next key event)
            break;

        case 0x0005:
            switch (opcode & 0x00F0)
            {
            case 0x0010:
                // 0xFX15
                // TODO: Sets the delay timer to VX
                break;

            case 0x0050:
                // 0xFX55
                // TODO: Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified
                break;

            case 0x0060:
                // 0xFX65
                // TODO: Fills from V0 to VX (including VX) with values from memory, starting at address I. The offset from I is increased by 1 for each value read, but I itself is left unmodified
                break;
            }

            break;

        case 0x0008:
            // 0xFX18
            // TODO: Sets the sound timer to VX
            break;

        case 0x000E:
            // 0xFX1E
            // Adds VX to I. VF is not affected
            I += V[X];
            break;

        case 0x0009:
            // 0xFX29
            // TODO: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
            break;

        case 0x0003:
            // 0xFX33
            // TODO: Stores the binary-coded decimal representation of VX, with the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2
            break;
        }
        break;
    }

    printf("\n");
}
