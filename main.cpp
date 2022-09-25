#include "include/battle_system.h"

//g++ main.cpp -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -static-libstdc++ -std=c++11 -Wl,-rpath,./ -o main

enum gameSystemMode
{
	TITLE_SCREEN=1,
	BATTLE=2,
	DUNGEON=3,
	MENU_SELECTION=4
};

void gameLoop(Window * gameWindow)
{
	gameSystemMode currentSystem = BATTLE;
	
	battle * b = new battle(gameWindow);
	
	while(input.state != EXIT)
	{
		gameWindow->clearDisplay();
		
		input.update();
		
		switch(currentSystem)
		{
			case BATTLE:
			if(b->systemEnd)
			{
				b->deallocate();
				b->systemEnd = false;
				currentSystem = DUNGEON;
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
/*
	texture monster = texture("resources/sprites/battle/Monster/0.png",gameWindow.renderer);
	int monsterType = 0;
	bool switchMonster = false;
	bool display = true;
	
	
	
	// switches based on argument if debug = true
	while(input.state != EXIT)
	{
		gameWindow.clearDisplay();
		
		input.update();
		switch(input.state)
		{
			case UP:
			switchMonster = true;
			if(monsterType == 0)
				monsterType = 25;
			else
				monsterType--;
			break;
			case DOWN:
			switchMonster = true;
			if(monsterType == 25)
				monsterType = 0;
			else
				monsterType++;
			break;
			case SELECT:
			display = false;
			monster.deallocate();
			break;
		}
		
		if(switchMonster)
		{
			monster.deallocate();
			monster = texture("resources/sprites/battle/Monster/"+to_string(monsterType)+".png",gameWindow.renderer);
			
			switchMonster = false;
		}
		
		if(display)
			monster.render(gameWindow.renderer,300,200);
		
		gameWindow.display();
	}
	*/
	
	gameLoop(&gameWindow);
	
	return 0;
}