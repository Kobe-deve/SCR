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
	enemy_sprites[0] = texture("resources/sprites/battle/Monster/0.png",gameWindow->renderer);
	enemy_sprites[1] = texture("resources/sprites/battle/Monster/2.png",gameWindow->renderer);
	
	commandBox = texture("resources/sprites/battle/battle_menu.png",gameWindow->renderer);
	
	textBox = texture("resources/sprites/battle/battle_text_area.png",gameWindow->renderer);
	textBox.scale = 3;
	
	battleBackground = texture("resources/sprites/battle/test_area.png",gameWindow->renderer);
	battleBackground.scale = 2;
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
	battleBackground.render(gameWindow->renderer,250,10);
	
	if(monsterType == 24 || monsterType == 12 ||
	   monsterType == 13 || monsterType == 14 ||
	   monsterType == 25)
	{
		enemy_sprites[0].render(gameWindow->renderer,400,30);
	}
	else
	{
		enemy_sprites[0].render(gameWindow->renderer,300,40);
		enemy_sprites[1].render(gameWindow->renderer,500,20);
	}
	
	textBox.render(gameWindow->renderer,200,320);
	
	for(int i=0;i<5;i++)
	{
		commandBox.render(gameWindow->renderer,10,60*i+20);
		switch(i)
		{
			case 0:
			gameWindow->textHandler.displayText.display("Skills",25,60*i+30);
			break;
			case 1:
			gameWindow->textHandler.displayText.display("Block",25,60*i+30);
			break;
			case 2:
			gameWindow->textHandler.displayText.display("Strategy",25,60*i+30);
			break;
			case 3:
			gameWindow->textHandler.displayText.display("Items",25,60*i+30);
			break;
			case 4:
			gameWindow->textHandler.displayText.display("Run",25,60*i+30);
			break;
		}
	}
};
	
void battle::handler() 
{
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
};

