#include "chip.h"
#include <fstream>
#include <iostream>
#include<vector>



Chip::Chip() : cpu(*this){ //Pass this to CPU
	//Programme counter starts at start of instructions too
	loadFonts();
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
	
	return true;
}

void Chip::loadFonts() {

	for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

}

void Chip::CPUCycle() {
	//SET THE TIMER
	
	double periodSec = 1.0 / BASE_FREQUENCY; //Time in seconds to wait for one frame
	std::chrono::duration<double, std::milli> periodMS(periodSec * 1000); //in ms
	auto nextTime = std::chrono::steady_clock::now() + periodMS; //current time


	//Fetch

	//Decode

	//Execute

	//DelayTimer

	//SoundTimer
}