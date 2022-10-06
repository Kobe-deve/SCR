#ifndef HANDLER
#include "game_handler.h"
#endif

class title_screen : public system_handler
{
	public:
		title_screen(){}
		
		title_screen(game_handler * g)
		{
			main_game = g;
			main_game->loadIn = false;
			test = image("resources/sprites/title_screen/kid.png",g->renderer);
			test2 = image("resources/sprites/title_screen/stone_crawler.png",g->renderer);
			test_logo = image("resources/sprites/title_screen/logo.png",g->renderer);
			
			megaAlpha = 0;
			
			loadIn = true;
		}
		
		// displayed selection/title screen
		void selectionScreen()
		{
			test2.render(main_game->renderer,0,0);
				
			test_logo.render(main_game->renderer,400,300);
			
			main_game->displayText.display("New Game",600,520,angleSelect[0]);
			main_game->displayText.display("Instructions",600,560,angleSelect[1]);
				
			main_game->displayText.display("FINALITY GAMES",10,600);
			main_game->displayText.display("Developed by: DEMI",10,650);
			
		}
		
		// opening animation logic
		void opening()
		{
			if(!kidLoaded)
				test_logo.setAlpha(megaAlpha);
			test.setAlpha(megaAlpha);
			test2.setAlpha(megaAlpha);
			
			if(!loadMonster)
				test.render(main_game->renderer,200,0);
			else
				test2.render(main_game->renderer,0,0);
				
			test_logo.render(main_game->renderer,400,300);
			
			if(!loadedMonster)
			{
				if(loadIn && megaAlpha == 255)
				{
					if(loadMonster)
					{
						main_game->switchBackground(0);
						loadedMonster = true;
					}
					loadIn = false;
					kidLoaded = true;
					switchOut = true;
				}
				else if(loadIn)
					megaAlpha+=1;
				else if(switchOut && megaAlpha == 0)
				{	
					switchOut = false;
					loadMonster = true;
					loadIn = true;
				}
				else if(switchOut)
					megaAlpha-=5;
			}
		}
		
		void instruction_display()
		{
			main_game->displayText.display("DUNGEON CONTROLS:",10,10);
			main_game->displayText.display("ARROW KEYS - MOVEMENT",10,40);
			main_game->displayText.display("SPACEBAR - STATUS/MENU",10,70);
			
			main_game->displayText.display("BATTLE CONTROLS:",10,120);
			main_game->displayText.display("ARROW KEYS - SELECT OPTION",10,150);
			main_game->displayText.display("ENTER - CONFIRM",10,180);
			main_game->displayText.display("BACKSPACE - BACK",10,210);
			main_game->displayText.display("SPACEBAR - AUTO BATTLE",10,240);
			
			main_game->displayText.display("Press Enter to exit",600,650);
			
		}
		
		void display() override
		{
			if(!loadedMonster)
				opening();
			else if(instructions)
				instruction_display();	
			else
				selectionScreen();
		}		
	
		// handle input/logic
		void handler() override
		{
			if(instructions && main_game->input.state == SELECT) // instructions 
			{
				main_game->switchBackground(0);
				instructions = false;
			}
			else if(loadedMonster) // title screen selection 
			{
				switch(main_game->input.state)
				{
					case UP:
					angleSelect[option]=0;
					if(option != 0)
						option--;
					break;
					case DOWN:
					angleSelect[option]=0;
					if(option != 1)
						option++;
					break;
					case SELECT:
					switch(option)
					{
						case 0:
						endSystemHandler();
						break;
						case 1:
						instructions = true;
						main_game->switchBackground(2);
						break;
					}
					break;
				}
				angleSelect[option]+=3;
				angleSelect[option]%=360;
			}
		}
		
		void deallocate() override
		{
			test.deallocate();
			test2.deallocate();
			test_logo.deallocate();	
		}
		
		// option on the title screen
		int option = 0;
	private:
		// for spinning the current selected option
		int angleSelect[2] = {0,0};
		
		// variables used for animation timing
		bool textPart = true;
			
		bool loadIn = false;
		bool switchOut = false;
		int megaAlpha = 0;
		
		bool kidLoaded = false;
		bool loadMonster = false;
		bool loadedMonster = false;
		bool instructions = false;
		
		// images used
		image test;
		image test2;
		image test_logo;
};
