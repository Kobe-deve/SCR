#ifndef HANDLER
#include "game_handler.h"
#endif

struct buildingObject
{
	int coords[2];
	int base_size[2];
	int type;
};

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
		
		building1 = image("resources/sprites/test_building.png",g->renderer);
		building1.scale = scale;	
		building2 = image("resources/sprites/test_building_2.png",g->renderer);
		building2.scale = scale;	
		loadIn = true;
			
		max_x = 100;
		max_y = 100;
	
		numBuildings = 2;
		
		buildings[0].type = 1;
		buildings[0].coords[0] = 52;
		buildings[0].coords[1] = 52;
		buildings[0].base_size[0] = 7;
		buildings[0].base_size[1] = 5;	

		buildings[1].type = 2;
		buildings[1].coords[0] = 70;
		buildings[1].coords[1] = 52;
		buildings[1].base_size[0] = 7;
		buildings[1].base_size[1] = 5;			
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
		
		for(int z=0;z<2;z++)
		{
		for(int y=1;y<max_y;y++)
		{
			for(int x=max_x-1;x>=0;x--)
			{
				bool buildingSpot = false;
				int i;
				
				// display specific block
				renderRect = {160,0,40,40};
				
				// display buildings
				for(i=0;i<numBuildings;i++)
				{
					if((buildings[i].coords[0] <= x && x <= buildings[i].coords[0]+buildings[i].base_size[0])
					   && (buildings[i].coords[1] <= y && y <= buildings[i].coords[1]+buildings[i].base_size[1])
					   || (x == buildings[i].coords[0]+buildings[i].base_size[0]+1 && (buildings[i].coords[1] <= y && y <= buildings[i].coords[1]+buildings[i].base_size[1])))
					   {
							buildingSpot = true;
							break;
					   }
				}
				
				if(z == 1 && buildingSpot && (buildings[i].coords[0] == x && buildings[i].coords[1] == y))
				{
					switch(buildings[i].type)
					{
						case 1:
						building1.render(main_game->renderer,cameraX+(y*20*scale)+(x*20*scale),cameraY-(x*10*scale)+(y*10*scale)-(367*scale));
						break;
						case 2:
						building2.render(main_game->renderer,cameraX+(y*20*scale)+(x*20*scale),cameraY-(x*10*scale)+(y*10*scale)-(367*scale));
						break;
					}
				}
				else if(!buildingSpot && z == 1 && pX == x && pY == y) 
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
				else if(!buildingSpot && z == 1)// other party members 
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
				else if(z==0)
					brick.render(main_game->renderer,cameraX+(y*20*scale)+(x*20*scale),cameraY-(x*10*scale)+(y*10*scale),&renderRect);
				
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
	
	// moving party members
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
	
	// building collission detection
	bool buildingInSpot(int x, int y)
	{
		for(int i=0;i<numBuildings;i++)
		{
			// start battle if enemy on player coord
			if((buildings[i].coords[0] <= x && x <= buildings[i].coords[0]+buildings[i].base_size[0])
			    && (buildings[i].coords[1] <= y && y <= buildings[i].coords[1]+buildings[i].base_size[1]))
				return true;
		}
		return false;
	}
	
	// door detection 
	bool doorInSpot(int x,int y)
	{
		for(int i=0;i<numBuildings;i++)
		{
			// start battle if enemy on player coord
			if((buildings[i].coords[0]+3 <= x && x <= buildings[i].coords[0]+buildings[i].base_size[0]-3)
			    && y == buildings[i].coords[1]+buildings[i].base_size[1])
			{
				// what building did the player enter? 
				buildingKey = buildings[i].type;
				direction = SOUTH;
				return true;
			}
		}
		return false;
	
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
					
					door = doorInSpot(pX,pY);
					
					if(buildingInSpot(pX,pY))
						pY++;
				}
				break;
				case LEFT:
				direction = WEST;
				if((pX != 0))
				{
					partyMovementUpdate();
					pX--;
					
					door = doorInSpot(pX,pY);
					
					if(buildingInSpot(pX,pY))
						pX++;
				}
				break;
				case RIGHT:
				direction = EAST;
				if((pX != max_x-1))
				{
					partyMovementUpdate();
					pX++;
					
					door = doorInSpot(pX,pY);
					
					if(buildingInSpot(pX,pY))
						pX--;
				}
				break;
				case DOWN:
				direction = SOUTH;
				if((pY != max_y-1))
				{
					partyMovementUpdate();
					pY++;
					
					door = doorInSpot(pX,pY);
					
					if(buildingInSpot(pX,pY))
						pY--;
				}
				break;
				case SELECT:
				/*if(scale > 1)
					scale--;
				brick.scale = scale;
				building1.scale = scale;
				building2.scale = scale;	
				*/
				break;
					
				case MENU:
				/*
				buildings[numBuildings].coords[0] = pX+3;
				buildings[numBuildings].coords[1] = pY+3;

				buildings[numBuildings].base_size[0] = 7;
				buildings[numBuildings].base_size[1] = 5;	
				
				buildings[numBuildings].type = 2;	
				numBuildings++;	
				
				cout << "\n" << numBuildings;*/
				break;				
					
				case CANCEL:
				/*
				if(scale < 4)
					scale++;
				brick.scale = scale;
				building1.scale = scale;
				building2.scale = scale;*/	
				break;
			}	
		}
		
	}
	
	// allocating sprites back into memory 
	void allocateSprites()
	{
		player = image("resources/sprites/dungeon/kid.png",main_game->renderer);
		b_player = image("resources/sprites/dungeon/back_kid.png",main_game->renderer);
		brick = image("resources/sprites/dungeon/brick.png",main_game->renderer);
		brick.scale = scale;
		textbackground = image("resources/sprites/dungeon/backoftext.png",main_game->renderer);
		
		building1 = image("resources/sprites/test_building.png",main_game->renderer);
		building1.scale = scale;	
		building2 = image("resources/sprites/test_building_2.png",main_game->renderer);
		building2.scale = scale;	
	}
	
	// dellocating sprites from memory 
	void deallocate()
	{
		door = false;
		player.deallocate(); 
		b_player.deallocate();
		brick.deallocate();  
		textbackground.deallocate();
		building1.deallocate();
		building2.deallocate();
	}
	
	// did the player enter a building?
	bool door = false;
	// what building did the player enter?
	int buildingKey = 0;
	
	private:
		// is there dialogue happening?
		bool dialogue = false;
		
		// for rendering specific blocks from the block image 
		SDL_Rect renderRect = {0,0,40,40};
		
		// building sprites
		image building1;
		image building2;
		
		// keeping track of buildings 
		int numBuildings = 0; 
		int buildingCoords[100][4] = {}; // (x,y,w,h)
		buildingObject buildings[100] = {};
		
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