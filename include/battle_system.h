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
	enemy_sprites = new texture("resources/sprites/battle/Monster/0.png",gameWindow->renderer);
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
		enemy_sprites->deallocate();
		enemy_sprites = new texture("resources/sprites/battle/Monster/"+to_string(monsterType)+".png",gameWindow->renderer);
		
		switchMonster = false;
	}
	enemy_sprites->render(gameWindow->renderer,300,200);
		
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
	enemy_sprites->deallocate();	
};

