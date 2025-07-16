#include <fstream>
#include <iostream>
#include <cstdint>
#include<vector>
#include<thread>
#include "SDL.h"
#include "chip.h"
Chip::Chip() : cpu(*this){ //Pass reference to self for CPU
	running = true;
	pause = false;
	loaded = false;
	//Programme counter starts at start of instructions too
	loadFonts();
	speed();
	std::cout << "Constructor ran for Chip" << std::endl;
}

//Need to upload the instructions to memory of chip
bool Chip::loadROM(char const* filename) {
	std::cout << "ATTEMPTING TO LOAD FILE...." << std::endl;

	//Open the file as a stream of binary and move the file pointer to the end
	std::ifstream file(filename, std::ios::binary | std::ios::ate);


	//Check if the file is open
	if (!file.is_open()) {
		std::cerr << "Failed to open ROM: " << filename << std::endl;
		return false;
	}
	
	//Ensure not empty or error
	std::streamsize romSize = file.tellg();
	if (romSize <= 0) {
		std::cerr << "ROM Empty or error occured" << std::endl;
		return false;
	}

	//Check the file is not too large
	if (romSize > (MEMORY_SIZE - START_ADDRESS)) {
		std::cerr << "ROM too big for memory!" << std::endl;
		return false;
	}

	//Allocate the buffer and read ROM into it
	std::vector<char> buffer(romSize);
	file.seekg(0, std::ios::beg);

	if (!file.read(buffer.data(), romSize)) {
		std::cerr << "Failed to read ROM file into buffer" << std::endl;
		return false;
	}

	//Now load buffer into memory starting at 0x200
	for (std::streamsize i = 0; i < romSize; ++i) {
		memory[START_ADDRESS + i] = static_cast<uint8_t>(buffer[i]);
	}
	std::cout << "READ ROM SUCCESS" << std::endl;
	resetEngine(); //Reset the CPU and set PC to start address
	return true;
}

void Chip::loadFonts() {

	for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}
	std::cout << "Loaded Fonts into memory" << std::endl;
}

void Chip::CPUCycle() {
	//SET THE TIMER
	double periodSec = 1.0 / BASE_FREQUENCY; //Time in seconds to wait for one frame
	std::chrono::duration<double, std::milli> periodMS(periodSec * 1000); //in ms
	auto nextTime = std::chrono::steady_clock::now() + periodMS; //current time

	std::ofstream outfile("output.txt", std::ios::out | std::ios::app);
	while (running) {
		SDL_Event event;
		handleEvent(event);
	
		//If not paused and ROM loaded, then we decrease timers if non-zero
		if (pause == false && loaded == true) {
			if (cpu.delayTimer > 0) {
				cpu.delayTimer--;
			}	
			if (cpu.soundTimer > 0) {
				cpu.soundTimer--;
			}
			for(int i = 0; i < instructionsPerFrame; ++i) {
				cpu.fetchDecodeExecuteOpcode(); //Fetch, decode and execute the opcode
			}	
		}
		int videoPitch = sizeof(video[0]) * VIDEO_WIDTH;
		
		for (int i = 0; i < (sizeof(video) /32); i++) {
			for (int j = 0; j < 32; j++) {
				std::cout << "i: " << i << " j: " << j << std::endl;
				outfile << video[i * 32 + j];
			}
			outfile << "\n";
		}

		//DISPLAY THE RESULTS ON THE SCREEN
		display.update(video, videoPitch);
		//SLEEP
		std::this_thread::sleep_until(nextTime); //Sleep until the next frame
		nextTime += periodMS; //Update the next time to be the next frame
	}
	outfile.close();
}

//Calculate the number of instructions per frame based on the base frequency
void Chip::speed() {
	instructionsPerFrame = INSTRUCTIONS_PER_SECOND / BASE_FREQUENCY;
	std::cout << "Instructions per frame set at: " << instructionsPerFrame << std::endl;
}

void Chip::resetEngine() {
	cpu.CPUInit(); //Reset the CPU
	std::cout << "RESETTING CHIP8 ENGINE" << std::endl;
}

void Chip::runEmulation() {
	std::cout << "RUNNING EMULATION" << std::endl;
	CPUCycle();
}

void Chip::printMemory() {


	for (unsigned int i = 0; i < MEMORY_SIZE; ++i) {
		std::cout << std::hex << static_cast<int>(memory[i]) << " ";
		if ((i + 1) % 16 == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << std::dec; //Reset to decimal output
}

void Chip::handleEvent(SDL_Event &event) {
	
	while (SDL_PollEvent(&event)) {
		std::cout << "AN EVENT" << std::endl;
		//CHECK FOR QUIT
		if (event.type == SDL_QUIT) {
			std::cout << "quit" << std::endl;
			quit();
		}
		//Re-sizing the window
		else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
			std::cout << "resize" << std::endl;
		}
		else if (event.type == SDL_KEYDOWN) {
			//QUIT THE EMU
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				std::cout << "quit" << std::endl;
				quit();
			}
			//Pause the EMU
			else if (event.key.keysym.sym == SDLK_SPACE) {
				std::cout << "pause" << std::endl;
				pauseEmu();
			}
			else {
				auto it = keymap.find(event.key.keysym.sym);
				if (it != keymap.end()) {
					keypad[it->second] = 1;
				}
			}
		}
		else if (event.type == SDL_KEYUP){
			auto it = keymap.find(event.key.keysym.sym);
			if (it != keymap.end()) {
				keypad[it->second] = 0;
			}
		}
	}

}

void Chip::quit() {
	running = false;
	std::cout << "Quitting Emulator" << std::endl;
}

void Chip::pauseEmu() {
	if (pause) { pause = false; }
	else { pause = true; }
	std::cout << "PAUSE TOGGLED TO: " << pause << std::endl;
}


