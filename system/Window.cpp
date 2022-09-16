#ifndef SDL_MAIN_HANDLED
#include "systems.h"
#endif

#define HANDLER
#define AUDIO2_NO_VIRTUAL_AUDIO_CLIENT

// initalize the game with background value
Window::Window()
{
	if(SDL_Init( IMG_INIT_JPG | IMG_INIT_PNG | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) >= 0)
	{
		//set texture filtering to linear
		//sets render quality to where images aren't blurry when resized
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
				
		// create window 
		window = SDL_CreateWindow( "Graphics", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(window != NULL)
		{
			// creates renderer
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
					
			//Initialize renderer color
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF );
					
			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ))
			{
				printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				success = false;
			}
					
			//Initialize text handling 
			if( TTF_Init() == -1 )
			{
				printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
				success = false;
			}
						
					//Initialize music handling 
			if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024*2 ) < 0 )
			{
				printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
				success = false;
			}
		}
	}
	
	// start framekeeper timer for limiting frame 
	framekeeper.start();
};

// clears display and starts frame cap timer 
void Window::clearDisplay()
{
	// clears display 
		SDL_SetRenderDrawColor(renderer, colors[0], colors[1], colors[2], colors[3]);
		SDL_RenderClear(renderer);
		capkeeper.start();
}

// displays renderer to screen and performs frame cap calculations 
void Window::display()
{					
	// displaying images 
		SDL_RenderPresent(renderer);
		++(frames); // adds to frame tally
		
		int framet = capkeeper.getTicks(); // for capping frame rate
		if( framet < SCREEN_TICK_PER_FRAME)
			SDL_Delay( SCREEN_TICK_PER_FRAME - framet );
}	

// ending game, deconstructor  
void Window::close()
{
	//Destroy window and renderer 
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;
			
	// close handlers 
	Mix_Quit();
	IMG_Quit();
		
	// close main SDL extension 
	SDL_Quit();
			
	exit(0);	
}