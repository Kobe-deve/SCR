#include "include/title_screen.h"
#include "include/battle_system.h"
#include "include/dungeon.h"
#include "include/menu_system.h"
#include "include/town_handler.h"

#include "include/http_provider.h"

//g++ prototype.cpp -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lcurl -static-libstdc++ -std=c++11 -Wl,-rpath,./ -o prototype

class game_over : public system_handler
{
	public:	
		game_over(){}
		game_over(game_handler * g)
		{
			main_game = g;
			main_game->displayText.textColor.a = 255;
		}
		void display() override
		{
			if(!main_game->loadIn && !main_game->switchOut)
				main_game->displayText.display("THE GAME IS OVER.",400,400);
		}
};

int main(int argc, char *argv[])
{
	// use time for randomizer seed
	srand((unsigned)time(NULL));
	
	// load dlls in memory
	LoadLibrary("./libfreetype-6.dll");
	LoadLibrary("./libgcc_s_dw2-1.dll");
	LoadLibrary("./libgcc_s_sjlj-1.dll");
	LoadLibrary("./libjpeg-9.dll");
	LoadLibrary("./libpng16-16.dll");
	LoadLibrary("./libstdc++-6.dll");
	LoadLibrary("./libtiff-5.dll");
	LoadLibrary("./libwebp-7.dll");
	LoadLibrary("./libwinpthread-1.dll");
	LoadLibrary("./zlib1.dll");
	
	game_handler game = game_handler(2);
	game.backgroundOn = true;
	
	// load initial stats 
	party = new stats[4];
	numParty = 1;
	party[0] = stats(game.renderer,PROTAGONIST);
	party[0].name = "TWILIGHT";
	
	// opening theme 
	game.music =  Mix_LoadMUS( "resources/music/Sleep.wav");
	Mix_PlayMusic(game.music, 1);
			
	// set sound effect volume to 1/3 max 
	Mix_Volume(0,MIX_MAX_VOLUME/2);
					
	// define game systems being used 		
	title_screen * test1 = new title_screen(&game);
	dungeon_crawling * test2 = nullptr;
	battle * test3 = nullptr;
	menu_system * test4 = nullptr;
	town test5;
	
	game_over * go = nullptr;
	
	game.currentGame = test1;
	
	while(game.input.state != EXIT)
	{
		game.runSystem();
		if(test1!= nullptr && test1->endSystem) // title screen ends
		{
			switch(test1->option)
			{
				default:
				test5 = town(&game);
				game.switchBackground(0);
				game.currentGame = &test5;
				test1->endSystem = false;
				
				test1->deallocate();
				delete test1;
				test1 = nullptr;
				break;
			}
		}
		else if(test5.door) // city to dungeon 
		{
			test5.deallocate();
			switch(test5.buildingKey)
			{
				case 1:
				test2 = new dungeon_crawling(&game,"test_floor");
				break;
				case 2:
				test2 = new dungeon_crawling(&game,"test_floor_2");
				break;
			}
			game.currentGame = test2;
			
			Mix_FadeOutMusic(0); // stop current music 
			game.music =  Mix_LoadMUS( "resources/music/test_dungeon.wav");
			Mix_FadeInMusic(game.music, -1, 100); // fades into new music 
			game.switchBackground(5);		
		}
		else if(test2 != nullptr && test2->exitBuilding && test2->endSystem) // dungeon to city
		{
			test2->deallocate();
			delete test2;
			test2 = nullptr;
			
			test5.allocateSprites();
			game.currentGame = &test5;
			game.switchBackground(0);
		}
		else if(test2 != nullptr && !test2->exitBuilding && test2->endSystem) // dungeon to battle system
		{
			test3 = new battle(&game);
			game.currentGame = test3;
			game.switchBackground(5);
			test2->endSystem = false;
			
			Mix_FadeOutMusic(0); // stop current music 
			switch(rand()%2+1)
			{
				case 1:
				game.music =  Mix_LoadMUS( "resources/music/Stone_Crawler_Battle_2.wav");
				break;
				case 2:
				game.music =  Mix_LoadMUS( "resources/music/Stone_Crawler_Battle.wav");
				break;
			}
			Mix_FadeInMusic(game.music, -1, 1000); // fades into new music 
		}
		else if(test2 != nullptr && game.currentGame == test2 && !test2->endSystem && game.input.state == MENU) // dungeon to menu 
		{
			test4 = new menu_system(&game);
			game.currentGame = test4;
			game.switchBackground(2);
		}
		else if(test4 != nullptr && test4->endSystem) // menu to dungeon 
		{
			test4->endSystem = false;
			
			test4->deallocate();
			delete test4;
			test4 = nullptr;
			
			game.currentGame = test2;
			game.switchBackground(3);
		}
		else if(test3 != nullptr && test3->endSystem) // battle to dungeon system
		{
			bool over = true;
			test3->endSystem = false;
			
			test3->deallocate();
			delete test3;
			test3 = nullptr;
			
			// checks if the player wasn't defeated
			for(int i=0;i<numParty;i++)
			{
				if(party[i].health > 0)
					over = false;	
			}
			
			if(over) // game over if player defeated 
			{
				game.switchBackground(2);
				go = new game_over(&game);
				game.currentGame = go;
			}
			else // continue dungeon crawling 
			{
			
				game.currentGame = test2;
				game.switchBackground(3);
			
				Mix_FadeOutMusic(0); // stop current music 
				game.music =  Mix_LoadMUS( "resources/music/test_dungeon.wav");
				Mix_FadeInMusic(game.music, -1, 1000); // fades into new music 
			}
		}
		
	}
	
	game.close();
	return 0;
}

