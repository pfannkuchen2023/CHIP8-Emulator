
#include <iostream>
#include "constants.h"
#include "chip.h"

int main(int argc, char* argv[]) {
    
    //Pass in the ROM file name
    char const* romFilename = argv[2];
	std::cout << "Loading ROM: " << romFilename << std::endl;

    if (argc > 1) {
        SCREEN_MULTIPLIER = std::atoi(argv[1]);
        if (SCREEN_MULTIPLIER <= 0) {
            std::cerr << "Invalid multiplier, using default 1\n";
            SCREEN_MULTIPLIER = 1;
        }
    }
    else {
        std::cout << "No multiplier provided, using default 1\n";
    }


    Chip chip8;
    if (!chip8.loadROM(romFilename)) {
        return 1;
    }

	chip8.printMemory(); //Print the memory to check if ROM loaded correctly

	chip8.runEmulation();

    std::cout << "SUCCESS" << std::endl;

    return 0;
}