#include <iostream>
#include<random>
#include<chrono>

#include "CPU.h"
#include"chip.h"
//Constructor
CPU::CPU(Chip& parent) : chip(parent), randGen(std::chrono::system_clock::now().time_since_epoch().count()), randByte(0, 255) { // Constructor seeds random generator and initializes uniform distribution [0, 255]
	CPUInit();
	std::cout << "CPU Constructor ran" << std::endl;
}

//Initialize the CPU
void CPU::CPUInit() {
	tableSetUp();
	//Reset stack
	pc = START_ADDRESS;
	sp = 0;
	memset(stack, 0, sizeof(stack));

	//Set memory pointer
	index = 0;
	memset(registers, 0, sizeof(registers)); //clear registers

	//Set timers
	delayTimer = 0;
	soundTimer = 0;
	std::cout << "CPU Initialized" << std::endl;
}

//Fetch the opcode
void CPU::fetchDecodeExecuteOpcode() {
	opcode = (chip.memory[pc] << 8u | chip.memory[pc + 1]); //grab two bytes and combine
	//Increase the pc by 2
	pc += 2;
	(this->*(table[(opcode & 0xF000u) >> 12u]))();
}

//Decode and execute Opcode
void CPU::tableSetUp() {
	//index 0x0 points to the function Table0
	table[0x0] = &CPU::Table0;
	//index 0x1 of table points to function OP_1nnn etc.
	table[0x1] = &CPU::OP_1nnn;
	table[0x2] = &CPU::OP_2nnn;
	table[0x3] = &CPU::OP_3xkk;
	table[0x4] = &CPU::OP_4xkk;
	table[0x5] = &CPU::OP_5xy0;
	table[0x6] = &CPU::OP_6xkk;
	table[0x7] = &CPU::OP_7xkk;
	table[0x8] = &CPU::Table8;
	table[0x9] = &CPU::OP_9xy0;
	table[0xA] = &CPU::OP_Annn;
	table[0xB] = &CPU::OP_Bnnn;
	table[0xC] = &CPU::OP_Cxkk;
	table[0xD] = &CPU::OP_Dxyn;
	table[0xE] = &CPU::TableE;
	table[0xF] = &CPU::TableF;

	//Then we point all elements in secondary tables to OP_NULL function
	for (size_t i = 0; i <= 0xE; i++)
	{
		table0[i] = &CPU::OP_NULL;
		table8[i] = &CPU::OP_NULL;
		tableE[i] = &CPU::OP_NULL;
	}
	//But overwrite existing opcodes with correct opcode functions
	table0[0x0] = &CPU::OP_00E0;
	table0[0xE] = &CPU::OP_00EE;

	table8[0x0] = &CPU::OP_8xy0;
	table8[0x1] = &CPU::OP_8xy1;
	table8[0x2] = &CPU::OP_8xy2;
	table8[0x3] = &CPU::OP_8xy3;
	table8[0x4] = &CPU::OP_8xy4;
	table8[0x5] = &CPU::OP_8xy5;
	table8[0x6] = &CPU::OP_8xy6;
	table8[0x7] = &CPU::OP_8xy7;
	table8[0xE] = &CPU::OP_8xyE;

	tableE[0x1] = &CPU::OP_ExA1;
	tableE[0xE] = &CPU::OP_Ex9E;
	//Repeat for 0xF which requires larger table array
	for (size_t i = 0; i <= 0x65; i++)
	{
		tableF[i] = &CPU::OP_NULL;
	}

	tableF[0x07] = &CPU::OP_Fx07;
	tableF[0x0A] = &CPU::OP_Fx0A;
	tableF[0x15] = &CPU::OP_Fx15;
	tableF[0x18] = &CPU::OP_Fx18;
	tableF[0x1E] = &CPU::OP_Fx1E;
	tableF[0x29] = &CPU::OP_Fx29;
	tableF[0x33] = &CPU::OP_Fx33;
	tableF[0x55] = &CPU::OP_Fx55;
	tableF[0x65] = &CPU::OP_Fx65;
	std::cout << "CPU Tables set up" << std::endl;
}

//TABLE RUN FUNCTIONS
void CPU::Table0() {
	//Table 0 will store pointers to member functions of Op code of pattern 0x
	//It will then run opcode function stored at the relevent position

	//Pass through the last digit e.g. 0x0AB3 pass through 0x3
	(this->*(table0[opcode & 0x000Fu]))();
}

void CPU::Table8() {
	//We access index e.g. 0x8AB3 -> 0x3 of table8 which links to function CPU::OP_8xy3
	(this->*(table8[opcode & 0x000Fu]))();
}

void CPU::TableE() {
	(this->*(tableE[opcode & 0x000Fu]))();
}

void CPU::TableF() {
	(this->*(tableF[opcode & 0x00FFu]))();
}

void CPU::OP_NULL() {
	std::cout << "Missing OPCODE: " << opcode<< std::endl;
}

//Clear the display
void CPU::OP_00E0() {
	//Set all the values in video array to 0
	memset(chip.video, 0, sizeof(chip.video)); 
	//This will point to address of start of video and set all the memory addresses in this array to 0
}

//Return from a subroutine
void CPU::OP_00EE() {
	/*Top of stack has address of one instruction past one that called subroutine
	Put this back into PC*/
	--sp;
	pc = stack[sp];
}

//Jump to location nnn and set PC to nnn
void CPU::OP_1nnn() {
	//Jump does not remember origin so no stack interaction needed
	uint16_t address = opcode & 0x0FFFu;
	pc = address;

}
//Call subroutine at nnn
void CPU::OP_2nnn() {
	/*We want to return eventuallyt so we put the current PC onto top of stack
	We did pc+=2 in cycle() so current PC holds next instruction after CALL which is correct
	Do not want to return to CALL instruction as it would be infinite loop*/
	uint16_t address = opcode & 0x0FFFu;
	stack[sp] = pc;
	++sp;
	pc = address;
}

//Skip next instruction if Vx == kk
void CPU::OP_3xkk() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;
	if (registers[Vx] == byte){
		pc += 2;
	}
}

//Skip next instruction if Vx != kk
void CPU::OP_4xkk() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;
	if (registers[Vx] != byte) {
		pc += 2;
	}
}

//Skip next instruction if Vx = Vy
void CPU::OP_5xy0() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	if (registers[Vx] == registers[Vy]) {
		pc += 2;
	}

}

//Set Vx == kk
void CPU::OP_6xkk() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = byte;
}

//Set Vx = Vx + kk
void CPU::OP_7xkk() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] += byte;
}

//Set Vx = Vy
void CPU::OP_8xy0() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	registers[Vx] = registers[Vy];
}

//Set Vx = Vx or Vy
void CPU::OP_8xy1() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	registers[Vx] |= registers[Vy];

}

//Set Vx = Vx and Vy
void CPU::OP_8xy2() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	registers[Vx] &= registers[Vy];
}

//Set Vx = Vx XOR Vy
void CPU::OP_8xy3() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	registers[Vx] ^= registers[Vy];
}

//Set Vx = Vx + Vy , set Vf = Carry
void CPU::OP_8xy4() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint16_t sum = registers[Vx] + registers[Vy];
	if (sum > 255) {
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}
	registers[Vx] = sum & 0xFFu;
}

//Set Vx = Vx - Vy, set VF to NOT Borrow
//If Vx > Vy then VF set to 1 else 0 and Vy substracted from Vx and result stored in Vx
void CPU::OP_8xy5() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] > registers[Vy]) {
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}
	registers[Vx] -= registers[Vy];

}

//Set Vx = Vx SHR 1
//If least significant bit of Vx is 1, VF set to 1 else 0
//Then Vx divided by 2
void CPU::OP_8xy6() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	registers[0xF] = (registers[Vx] & 0x1u);
	registers[Vx] >>= 1;
}

//Set Vx = Vy - Vx set VF to Not Borrow
//Vy > Vx then VF set to 1 else 0 and Vx subbed from Vy
//Results stored in Vx
void CPU::OP_8xy7() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] < registers[Vy]) {

		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}
	registers[Vx] = registers[Vy] - registers[Vx];

}

//Set Vx = Vx SHL 1
//If most sig bit of Vx is 1 then VF is 1 else 0
//Vx then multiplied by 2 (left shift)
void CPU::OP_8xyE() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	//Save MSB in VF
	registers[0xF] = (registers[Vx] & 0x80u) >> 7u;
	registers[Vx] <<= 1;
}

//Skip the next instruction if Vx != Vy
void CPU::OP_9xy0() {
	/*PC already incremented by 2 in Cylce() so increment by 2 again to skip instruction*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] != registers[Vy]) {
		pc += 2;
	}
}

//Set I = nnn
void CPU::OP_Annn() {
	uint16_t address = opcode & 0X0FFFu;
	index = address;
}

//Jump to location nnn + V0
void CPU::OP_Bnnn() {
	uint16_t address = opcode & 0x0FFFu;
	pc = registers[0] + address;
}

//Set Vx = random byte and kk
void CPU::OP_Cxkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;
	registers[Vx] = getRandomByte() & byte;
}

//Display n-byute sprite starting at memory location I at (Vx, Vy), set VF = collision
void CPU::OP_Dxyn() {
	/*Iterate over sprite - 8 columns because a sprite is 8 pixels wide
	If sprite pixel on there may be a collision with what is already being displayed so check if out screen pixel in same location is set
	If it is then VF register to express collision
	
	Just XOR screen pixel with 0xFFFFFFFF to XOR it with sprite pixel (on)*/

	uint8_t Vx = (opcode & 0x0F00u) << 8u;
	uint8_t Vy = (opcode & 0x00F0u) << 4u;
	uint8_t height = opcode & 0x000Fu;

	//Ensure that we do not exceed screen boundaries
	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

	//Set VF to 0
	registers[0xF] = 0;

	//Loop over all 
	for (unsigned int row = 0; row < height; ++row) {
		uint8_t spriteByte = chip.memory[index + row];

		for (unsigned int col = 0; col < 8; ++col) {
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			//Point to memory address of the index in video array at  (yPos + row) * chip.VIDEO_WIDTH + (xPos + col)
			uint32_t* screenPixel = &chip.video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];
		
			//Is the sprite pixel on at this point
			if (spritePixel) {
				//Is the screen pixel also on?
				if (*screenPixel == 0xFFFFFFFF) {
					registers[0xF] = 1; //set VF to 1 if collision
				}
				//NOW XOR WITH THE SPRITE PIXEL WHICH IS ON
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

//Skip next instruction if key with the value of Vx is pressed
void CPU::OP_Ex9E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t key = registers[Vx];
	if (chip.keypad[key]) 
	{
		pc += 2;
	}
}

//Skip next instruction if key with value Vx not pressed
void CPU::OP_ExA1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t key = registers[Vx];
	if (!chip.keypad[key])
	{
		pc += 2;
	}
}

//Set Vx = delay timer value
void CPU::OP_Fx07() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	registers[Vx] = delayTimer;
}

//Wait for a key press, store the value in Vx
void CPU::OP_Fx0A() {
	bool keyFound = false;
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	for (uint8_t i = 0; i < 16; ++i) {
		if (chip.keypad[i]) {
			registers[Vx] = i;
			keyFound = true;
			break;
		}
	}

	//If key not pressed we decrement PC by 2 - same effect as running same instruction again
	if (!keyFound) {
		pc -= 2;
	}
}

//Set delay timer to Vx
void CPU::OP_Fx15() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	delayTimer = registers[Vx];
}

//Set sound timer to Vx
void CPU::OP_Fx18() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	soundTimer = registers[Vx];
}

//Set I = I + Vx
void CPU::OP_Fx1E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	index += registers[Vx];
}

//Set I = location of sprite for digit Vx
void CPU::OP_Fx29() {
	/*Font characters start at 0x50 and 5 bytes each, so can get address of first byte of any character by taking offset from start address*/
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = registers[Vx];
	index = FONTSET_START_ADDRESS + (5 * digit);
}

//Store BCD representation of Vx in memory location I, I+1, I+2
//Interpreter takes decimal value of Vx and places hundreds digit in memory at location in I, the tens digit at location I+1 and the ones digit at location I+2
void CPU::OP_Fx33() {
	uint8_t Vx = (opcode & 0x0F00u) << 8u;
	uint8_t value = registers[Vx];
	//Need to get the tens, hundreds and ones
	chip.memory[index + 2] = value % 10; //ones
	value /= 10; //integer division by 10

	//Tens place
	chip.memory[index + 1] = value % 10;
	value /= 10;

	//Hundreds
	chip.memory[index] = value & 10;
}

//Store registers V0 through Cx in memory starting at location I
void CPU::OP_Fx55() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	for (uint8_t i = 0; i <= Vx; ++i) {
		chip.memory[index + i] = registers[i];
	}
}

//Read registers V0 through Vx from memory starting at I
void CPU::OP_Fx65() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	for (uint8_t i = 0; i <= Vx; ++i) {
		registers[i] = chip.memory[index + i];
	}
}


