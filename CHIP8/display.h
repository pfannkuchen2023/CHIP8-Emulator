#pragma once
#include "SDL.h"
#include "constants.h"
class Chip;

class Display {

public:
	//Constructor
	Display();
	//Destructor
	~Display();
	//We need to be able to set the size
	void update(void const* buffer, int pitch);
	void calculateRes();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	//SDL_Rect drawRect;

	int windowHeight;
	int windowWidth;
	
	
};
