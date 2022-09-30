#ifndef SDL_MAIN_HANDLED
	#include "system/systems.h"
#endif

#ifndef HANDLER
	#include "system/Window.h"
	#include "system/textInfo.h"
#endif

#ifndef CHARACTER_HANDLED
	#include "models/character.h"
#endif


battle::battle(Window * g)
{
	gameWindow = g;
	
	enemy_sprites = new texture[2];
	
	// set up fight stats
	monsterType =  rand()%25+1;
	
	// set sprites 
	if(!(monsterType == 24 || monsterType == 12 || // boss display 
	   monsterType == 13 || monsterType == 14 ||
	   monsterType == 25))
		enemy_sprites[1] = texture("resources/sprites/battle/Monster/"+to_string((monsterType+2)%25)+".png",gameWindow->renderer);
	
	enemy_sprites[0] = texture("resources/sprites/battle/Monster/"+to_string(monsterType)+".png",gameWindow->renderer);
	
	
	commandBox = texture("resources/sprites/battle/battle_menu.png",gameWindow->renderer);
	
	textBox = texture("resources/sprites/battle/battle_text_area.png",gameWindow->renderer);
	textBox.scale = 3;
	
	battleBackground = texture("resources/sprites/battle/test_area.png",gameWindow->renderer);
	battleBackground.scale = 2;

	partyBackground = texture("resources/sprites/battle/party_menu.png",gameWindow->renderer);
	partyBackground.scale = 2;
	
	// start text displayer 
	gameWindow->textHandler.reset();
	gameWindow->textHandler.addLine("Encountered some enemies");
	gameWindow->textHandler.startTextDisplay();
}


// enemy AI actions
void battle::enemyAI()
{
	
};
		
// handling the turns in combat 
void battle::turn_handler()
{
	
};
		
// when the player selects a move for a party member 
void battle::player_selection()
{
	// coordinates for options
	int optionsX = 25;
	int optionsY = 30;
	
	for(int i=0;i<maxOptions;i++)
	{
		if(menuOption == i)
			optionsX = 50;
		else
			optionsX = 25;
			
		commandBox.render(gameWindow->renderer,optionsX-15,60*i+optionsY-10);
		
		switch(i)
		{
			case 0:
			gameWindow->textHandler.displayText.display("Skills",optionsX,60*i+optionsY);
			break;
			case 1:
			gameWindow->textHandler.displayText.display("Block",optionsX,60*i+optionsY);
			break;
			case 2:
			gameWindow->textHandler.displayText.display("Strategy",optionsX,60*i+optionsY);
			break;
			case 3:
			gameWindow->textHandler.displayText.display("Items",optionsX,60*i+optionsY);
			break;
			case 4:
			gameWindow->textHandler.displayText.display("Run",optionsX,60*i+optionsY);
			break;
		}
	}
};

void battle::display() 
{	
	
	if(switchMonster)
	{
		enemy_sprites[0].deallocate();
		enemy_sprites[1].deallocate();
		enemy_sprites[0] = texture("resources/sprites/battle/Monster/"+to_string(monsterType)+".png",gameWindow->renderer);
		enemy_sprites[1] = texture("resources/sprites/battle/Monster/"+to_string((monsterType+2)%25)+".png",gameWindow->renderer);
	
		switchMonster = false;
	}
	
	// background 
	battleBackground.render(gameWindow->renderer,250,10);
	
	// displaying monsters 
	if(monsterType == 24 || monsterType == 12 || // boss display 
	   monsterType == 13 || monsterType == 14 ||
	   monsterType == 25)
	{
		enemy_sprites[0].render(gameWindow->renderer,400,30);
	}
	else // regular combat monster display 
	{
		enemy_sprites[0].render(gameWindow->renderer,300,40);
		enemy_sprites[1].render(gameWindow->renderer,500,20);
	}
	
	// text box and displaying text information 
	textBox.render(gameWindow->renderer,200,320);
	
	// text info 
	gameWindow->textHandler.display(250,360);
	
	// display options if the player is selecting a character's move 
	player_selection();
	
	// party status 
	partyBackground.render(gameWindow->renderer,250,450);
	partyBackground.render(gameWindow->renderer,600,450);
	partyBackground.render(gameWindow->renderer,250,550);
	partyBackground.render(gameWindow->renderer,600,550);
};
	
void battle::handler() 
{
	switch(input.state)
	{
		case UP:
		if(menuOption > 0)
			menuOption--;
		else
			menuOption = maxOptions-1;
		break;
		case DOWN:
		if(menuOption < maxOptions-1)
			menuOption++;
		else
			menuOption = 0;
		break;
		case SELECT:
		
		gameWindow->textHandler.addLine("You did something really special.");
		gameWindow->textHandler.continueText();
		break;
		
		case CANCEL:
		systemEnd = true;
		break;
	}
};
		
void battle::deallocate() 
{
	enemy_sprites[0].deallocate();
	enemy_sprites[1].deallocate();
			
	commandBox.deallocate();		
			
	battleBackground.deallocate();
	textBox.deallocate();
	partyBackground.deallocate();
};

