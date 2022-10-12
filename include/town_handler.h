#ifndef HANDLER
#include "game_handler.h"
#endif

class town : public system_handler
{
	public:
	town(){}
	
	town(game_handler * g)
	{
		main_game = g;
		
		player = image("resources/sprites/dungeon/kid.png",g->renderer);
		b_player = image("resources/sprites/dungeon/back_kid.png",g->renderer);
		brick = image("resources/sprites/dungeon/brick.png",g->renderer);
		brick.scale = scale;
		textbackground = image("resources/sprites/dungeon/backoftext.png",g->renderer);
		
		building = image("resources/sprites/test_building.png",g->renderer);
		building.scale = scale;	
		loadIn = true;
			
		max_x = 100;
		max_y = 100;	
			
	}
	
	void display() override
	{
		// set camera position for the dungeon
		cameraX = SCREEN_WIDTH/2-(pY*20*scale)-(pX*20*scale);
		cameraY = SCREEN_HEIGHT/2+(pX*10*scale)-(pY*10*scale);
		
		if(loadIn || switchOut)
		{
			brick.setAlpha(megaAlpha);
		}
			
		// go through data of the floor the player is on to display dungeon
		for(int y=1;y<max_y;y++)
		{
			for(int x=max_x-1;x>=0;x--)
			{
				// display specific block
				renderRect = {160,0,40,40};
				
				brick.render(main_game->renderer,cameraX+(y*20*scale)+(x*20*scale),cameraY-(x*10*scale)+(y*10*scale),&renderRect);
				
				// display characters 
				if(pX == x && pY == y)
				{
					// display player 
					if(!loadIn && !switchOut) // if everything is loaded in
					{
						switch(direction)
						{
							case NORTH:
							b_player.flip = SDL_FLIP_HORIZONTAL;
							b_player.render(main_game->renderer,cameraX+(pY*20*scale)+(pX*20*scale)+30,cameraY-(pX*10*scale)+(pY*10*scale)-70);
							break;
							case EAST:
							b_player.flip = SDL_FLIP_NONE;
							b_player.render(main_game->renderer,cameraX+(pY*20*scale)+(pX*20*scale)+30,cameraY-(pX*10*scale)+(pY*10*scale)-70);
							break;
							case WEST:
							player.flip = SDL_FLIP_HORIZONTAL;
							player.render(main_game->renderer,cameraX+(pY*20*scale)+(pX*20*scale)+30,cameraY-(pX*10*scale)+(pY*10*scale)-70);
							break;
							case SOUTH:
							player.flip = SDL_FLIP_NONE;
							player.render(main_game->renderer,cameraX+(pY*20*scale)+(pX*20*scale)+30,cameraY-(pX*10*scale)+(pY*10*scale)-70);
							break;
						}
					}
				}
				else // other party members 
				{
					for(int i=0;i<numParty-1;i++)
					{
						if(partCoords[i][0] == x && partCoords[i][1] == y)
						{
							switch(i)
							{
								case 0:
								player.setColor(255,0,0);
								b_player.setColor(255,0,0);
								case 1:
								player.setColor(0,255,0);
								b_player.setColor(0,255,0);
								case 2:
								player.setColor(0,0,255);
								b_player.setColor(0,0,255);
								break;
							}
							// display player 
							if(!loadIn && !switchOut) // if everything is loaded in
							{
									switch(partFace[i])
								{
									case NORTH:
									b_player.flip = SDL_FLIP_HORIZONTAL;
									b_player.render(main_game->renderer,cameraX+(partCoords[i][1]*20*scale)+(partCoords[i][0]*20*scale)+30,cameraY-(partCoords[i][0]*10*scale)+(partCoords[i][1]*10*scale)-70);
									break;
									case EAST:
									b_player.flip = SDL_FLIP_NONE;
									b_player.render(main_game->renderer,cameraX+(partCoords[i][1]*20*scale)+(partCoords[i][0]*20*scale)+30,cameraY-(partCoords[i][0]*10*scale)+(partCoords[i][1]*10*scale)-70);
									break;
									case WEST:
									player.flip = SDL_FLIP_HORIZONTAL;
									player.render(main_game->renderer,cameraX+(partCoords[i][1]*20*scale)+(partCoords[i][0]*20*scale)+30,cameraY-(partCoords[i][0]*10*scale)+(partCoords[i][1]*10*scale)-70);
									break;
									case SOUTH:
									player.flip = SDL_FLIP_NONE;
									player.render(main_game->renderer,cameraX+(partCoords[i][1]*20*scale)+(partCoords[i][0]*20*scale)+30,cameraY-(partCoords[i][0]*10*scale)+(partCoords[i][1]*10*scale)-70);
									break;
								}
							}
							break;
						}
					}
					player.setColor(255,255,255);
					b_player.setColor(255,255,255);		
				}
		
			}	
		}	
		
		// displaying dialogue
		if(dialogue)
		{
			textbackground.render(main_game->renderer,25,500);
		}
		
		// loading into new floor
		if(loadIn && megaAlpha == 255)
			loadIn = false;
		else if(loadIn)
			megaAlpha+=5;
		else if(switchOut && megaAlpha == 0)
		{
			megaAlpha = 255;
			switchOut = false;
			loadIn = true;
		}
		else if(switchOut)
			megaAlpha-=5;
	}
	
	void partyMovementUpdate()
	{
		if(numParty > 1)
		{
			for(int i=numParty-2;i>=1;i--)
			{
				partFace[i] = partFace[i-1];
				partCoords[i][1] = partCoords[i-1][1]; 
				partCoords[i][0] = partCoords[i-1][0];
			}	
			partFace[0] = direction;
			partCoords[0][1] = pY;
			partCoords[0][0] = pX;			
		}
	}
	
	void handler() override
	{
		if(!loadIn && !switchOut)
		{
			switch(main_game->input.state)
			{
				case UP:
				direction = NORTH;
				if((pY != 1))
					{
					partyMovementUpdate();
					pY--;
				}
				break;
				case LEFT:
				direction = WEST;
				if((pX != 0))
				{
					partyMovementUpdate();
					pX--;
				}
				break;
				case RIGHT:
				direction = EAST;
				if((pX != max_x-1))
				{
					partyMovementUpdate();
					pX++;
				}
				break;
				case DOWN:
				direction = SOUTH;
				if((pY != max_y-1))
				{
					partyMovementUpdate();
					pY++;
				}
				break;
				case SELECT:
				break;
					
				case CANCEL:
				break;
			}	
		}
	}
	
	void deallocate()
	{
		player.deallocate(); 
		b_player.deallocate();
		brick.deallocate();  
		textbackground.deallocate();
		building.deallocate();
	}
	
	private:
		// is there dialogue happening?
		bool dialogue = false;
		
		// for rendering specific blocks from the block image 
		SDL_Rect renderRect = {0,0,40,40};
		
		image building;
		
		
		// variables used for loading in a new floor 
		bool loadIn = false;
		bool switchOut = false;
		int megaAlpha = 0;
		
		// max size of map
		int max_z; 
		int max_y;
		int max_x;
		
		// player coords
		int pX = 50;
		int pY = 50;
		int pZ = 0;
		
		// image of dungeon blocks
		image brick;  
		
		// player sprite 
		image player; 
		image b_player;
		
		// text background
		image textbackground;
		
		// party member coordinates;
		int partCoords[3][2];
		facing partFace[3];
		
		// camera variables
		int cameraX = 400;
		int cameraY = 300;
		
		// scale of images
		int scale = 3;
		
		// the direction the player is facing 
		facing direction = SOUTH;
};