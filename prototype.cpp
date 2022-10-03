#include "include/title_screen.h"
#include "include/battle_system.h"
#include "include/dungeon.h"

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
	dungeon_crawling test2;
	battle * test3 = nullptr;
	game_over * go = nullptr;
	
	
	httpProvider connectionHandler = httpProvider();
	
	game.currentGame = test1;
	
	if(connectionHandler.connected)
	{
		connectionHandler.testGET();
	}
	
	while(game.input.state != EXIT)
	{
		game.runSystem();
		if(test1!= nullptr && test1->endSystem) // title screen ends
		{
			switch(test1->option)
			{
				default:
				test2 = dungeon_crawling(&game,"test_floor");
				game.switchBackground(3);
				game.currentGame = &test2;
				test1->endSystem = false;
				
				Mix_FadeOutMusic(0); // stop current music 
				game.music =  Mix_LoadMUS( "resources/music/test_dungeon.wav");
				Mix_FadeInMusic(game.music, -1, 100); // fades into new music 
				test1->deallocate();
				delete test1;
				test1 = nullptr;
				break;
			}
		}
		else if(test2.endSystem) // dungeon to battle system
		{
			test3 = new battle(&game);
			game.currentGame = test3;
			game.switchBackground(5);
			test2.endSystem = false;
			
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
				game.currentGame = &test2;
				game.switchBackground(3);
			
				Mix_FadeOutMusic(0); // stop current music 
				game.music =  Mix_LoadMUS( "resources/music/test_dungeon.wav");
				Mix_FadeInMusic(game.music, -1, 1000); // fades into new music 
			}
		}
		
		
		if(connectionHandler.connected)
		{
			connectionHandler.pingServer();
		}
	}
	
	game.close();
	return 0;
}

