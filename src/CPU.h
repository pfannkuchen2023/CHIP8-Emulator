#pragma once
#include<cstdint>
#include<random>
#include "constants.h"

class Chip;

class CPU {
public:
	uint8_t delayTimer{}; //8 bit delay timer
	uint8_t soundTimer{}; //8 bit sound timer

	explicit CPU(Chip& parent);
	void CPUInit();
	//fetch, decode, execute
	void fetchDecodeExecuteOpcode();
	void tableSetUp();
	uint8_t getRandomByte() {
		return static_cast<uint8_t>(randByte(randGen));
	}


	//Tables for decoding opcode
	typedef void(CPU::* CPUFunc)();
	CPUFunc table[0xF + 1];
	CPUFunc table0[0xE + 1];
	CPUFunc table8[0xE + 1];
	CPUFunc tableE[0xE + 1];
	CPUFunc tableF[0x65 + 1];

	void Table0();
	void Table8();
	void TableE();
	void TableF();
	void OP_NULL();

	//OPCODES
	void OP_00E0(); //CLS
	void OP_00EE(); //RET
	void OP_1nnn(); //JP ADDR
	void OP_2nnn(); //CALL addr
	void OP_3xkk(); //SE Vx, Byte
	void OP_4xkk(); //SNE Vx, Byte
	void OP_5xy0(); //SE  Vx, Vy
	void OP_6xkk(); //LD Vx, Byte
	void OP_7xkk(); //ADD Vx, byte
	void OP_8xy0(); //LD Vx, Vy
	void OP_8xy1(); //OR Vx, Vy
	void OP_8xy2(); //AND Vx, Vy
	void OP_8xy3(); //XOR Vx, Vy
	void OP_8xy4(); //ADD Vx, Vy
	void OP_8xy5(); //SUB Vx, Vy
	void OP_8xy6(); //SHR VX
	void OP_8xy7(); //SUBN Vx, Vy
	void OP_8xyE(); //SHL Bx {, Vy}
	void OP_9xy0(); //SNE Vx, Vy
	void OP_Annn(); //LD I, addr
	void OP_Bnnn(); //JP V0, addr
	void OP_Cxkk(); //RND Vx, byte
	void OP_Dxyn(); //DRW Vx, Vy, nibble
	void OP_Ex9E(); //SKP Vx
	void OP_ExA1(); //SKNP Vx
	void OP_Fx07(); //LD Vx, DT
	void OP_Fx0A(); //LD Vx, K
	void OP_Fx15(); //LD DT, Vx
	void OP_Fx18(); //LD ST, Vx
	void OP_Fx1E(); //ADD I, Vx
	void OP_Fx29(); //LD F, Vx
	void OP_Fx33(); //LD B, Vx
	void OP_Fx55(); //LD [I], Vx
	void OP_Fx65(); //LD Vx, [I]

	void printToFile(std::string name);

	
private:
	//Create a reference to chip
	Chip& chip;
	std::default_random_engine randGen; //get a random seed
	std::uniform_int_distribution<unsigned int> randByte; //generate random int
	uint8_t registers[REGISTER_SIZE]{}; //16 8bit registers
	uint16_t index{}; // one 16 bit index register
	uint16_t pc{}; //one 16 bit programme counter
	uint16_t stack[STACK_SIZE]{};//16 level stack
	uint8_t sp{}; //8 bit stack pointer
	uint16_t opcode; //instructions
};