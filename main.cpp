#ifndef HANDLER
#include "include/system/Window.cpp"
#endif

//g++ main.cpp -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -static-libstdc++ -std=c++11 -Wl,-rpath,./ -o main

int main(int argc, char *argv[])
{
	// is the game in debug mode 
	bool debug = true;

	// use time for randomizer seed
	srand((unsigned)time(NULL));
	
	// define Window used for the game 
	Window gameWindow = Window();
	
	
	// switches based on argument if debug = true
	while(true)
		gameWindow.display();
	
	
	return 0;
}