#include "include/battle_system.h"
#include "include/menu_system.h"
#include "include/dungeon.h"
#include "include/town_handler.h"

#include "include/http_provider.h"
//g++ dungeon_runner.cpp -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lcurl -lSDL2_mixer -static-libstdc++ -std=c++11 -Wl,-rpath,./ -o dungeon_runner

int main(int argc, char *argv[])
{
	// use time for randomizer seed
	srand((unsigned)time(NULL));
	
	game_handler game = game_handler(0);
	game.backgroundOn = true;
	
	// load initial stats 
	party = new stats[4];
	numParty = 1;
	party[0] = stats(game.renderer,PROTAGONIST);
	party[0].name = "TWILIGHT";
	
	battle * battle_system = nullptr;
	dungeon_crawling * test_building = nullptr; 
	town test = town(&game);
	menu_system * test_menu = nullptr;
	game.currentGame = &test;
	
	while(game.input.state != EXIT)
	{
		if(test_building != nullptr && test_building->exitBuilding && test_building->endSystem) // exiting building to town 
		{
			test_building->deallocate();
			delete test_building;
			test_building = nullptr;
			
			test.allocateSprites();
			game.currentGame = &test;
			game.switchBackground(0);
		}
		else if(test.door) // town to building 
		{
			test.deallocate();
			switch(test.buildingKey)
			{
				case 1:
				test_building = new dungeon_crawling(&game,"test_floor");
				break;
				case 2:
				test_building = new dungeon_crawling(&game,"test_floor_2");
				break;
			}
			game.currentGame = test_building;
			
			game.switchBackground(5);
		}
		else if(test_building != nullptr && !test_building->exitBuilding && !test_building->exitBuilding && test_building->endSystem) // building to battle system
		{
			test_building->deallocate(false);
			battle_system = new battle(&game);
			game.currentGame = battle_system;
			game.switchBackground(5);
			test_building->endSystem = false;
			
		}
		else if(battle_system != nullptr && battle_system->endSystem) // battle to building system
		{
			bool over = true;
			battle_system->endSystem = false;
			
			battle_system->deallocate();
			delete battle_system;
			battle_system = nullptr;
			
			// checks if the player wasn't defeated
			for(int i=0;i<numParty;i++)
			{
				if(party[i].health > 0)
					over = false;	
			}
			
			if(over) // game over if player defeated 
			{
				cout << "\nDEFEATED";
				for(int i=0;i<numParty;i++)
					party[i].health = party[i].maxHealth;
			}
			
			test_building->allocate();
			game.currentGame = test_building;
			game.switchBackground(3);
		}
		else if(game.input.state == MENU && battle_system == nullptr && (test_building != nullptr || game.currentGame == &test) && test_menu == nullptr) // enter menu 
		{
			// deallocate system 
			if(test_building != nullptr)
			{
				test_building->deallocate(false);
				game.currentGame = test_building;
			}
			else
			{
				test.deallocate();
				game.currentGame = &test;
			}
			test_menu = new menu_system(&game);
			game.currentGame = test_menu;
		}
		else if(test_menu != nullptr && test_menu->endSystem) // enter menu system  
		{
			test_menu->endSystem = false;
			
			test_menu->deallocate();
			delete test_menu;
			test_menu = nullptr;
			
			
			// allocate system back
			if(test_building != nullptr)
			{
				test_building->allocate();
				game.currentGame = test_building;
			}
			else
			{
				test.allocateSprites();
				game.currentGame = &test;
			}
		}
		
		game.runSystem();		
	}
	
	game.close();
	return 0;
}
