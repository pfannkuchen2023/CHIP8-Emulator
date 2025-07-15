//#pragma once
//class Display {
//
//public:
//	//Constructor
//	Display();
//	//Destructor
//	~Display();
//	//We need to be able to set the size
//	void drawScreen();
//
//
//	//Some new terms learned here:
//	//Constexpr vs const: const not guaranteed to be evaluated at compile time
//	//Static - shared by all objects of this class and belongs to class not individual object so can access without object
//	static constexpr int BASE_HEIGHT = 32;
//	static constexpr int BASE_WIDTH = 64;
//	static constexpr int SCREEN_MULTIPLIER = 15;
//
//private:
//	SDL_Window* window { nullptr };
//	SDL_Renderer* renderer{ nullptr };
//	SDL_Texture* texture{ nullptr };
//	int window_height;
//	int window_width;
//	
//};
