#include "display.h"
#include "SDL.h"
#include <iostream>
Display::Display() {

	//SET THE DIMENSIONS OF THE SCREEN

	//Note that need to exchagnge 64 by 32 into our screen
	windowHeight = VIDEO_HEIGHT * SCREEN_MULTIPLIER;
	windowWidth = VIDEO_WIDTH * SCREEN_MULTIPLIER;
	calculateRes();
	//Init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
		std::cerr << "SDL failed to initialize: " << SDL_GetError() << std::endl;
	}


	//Create window
	window = SDL_CreateWindow(
		"CHIP-8",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
	);

	//Creat a renderer
	//-1 means we use first available rendering driver (default)
	//0 means no special behaviour
	renderer = SDL_CreateRenderer(window, -1, 0);

	texture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);

	//Check everything is created properly
	if (renderer ==NULL || window == NULL || texture == NULL) {
		std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
	}

}


Display::~Display() {
	SDL_DestroyRenderer(renderer);
	renderer = NULL; //safety net for crashes

	SDL_DestroyTexture(texture);
	texture = NULL; //safety net for crashes

	SDL_DestroyWindow(window);
	window = NULL; //safety net for crashes

	SDL_Quit();

}

void Display::calculateRes() {
	//Get current window size
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	//Smallest integeter scale that can fit within the window
	int scale = std::min(windowWidth / VIDEO_WIDTH, windowHeight / VIDEO_HEIGHT);

	//Set renderWidth
	int renderWidth = VIDEO_WIDTH * scale;
	//Set renderHeight
	int renderHeight = VIDEO_HEIGHT * scale;

	////Calculate teh rectangle position to be centered
	//drawRect.x = (windowWidth - renderWidth) / 2;
	//drawRect.y = (windowHeight - renderHeight) / 2;
	//drawRect.w = renderWidth;
	//drawRect.h = renderHeight;
}

void Display::update(void const* buffer, int pitch) {
	SDL_UpdateTexture(texture, nullptr, buffer, pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}