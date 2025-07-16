#pragma once
#include <cstdint>
#include "SDL.h"
#include <unordered_map> 
#include "CPU.h"
#include "constants.h"
#include "Display.h";
class Chip {
public:
	
	
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
	
	bool running;
	bool pause;
	bool loaded;
	int targetNumberInstructionsPerFrame;
	int instructionsPerFrame;
	//Need to define key features
	uint8_t memory[MEMORY_SIZE]{}; //4096 8bit memory addresses
	uint8_t keypad[NUM_KEYS]{}; //16 key input each 8 bit
	uint32_t video[64 * 32]{};
	/*
	Video array replicates the rows and columns
	[1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,0,0,| ....]
	So every 32 elements it is a new row 1 means pixel is on and 0 means pixel is off
	*/
	
	//Constructor
	Chip();
	
	//Load the ROM file into memory
	bool loadROM(char const* filename);

	void speed();
	void resetEngine();
	void runEmulation();
	void printMemory();
	void handleEvent(SDL_Event& event);
	void quit();
	void pauseEmu();
private:
	CPU cpu; //Now CPU is a member varibale of CHIP, separate Class
	void loadFonts();
	void CPUCycle();
	Display display;
	std::unordered_map<SDL_Keycode, uint8_t> keymap = {
	{SDLK_x, 0x0}, {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3},
	{SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_a, 0x7},
	{SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_z, 0xA}, {SDLK_c, 0xB},
	{SDLK_4, 0xC}, {SDLK_r, 0xD}, {SDLK_f, 0xE}, {SDLK_v, 0xF}
	};
	
};