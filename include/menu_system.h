#ifndef HANDLER
#include "game_handler.h"
#endif

#ifndef MODELS_DEFINED
#include "models.h"
#endif

class menu_system : public system_handler
{
	public:
		menu_system(game_handler * g)
		{
			main_game = g;
			party_menu = image("resources/sprites/battle/party_menu.png",g->renderer);
		
			combat_info = text(g->renderer,1);
			party_menu.scale = 2;
			option = numParty;
		}
		
		void display() override
		{
			int menx;
			int meny = 20;
			
			if(option < numParty)
			{	
				// display character sprite
				if(party[option].type == COBOL)
					examY = 300;
				else if(party[option].type == TRAOTIC)
					examY = 150;
				else
					examY = 100;
				
				party[option].sprite.setAlpha(100);
				party[option].sprite.render(main_game->renderer,10,examY);
				party[option].sprite.setAlpha(255);
			}

			// display party 
			for(int i=0;i<numParty;i++)
			{
				if(i==option)
					menx = 500;
				else
					menx = 580;
				
				party_menu.render(main_game->renderer,menx,meny+150*i); 
				party[i].portraitDisplay(main_game->renderer,menx+10,meny+5+150*i);
				combat_info.display(party[i].name,menx+150,meny+100+150*i);	
				combat_info.display("HP:" + to_string(party[i].health)+"/"+to_string(party[i].maxHealth),menx+150,meny+10+150*i);
				combat_info.display("STAMINA:" + to_string(party[i].stamina)+"/"+to_string(party[i].maxStamina),menx+150,meny+55+150*i);
			}
			if(option==numParty)
				menx = 500;
			else
				menx = 580;
			party_menu.scale = 1;
			party_menu.render(main_game->renderer,menx,meny+150*numParty); 
			combat_info.display("BACK",menx+20,meny+20+150*numParty);	
				
			if(option < numParty)
			{	
				// display stats
				combat_info.display("ATT:" + to_string(party[option].strength) + "/" + to_string(party[option].maxStr),10,40);
				combat_info.display("DEF:" + to_string(party[option].defense) + "/" + to_string(party[option].maxDef),10,80);
				combat_info.display("AGI:" + to_string(party[option].agility) + "/" + to_string(party[option].maxAgil),10,120);
				
				combat_info.display("Dealt Damage:" + to_string(party[option].dealtdamageNeeded) + "/" + to_string(party[option].maxDealtDam),10,550);
				combat_info.display("Damage Taken:" + to_string(party[option].damageNeeded) + "/" + to_string(party[option].maxDam),10,590);
				combat_info.display("Times Exhausted:" + to_string(party[option].expiredStam) + "/" + to_string(party[option].maxExpStam),10,630);
				
				combat_info.display("Scars:" + to_string(party[option].scars),320,550);				
				
				// display moves 
				combat_info.display("Moves:",200,40);
				for(int i=0;i<party[option].numMoves;i++)
				{
					party_menu.render(main_game->renderer,300,20+i*70); 
					combat_info.display(party[option].abilities[i].name,320,40+i*70);	
				}
			}	
			party_menu.scale = 2;
		}
		
		void handler() override
		{
			switch(main_game->input.state)
			{
				case UP:
				if(option != 0)
					option--;
				break;
				case SELECT:
				endSystemHandler();
				break;
				case DOWN:
				if(option != numParty)
					option++;
				break;
			}
		}
		
		void deallocate()
		{
			party_menu.deallocate();
			combat_info.deallocate();
		}
	private:
		int examY;
		
		int option = 0;
	
		// images used for UI
		image party_menu;
		
		// for displaying info on what's happening in battle  
		text combat_info; 
};
