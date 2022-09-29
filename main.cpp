#include "include/battle_system.h"

//g++ main.cpp -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -static-libstdc++ -std=c++11 -Wl,-rpath,./ -o main

enum gameSystemMode
{
	TITLE_SCREEN=1,
	BATTLE=2,
	DUNGEON=3,
	MENU_SELECTION=4
};

// class for handling background window animation
class backgroundHandler
{
	public:	
		backgroundHandler();
		backgroundHandler(Window * r, gameSystemMode m)
		{
			backgroundMode = m;
			gameWindow = r;
		
			switch(backgroundMode)
			{
				case BATTLE:
				background_assets = new texture[3];
				background_assets[0] = texture("resources/sprites/background/2_background_asset.png",gameWindow->renderer);
				background_assets[1] = texture("resources/sprites/background/2_background_asset_2.png",gameWindow->renderer);
				background_assets[2] = texture("resources/sprites/background/theslayer.png",gameWindow->renderer);
				
				background_assets[1].scale = 3;
				background_assets[0].scale = 3;	
				background_assets[2].scale = 3;	
		
				break;
			}
		}
		
		void display()
		{
			SDL_SetRenderDrawColor(gameWindow->renderer, colors[0], colors[1], colors[2], colors[3]);
			
			gameWindow->colors[0] = 61;
			gameWindow->colors[1] = 0;
			gameWindow->colors[2] = 92;
			
			for(int i=0;i<6;i++)
			{
				background_assets[2].render(gameWindow->renderer,i*180,50);
				background_assets[2].render(gameWindow->renderer,i*180,300);
			}
		
			background_assets[2].angle = ((((int)background_assets[2].angle + 2)%360)*60/SCREEN_FPS);		
			
			background_assets[0].render(gameWindow->renderer,0,0);
			background_assets[1].render(gameWindow->renderer,0,SCREEN_HEIGHT-180);
	
			background_assets[1].render(gameWindow->renderer,SCREEN_WIDTH-137*3,0);
			background_assets[0].render(gameWindow->renderer,SCREEN_WIDTH-137*3,SCREEN_HEIGHT-180);
		
			if(switchTo || loadIn)
			{
				background_assets[0].setAlpha(megaAlpha);
				background_assets[1].setAlpha(megaAlpha);
				background_assets[2].setAlpha(megaAlpha);
			
				if(switchTo)
					megaAlpha-=5;
				else
					megaAlpha+=5;			
				
				if(switchTo && megaAlpha == 0)
				{
					switchTo = false;
					
					background_assets[0].deallocate();
					background_assets[1].deallocate();
					background_assets[2].deallocate();			
				}
				else if(loadIn && megaAlpha == 255)
					loadIn = false;
			}
			
		}
		
		// switching background 
		void switchBackgroundTo(gameSystemMode m)
		{
			backgroundMode = m;
			
			if(switchTo == false && megaAlpha == 0)
			{
				background_assets[0] = texture("resources/sprites/background/2_background_asset.png",gameWindow->renderer);
				background_assets[1] = texture("resources/sprites/background/2_background_asset_2.png",gameWindow->renderer);
				background_assets[2] = texture("resources/sprites/background/theslayer.png",gameWindow->renderer);
				
				background_assets[1].scale = 3;
				background_assets[0].scale = 3;	
				background_assets[2].scale = 3;	
		
				loadIn = true;
			}
			else
				switchTo = true;
		}
	private:
		Uint8 colors[4] = {WHITE,0}; // background color of the window
	
		// renderer used 
		Window * gameWindow;
		
		// background mode based on current game system
		gameSystemMode backgroundMode;
		
		// is the background switching?
		bool switchTo = false;
		bool loadIn = false;
		
		// background sprites 
		texture * background_assets;
		
		// alpha of all assets
		int megaAlpha = 255;
		
		// animation counter 
		int movementCounter = 0;
		
};

// main game loop
void gameLoop(Window * gameWindow)
{
	gameSystemMode currentSystem = BATTLE;
	
	gameWindow->textHandler = textInfo(gameWindow->renderer,1,30);
	
	backgroundHandler background = backgroundHandler(gameWindow,currentSystem);
	
	battle * b = new battle(gameWindow);
	
	while(input.state != EXIT)
	{
		gameWindow->clearDisplay();
		
		background.display();
		
		input.update();
		
		switch(currentSystem)
		{
			case BATTLE:
			if(b->systemEnd)
			{
				b->deallocate();
				b->systemEnd = false;
				currentSystem = DUNGEON;
				
				background.switchBackgroundTo(currentSystem);
				delete b;
			}
			else
			{
				b->display();
				b->handler();
			}
			break;
			default:
			if(input.state == SELECT)
			{
				b = new battle(gameWindow);
				currentSystem = BATTLE;
				
				background.switchBackgroundTo(currentSystem);
			}
			break;
		}
		
		gameWindow->display();
	}
}

int main(int argc, char *argv[])
{
	// is the game in debug mode 
	bool debug = true;

	// use time for randomizer seed
	srand((unsigned)time(NULL));
	
	// define Window used for the game 
	Window gameWindow = Window();

	gameLoop(&gameWindow);
	
	return 0;
}