#pragma once
constexpr unsigned int MEMORY_SIZE = 4096;
constexpr unsigned int FONTSET_START_ADDRESS = 0x050;
constexpr unsigned int START_ADDRESS = 0x200;
constexpr unsigned int FONTSET_SIZE = 80;
constexpr unsigned int REGISTER_SIZE = 16;
constexpr unsigned int STACK_SIZE = 16;
constexpr unsigned int NUM_KEYS = 16;
constexpr unsigned int BASE_FREQUENCY = 60;
constexpr unsigned int VIDEO_HEIGHT = 32;
constexpr unsigned int VIDEO_WIDTH = 64;
constexpr unsigned int INSTRUCTIONS_PER_SECOND = 560;
extern unsigned int SCREEN_MULTIPLIER;