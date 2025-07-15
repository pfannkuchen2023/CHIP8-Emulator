//#include "display.h"
//#include "SDL.h"
//#include <iostream>
//Display::Display() {
//
//	//SET THE DIMENSIONS OF THE SCREEN
//
//	//Note that need to exchagnge 64 by 32 into our screen
//	window_height = BASE_HEIGHT * SCREEN_MULTIPLIER;
//	window_width = BASE_WIDTH * SCREEN_MULTIPLIER;
//	
//	//Init SDL
//	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
//		std::cerr << "SDL failed to initialize: " << SDL_GetError() << std::endl;
//	}
//
//
//	//Create window
//	window = SDL_CreateWindow(
//		"CHIP-8",
//		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//		window_width, window_height,
//		SDL_WINDOW_SHOWN
//	);
//
//	//Creat a renderer
//	//-1 means we use first available rendering driver (default)
//	//0 means no special behaviour
//	renderer = SDL_CreateRenderer(window, -1, 0);
//
//	texture = SDL_CreateTexture(
//		renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, BASE_WIDTH, BASE_HEIGHT);
//
//	if (renderer ==NULL || window == NULL || texture == NULL) {
//		std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
//	}
//
//}
//
//
//Display::~Display() {
//	SDL_DestroyRenderer(renderer);
//	renderer = NULL; //safety net for crashes
//
//	SDL_DestroyWindow(window);
//	window = NULL; //safety net for crashes
//
//	SDL_Quit();
//
//}
//
//void Display::drawScreen() {
//
//
//}