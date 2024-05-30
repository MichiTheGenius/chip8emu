#include "chip8.h"

void Chip8::print_registers()
{
	for (int i = 0; i < 16; i++)
	{
		printf("V[%0x]:0x%02x ", i, V[i]);
	}
}

void Chip8::print_keys()
{
	for (int i = 0; i < 16; i++)
	{
		printf("key[%d]=%d ", i, keypad[i]);
	}
}

void Chip8::init(char const* ROM)
{
	// READ FILE
	FILE *pFile = fopen(ROM, "rb");
	char *buffer;
	// get the file size
	fseek(pFile, 0, SEEK_END);
	long lsize = ftell(pFile);
	rewind(pFile);
	printf("filesize: %d\n", lsize);
	// Allocate memory to contain the whole file
	buffer = (char *)malloc(sizeof(char) * lsize);
	// Copy the file into the buffer
	fread(buffer, 1, lsize, pFile);

	// INITIALIZE VALUES WITH 0
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;
	delay_timer = 0;
	draw_flag = 0;
	cycles = 0;
	// clear_flag = 0;
	for (int i = 0; i < 16; i++)
	{
		V[i] = 0;
		keypad[i] = 0;
	}

	// SET KEYBINDS FOR GERMAN KEYBOARD
	keybinds[0x1] = sf::Keyboard::Num1;
	keybinds[0x2] = sf::Keyboard::Num2;
	keybinds[0x3] = sf::Keyboard::Num3;
	keybinds[0xC] = sf::Keyboard::Num4;

	keybinds[0x4] = sf::Keyboard::Q;
	keybinds[0x5] = sf::Keyboard::W;
	keybinds[0x6] = sf::Keyboard::E;
	keybinds[0xD] = sf::Keyboard::R;

	keybinds[0x7] = sf::Keyboard::A;
	keybinds[0x8] = sf::Keyboard::S;
	keybinds[0x9] = sf::Keyboard::D;
	keybinds[0xE] = sf::Keyboard::F;

	keybinds[0xA] = sf::Keyboard::Y;
	keybinds[0x0] = sf::Keyboard::X;
	keybinds[0xB] = sf::Keyboard::C;
	keybinds[0xF] = sf::Keyboard::V;

	// setup characters for the font reaching from 0 to F
	// each character is 5 bytes long
	// [digit * sizeof_character + offset]

	// '0'
	memory[0x0 * 5 + 0] = 0xF0;
	memory[0x0 * 5 + 1] = 0x90;
	memory[0x0 * 5 + 2] = 0x90;
	memory[0x0 * 5 + 3] = 0x90;
	memory[0x0 * 5 + 4] = 0xF0;

	// '1'
	memory[0x1 * 5 + 0] = 0x20;
	memory[0x1 * 5 + 1] = 0x60;
	memory[0x1 * 5 + 2] = 0x20;
	memory[0x1 * 5 + 3] = 0x20;
	memory[0x1 * 5 + 4] = 0x70;

	// '2'
	memory[0x2 * 5 + 0] = 0xF0;
	memory[0x2 * 5 + 1] = 0x10;
	memory[0x2 * 5 + 2] = 0xF0;
	memory[0x2 * 5 + 3] = 0x80;
	memory[0x2 * 5 + 4] = 0xF0;

	// '3'
	memory[0x3 * 5 + 0] = 0xF0;
	memory[0x3 * 5 + 1] = 0x10;
	memory[0x3 * 5 + 2] = 0xF0;
	memory[0x3 * 5 + 3] = 0x10;
	memory[0x3 * 5 + 4] = 0xF0;

	// '4'
	memory[0x4 * 5 + 0] = 0x90;
	memory[0x4 * 5 + 1] = 0x90;
	memory[0x4 * 5 + 2] = 0xF0;
	memory[0x4 * 5 + 3] = 0x10;
	memory[0x4 * 5 + 4] = 0x10;

	// '5'
	memory[0x5 * 5 + 0] = 0xF0;
	memory[0x5 * 5 + 1] = 0x80;
	memory[0x5 * 5 + 2] = 0xF0;
	memory[0x5 * 5 + 3] = 0x10;
	memory[0x5 * 5 + 4] = 0xF0;

	// '6'
	memory[0x6 * 5 + 0] = 0xF0;
	memory[0x6 * 5 + 1] = 0x80;
	memory[0x6 * 5 + 2] = 0xF0;
	memory[0x6 * 5 + 3] = 0x90;
	memory[0x6 * 5 + 4] = 0xF0;

	// '7'
	memory[0x7 * 5 + 0] = 0xF0;
	memory[0x7 * 5 + 1] = 0x10;
	memory[0x7 * 5 + 2] = 0x20;
	memory[0x7 * 5 + 3] = 0x40;
	memory[0x7 * 5 + 4] = 0x40;

	// '8'
	memory[0x8 * 5 + 0] = 0xF0;
	memory[0x8 * 5 + 1] = 0x90;
	memory[0x8 * 5 + 2] = 0xF0;
	memory[0x8 * 5 + 3] = 0x90;
	memory[0x8 * 5 + 4] = 0xF0;

	// '9'
	memory[0x9 * 5 + 0] = 0xF0;
	memory[0x9 * 5 + 1] = 0x90;
	memory[0x9 * 5 + 2] = 0xF0;
	memory[0x9 * 5 + 3] = 0x10;
	memory[0x9 * 5 + 4] = 0xF0;

	// 'A'
	memory[0xA * 5 + 0] = 0xF0;
	memory[0xA * 5 + 1] = 0x90;
	memory[0xA * 5 + 2] = 0xF0;
	memory[0xA * 5 + 3] = 0x90;
	memory[0xA * 5 + 4] = 0x90;

	// 'B'
	memory[0xB * 5 + 0] = 0xE0;
	memory[0xB * 5 + 1] = 0x90;
	memory[0xB * 5 + 2] = 0xE0;
	memory[0xB * 5 + 3] = 0x90;
	memory[0xB * 5 + 4] = 0xE0;

	// 'C'
	memory[0xC * 5 + 0] = 0xF0;
	memory[0xC * 5 + 1] = 0x80;
	memory[0xC * 5 + 2] = 0x80;
	memory[0xC * 5 + 3] = 0x80;
	memory[0xC * 5 + 4] = 0xF0;

	// 'D'
	memory[0xD * 5 + 0] = 0xE0;
	memory[0xD * 5 + 1] = 0x90;
	memory[0xD * 5 + 2] = 0x90;
	memory[0xD * 5 + 3] = 0x90;
	memory[0xD * 5 + 4] = 0xE0;

	// 'E'
	memory[0xE * 5 + 0] = 0xF0;
	memory[0xE * 5 + 1] = 0x80;
	memory[0xE * 5 + 2] = 0xF0;
	memory[0xE * 5 + 3] = 0x80;
	memory[0xE * 5 + 4] = 0xF0;

	// 'F'
	memory[0xF * 5 + 0] = 0xF0;
	memory[0xF * 5 + 1] = 0x80;
	memory[0xF * 5 + 2] = 0xF0;
	memory[0xF * 5 + 3] = 0x80;
	memory[0xF * 5 + 4] = 0x80;

	// program reading begins at memory position 512 (0x200)
	for (int i = 0; i < lsize; i++)
	{
		memory[i + 512] = buffer[i];
	}

	// memory[0x1FF] = 2; // modify memory to select SUPER-CHIP in the menu directly

	// Close file, free buffer
	fclose(pFile);
	free(buffer);
}

void Chip8::emulate_cycle()
{
	int counter = 0; // counter for FX0A instruction
	// reset the draw_flag every time to 0 at the beginning of the emulate_cycle
	// fetch the instrucion itself -> what does it do?
	opcode =
		memory[pc] << 8 |
		memory[pc + 1]; // combine two bytes from memory for the opcode
						//  memory[pc]   = 0xA2 = 0b10100010
						//  memory[pc+1] = 0xF0 = 0b11110000
						//  "make place" for the second byte by bitshifting the
						//  first one 8 bits to the right 0xA2 << 8 = 0xA200  =
						//  0b1010001000000000 use the bitwise OR "|" operator to
						//  combine the 2 bytes: 0b1010001000000000 (0xA200) |
						//                       0b0000000011110000 (0x00F0)
						//                     = 0b1010001011110000 (0xA2F0) -> combined opcode

	// fetch the values of the instruction -> for example which register do we need?
	u_int8_t X = (opcode & 0x0F00) >> 8;
	u_int8_t Y = (opcode & 0x00F0) >> 4;
	u_int8_t F = 0xF; // carry register for setting the carry flag

	uint8_t NN = opcode & 0x00FF;
	uint8_t N = opcode & 0x000F;
	uint16_t NNN = opcode & 0x0FFF;

	// print the address of the current instruction
	printf("pc:0x%04x ", pc);
	print_registers();
	printf("I:0x%04x ", I, opcode);
	printf("op:0x%04x ", opcode);
	printf("dt:%d ", delay_timer);
	printf("cyc:%d ", cycles);
	// printf("cycle_count:%d ", cycle_count);
	// printf("clear:%d ", clear_flag);
	//  printf("draw:%d ", draw_flag);
	// print_keys();
	printf("\n");
	// clear_flag = 0;
	draw_flag = 0;
	// printf("key: %0x\n", key);

	// decode the opcode -> each instruction has its own opcode
	// extract the first 4 bits of the opcode because they determine the
	// instruction
	switch (opcode & 0xF000)
	{
	case 0x0000:
		// switch (opcode & 0x000F)
		switch (NNN)
		{
		// case 0x0000:
		case 0x00E0:
			// 0x00E0
			// clears the screen
			// execute the instruction -> fetch-fetch-decode-execute-cycle
			draw_flag = 1;
			// clear_flag = 1;
			//  for (int i = 0; i < 64 * 32; i++)
			//{
			//	video[i] = 0;
			//  }
			memset(video, 0, 64 * 32 * sizeof(uint8_t));
			pc += 2;
			break;

		// case 0x000E:
		case 0x00EE:
			// 0x00EE
			// Returns from a subroutine
			// pop the last address from the stack
			sp--;
			pc = stack[sp];
			stack[sp] = 0;
			pc += 2;
			break;
		}
		break;

	case 0x1000:
		// 0x1NNN
		// JMP NNN (address)
		pc = NNN;
		break;

	case 0x2000:
		// 0x2NNN
		// calls subroutine at address NNN
		// store the current pc address on the stack for jumping back
		stack[sp] = pc;
		sp++;

		// jump to the call by setting the pc to the needed address
		pc = NNN;
		break;

	case 0x3000:
		// 0x3XNN
		// Skips the next instruction if VX equals NN
		if (V[X] == NN)
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
		if (V[X] != NN)
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
		V[X] = NN;
		pc += 2;
		break;

	case 0x7000:
		// 0x7XNN
		// Adds NN to VX (carry flag is not changed)
		V[X] += NN;
		pc += 2;
		break;

	case 0x8000:
	{
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
			// Sets VX to VX OR VY. (bitwise OR operation)
			V[X] = (V[X] | V[Y]);
			V[F] = 0;
			pc += 2;
			break;

		case 0x0002:
			// 0x8XY2
			// Sets VX to VX AND VY. (bitwise AND operation)
			V[X] = (V[X] & V[Y]);
			V[F] = 0;
			pc += 2;
			break;

		case 0x0003:
			// 0x8XY3
			// Sets VX to VX XOR VY (bitwise XOR operation)
			V[X] = (V[X] ^ V[Y]);
			V[F] = 0;
			pc += 2;
			break;

		case 0x0004:
			// 0x8XY4
			// Adds VY to VX -> VX += VY
			//
			// if the sum of VX and VY is greater than 255
			// set the carry flag to 1 to inform the system
			// reason: registers are only 8 bits in size -> maximum number is 255
			if (V[Y] > (0xFF - V[X]))
			{
				V[X] += V[Y];
				V[F] = 1;
			}
			else
			{
				V[X] += V[Y];
				V[F] = 0;
			}
			pc += 2;
			break;

		case 0x0005:
			// 0x8XY5
			// subtracts VY from VX -> VX -= VY
			// if the difference of VX and VY is less than 0
			// set the carry flag to 0 to inform the system
			// reason: registers are only 8 bits in size -> numbers are unsigned 8 bit integers -> range from 0 to 255
			// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not
			if (V[Y] > V[X])
			{
				V[X] -= V[Y];
				V[F] = 0;
			}
			else
			{
				V[X] -= V[Y];
				V[F] = 1;
			}
			pc += 2;
			break;

		case 0x0006:
		{
			// 0x8XY6
			// Store the value of register VY shifted right one bit in register VX
			// Set register VF to the least significant bit prior to the shift
			// VY is unchanged
			uint8_t LSB = V[X] & 0x01;
			V[X] = (V[X] >> 1);
			V[F] = LSB;
			pc += 2;
			break;
		}

		case 0x0007:
			// 0x8XY7
			// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not

			V[X] = V[Y] - V[X];
			if (V[Y] > V[X])
			{
				V[F] = 1;
			}
			else
			{
				V[F] = 0;
			}
			pc += 2;
			break;

		case 0x000E:
		{
			// 0x8XYE
			// Store the value of register VY shifted left one bit in register VX
			// Set register VF to the most significant bit prior to the shift
			// VY is unchanged
			uint8_t MSB = (V[X] & 0x80) >> 7;
			V[X] = V[X] << 1;
			V[F] = MSB;
			pc += 2;
			break;
		}
		}
		break;
	}

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
		I = NNN;
		pc += 2;
		break;

	case 0xB000:
		// 0xBNNN
		// Jumps to the address NNN plus V0
		pc = NNN + V[0];
		break;

	case 0xC000:
	{
		// 0xCXNN
		// Sets VX to the result of a bitwise AND operation on a random number (Typically: 0 to 255) and NN
		uint8_t random_number = rand() % 256; // random value between 0 and 255
		V[X] = random_number & NN;
		pc += 2;
		break;
	}

	case 0xD000:
	{
		// 0xDXYN
		// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
		draw_flag = 1;

		// loop through every row/byte of the sprite data -> sprite has N rows
		for (uint8_t yline = 0; yline < N; yline++)
		{
			// sprite data begins at address stored in I register
			uint8_t sprite = memory[I + yline];
			// the row wraps around the screen -> if the sprite goes off screen it appears back at the top of the screen
			// mod dividing by screen height solves this problem
			// e.g if position is 33 (off screen) the row is set to 33%32=1
			uint8_t row = (V[Y] + yline) % 32;

			// loop through every single bit in the byte of sprite data
			// every single column of the sprite -> every sprite is 8 bits wide
			for (uint8_t xline = 0; xline < 8; xline++)
			{
				// the same wrapping logic as seen before for the rows
				uint8_t col = (V[X] + xline) % 64;
				uint16_t offset = row * 64 + col; // convert 2d array indices into 1d array index

				// the most significant bit determines whether a pixel is drawn or not
				// filter out the bit with a logic AND operation
				uint8_t MSB = (sprite & 0x80) >> 7;

				// if the MSB is 1, draw a pixel at the given row and column
				if (MSB == 1)
				{
					if (video[offset] == 1)
					{
						// if a pixel is already present at the given location, set the V[F] register to 1 and turn off the pixel
						// -> used for collision in some applications
						V[F] = 1;
						video[offset] = 0;
					}
					else
					{
						V[F] = 0;
						video[offset] = 1;
					}
				}

				// shift the sprite data 1 bit to the left to get the next bit of information whether to draw somehting or not
				sprite <<= 1;
			}
		}
		pc += 2;
		break;
	}

	case 0xE000:
		switch (opcode & 0x000F)
		{
		case 0x000E:
			// 0xEX9E
			// Skips the next instruction if the key stored in VX is pressed (usually the next instruction is a jump to skip a code block)
			if (keypad[V[X]] == 1)
			{
				pc += 4;
			}
			else
			{
				pc += 2;
			}
			break;

		case 0x0001:
			// 0xEXA1
			// Skips the next instruction if the key stored in VX is not pressed (usually the next instruction is a jump to skip a code block)
			if (keypad[V[X]] == 0)
			{
				pc += 4;
			}
			else
			{
				pc += 2;
			}
			break;
		}
		break;

	case 0xF000:
		switch (opcode & 0x000F)
		{
		case 0x0007:
			// 0xFX07
			// Sets VX to the value of the delay timer
			// write the delay timer
			// get the current time at time of reading the delay timer
			V[X] = delay_timer;
			pc += 2;
			break;

		case 0x000A:
			// 0xFX0A
			//  A key press is awaited, and then stored in VX (blocking operation, all instructions halted until next key event)
			for (int i = 0; i < 16; i++)
			{
				if (keypad[i] == 1)
				{
					V[X] == keypad[counter];
					pc += 2;
					break;
				}
			}
			break;

		case 0x0005:
			switch (opcode & 0x00F0)
			{
			case 0x0010:
				// 0xFX15
				//  Sets the delay timer to VX
				// get the current time at time of writing to the delay timer register
				delay_timer = V[X];
				pc += 2;
				break;

			case 0x0050:
				// 0xFX55
				// Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified
				for (uint8_t i = 0; i <= X; i++)
				{
					printf("moving V[%0x] into memory[%04x + %d]\n", i, I, i);
					memory[I + i] = V[i];
				}
				pc += 2;
				break;

			case 0x0060:
				// 0xFX65
				// Fills from V0 to VX (including VX) with values from memory, starting at address I. The offset from I is increased by 1 for each value read, but I itself is left unmodified
				for (uint8_t i = 0; i <= X; i++)
				{
					V[i] = memory[I + i];
				}
				pc += 2;
				break;
			}

			break;

		case 0x0008:
			// 0xFX18
			// Sets the sound timer to VX
			sound_timer = V[X];
			pc += 2;
			break;

		case 0x000E:
			// 0xFX1E
			// Adds VX to I. VF is not affected
			I += V[X];
			pc += 2;
			break;

		case 0x0009:
			// 0xFX29
			// Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 8x5 font
			// each character 5 bytes -> 16 characters totalling 80 bytes
			I = V[X] * 5;
			pc += 2;
			break;

		case 0x0003:
			// 0xFX33
			// Stores the binary-coded decimal representation of VX, with the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2
			uint8_t temp = V[X];
			memory[I + 2] = temp % 10;
			temp /= 10;
			memory[I + 1] = temp % 10;
			temp /= 10;
			memory[I] = temp % 10;

			pc += 2;
			break;
		}
		break;
	}
	if (delay_timer > 0)
	{
		delay_timer--;
	}
	if(sound_timer > 0)
	{
		// TODO implement sound with SFML
		sound_timer--;
		printf("BEEP BEEP!!!\n");
	}
	// cycles += 1;
	// if (cycles == CPU_CYCLES_PER_TIMER_CYCLE)
	//{
	//	cycles = 0;
	//	if (delay_timer > 0)
	//	{
	//		delay_timer--;
	//	}
	// }
}

void Chip8::get_keys()
{
	for (int i = 0; i < 16; i++)
	{
		if (sf::Keyboard::isKeyPressed(keybinds[i]))
		{
			keypad[i] = 1;
		}
		else
		{
			keypad[i] = 0;
		}
	}
}