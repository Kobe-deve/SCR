#ifndef HANDLER
#include "include/system/Window.h"
#include "include/system/text_prompt.h"
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
	
	textInfo testDisplay = textInfo(gameWindow.renderer,1,30);
	testDisplay.addLine("HERE WE GO");
	testDisplay.startTextDisplay();
	testDisplay.displayText.black = true;
	input_handler input = input_handler();
	
	texture monster = texture("resources/sprites/battle/Monster/0.png",gameWindow.renderer);
	int monsterType = 0;
	bool switchMonster = false;
	
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
				monsterType = 19;
			else
				monsterType--;
			break;
			case DOWN:
			switchMonster = true;
			if(monsterType == 19)
				monsterType = 0;
			else
				monsterType++;
			break;
		}
		
		if(switchMonster)
		{
			monster.deallocate();
			monster = texture("resources/sprites/battle/Monster/"+to_string(monsterType)+".png",gameWindow.renderer);
		
			switch(monsterType)
			{
				case 0:
				testDisplay.addLine("Stranjer");
				break;
				case 1:
				testDisplay.addLine("Cobol");
				break;
				case 2:
				testDisplay.addLine("Order Duck");
				break;
				case 3:
				testDisplay.addLine("Loating");
				break;
				case 4:
				testDisplay.addLine("Dream Fiend");
				break;
				case 5:
				testDisplay.addLine("Sorrow");
				break;
				case 6:
				testDisplay.addLine("Demonoid");
				break;
				case 7:
				testDisplay.addLine("Shopkeeper");
				break;
				case 8:
				testDisplay.addLine("Anima Remnant");
				break;
				case 9:
				testDisplay.addLine("Traotic");
				break;
				case 10:
				testDisplay.addLine("World Beholder");
				break;
				case 11:
				testDisplay.addLine("Tinker Toy");
				break;
				case 12:
				testDisplay.addLine("The Cure");
				break;
				case 13:
				testDisplay.addLine("Transmitter-Z");
				break;
				case 14:
				testDisplay.addLine("Cosmic Puppet");
				break;
				case 15:
				testDisplay.addLine("Dream Demon");
				break;
				case 16:
				testDisplay.addLine("Malice");
				break;
				case 17:
				testDisplay.addLine("Gatsby");
				break;
				case 18:
				testDisplay.addLine("Moonlight");
				break;
				case 19:
				testDisplay.addLine("Tower Demon");
				break;
			}
			testDisplay.continueText();
			switchMonster = false;
		}
		
		testDisplay.display(100,100);
		
		monster.render(gameWindow.renderer,300,200);
		
		gameWindow.display();
	}
	
	return 0;
}