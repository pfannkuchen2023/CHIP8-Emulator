#pragma once
#include <cstdint>
#include <random>
//Use the system clock as the seed for random generator
#include<chrono>

class Chip {
public:
	
	static constexpr unsigned int MEMORY_SIZE = 4096;
	static constexpr unsigned int FONTSET_START_ADDRESS = 0x050;
	static constexpr unsigned int START_ADDRESS = 0x200;
	static constexpr unsigned int FONTSET_SIZE = 80;
	static constexpr unsigned int REGISTER_SIZE = 16;
	static constexpr unsigned int STACK_SIZE = 16;
	static constexpr unsigned int NUM_KEYS = 16;
	static constexpr unsigned int BASE_FREQUENCY = 60;
	uint8_t delayTimer{}; //8 bit delay timer
	uint8_t soundTimer{}; //8 bit sound timer
	uint8_t fontset[FONTSET_SIZE] =
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

	//Need to define key features
	uint8_t memory[MEMORY_SIZE]{}; //4096 8bit memory addresses
	uint8_t keypad[NUM_KEYS]{}; //16 key input each 8 bit
	uint32_t video[64 * 32]{};
	static constexpr unsigned int VIDEO_HEIGHT = 32;
	static constexpr unsigned int VIDEO_WIDTH = 64;

	/*
	Video array replicates the rows and columns
	[1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,0,0,| ....]
	So every 32 elements it is a new row 1 means pixel is on and 0 means pixel is off
	*/

	//Constructor
	Chip();

	//Load the ROM file into memory
	bool loadROM(char const* filename);


private:
	void loadFonts();
	void CPUCycle();

	//Create a CPU class that belongs to chip
	class CPU {
	public:
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
		CPU(Chip& parent); //constructor
		
		
		uint8_t getRandomByte() {
			return static_cast<uint8_t>(randByte(randGen));
		}
		
		//fetch, decode, execute
		uint16_t fetchOpcode();
		void decodeOpcode(const uint16_t &opcode);
		void executeOpcode();

		
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
	};


	CPU cpu;

};