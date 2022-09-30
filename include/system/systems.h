// SDL2 libraries 
#define SDL_MAIN_HANDLED

// main SDL2 handler
#include <SDL2/SDL.h>

// handles pngs with SDL2
#include <SDL2/SDL_image.h>

// handles fonts with SDL2
#include <SDL2/SDL_ttf.h>

// handles music with SDL2
#include <SDL2/SDL_mixer.h>

// windows handler
#include <map>
#include <vector>
#include <utility>
#include <fstream>
#include <math.h>
#include <time.h>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;

#define PI 3.14159256

// max FPS constant 
int SCREEN_FPS = 60; 
// calculates ticks per frame for timers
int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS; 

// Color constants 
#define WHITE 255,255,255
#define BLACK 0,0,0

class text; 
class system_handler;
class game_handler;

// ------------------------------------ Handler Base --------------------------------

// class for handling timers
class timer 
{
	public:
		timer()
		{
			started = false;
			paused = false;
			
			startTicks = 0;
			pauseTicks = 0;
		}
		// starting timer 
		void start()
		{
			started = true;
			paused = false;
			
			startTicks = SDL_GetTicks();
			pauseTicks = 0;
		}
		// stopping timer 
		void stop()
		{
			started = false;
			paused = false;
			
			startTicks = 0;
			pauseTicks = 0;	
		}
		
		// pausing timer to resume later 
		void pause()
		{
			if(started && !paused)
			{
				paused = true;
				
				pauseTicks = SDL_GetTicks() - startTicks;
				startTicks = 0;
			}
		}
		// resuming timer 
		void resume()
		{
			if(started && paused)
			{
				paused = false;
				
				startTicks = SDL_GetTicks() - pauseTicks;
				pauseTicks = 0;
			}	
		}
		// gets ticks passed by from timer 
		Uint32 getTicks()
		{
			Uint32 t = 0;
			if(started)
			{
				if(paused)
					t = pauseTicks;
				else
					t = SDL_GetTicks() - startTicks;
			}
			return t;
		}
		bool paused = false; // is the timer paused? 
		bool started = false; // has the timer started? 
	
	private:
		// ticks used for timer 
		Uint32 startTicks = 0;
		Uint32 pauseTicks = 0;
};

// starting render font size of text
#define START_SIZE 35

// image class  
class texture 
{
	public:
		texture()
		{
			center->x = 0;
			center->y = 0;
		}
		
		// loads class by getting texture ready 
		texture(string path, SDL_Renderer* gRenderer)
		{
			//Load image at specified path
			SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
			if( loadedSurface == NULL )
				printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
			else
			{
				//Create texture from surface pixels
				imageTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
				
				if( imageTexture == NULL )
					printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
				else
				{
					width = loadedSurface->w;
					height = loadedSurface->h;
				}
				
				//Get rid of old loaded surface
				SDL_FreeSurface( loadedSurface );
			}
		}
		
		// loads class through font format 
		texture(int fontType, int paramSize=40)
		{
			
			size = paramSize;
			switch(fontType)
			{
				case 0:
				font = TTF_OpenFont("resources/fonts/ARCADE.ttf",paramSize);
				break;
				default:
				case 1:
				font = TTF_OpenFont("resources/fonts/chiaro.ttf",paramSize);	
				break;
			}
		}
		
		// changing color of the image 
		void setColor( Uint8 red, Uint8 green, Uint8 blue ) // changes color value of image
		{
			//Modulate texture
			SDL_SetTextureColorMod(imageTexture, red, green, blue );
		}
		
		// changing transparency of the image 
		void setAlpha(Uint8 alph) // changes alpha value of image
		{
			SDL_SetTextureBlendMode(imageTexture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(imageTexture, alph);
			
			color.a = alph;
		}
		
		// for rendering images 
		void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip=NULL) // displays an image, clip is a section of an image
		{
			//Set rendering space and render to screen
			renderQuad = { x, y, width*scale, height*scale };
			
			if(clip!=NULL)
			{
				renderQuad.w = clip->w*scale;
				renderQuad.h = clip->h*scale;	
			}
			
			//Render to screen 
			SDL_RenderCopyEx(gRenderer, imageTexture, clip, &renderQuad, angle, NULL, flip);
		}
		
		void deallocate()
		{
			SDL_DestroyTexture(imageTexture);
		}
		
		// for displaying text 
		void createFontTexture(SDL_Renderer* gRenderer, string t)
		{
			SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, t.c_str(), color );
			imageTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
			
			width = textSurface->w;
			height = textSurface->h;
			SDL_FreeSurface(textSurface);
		}
		
		// get texture
		SDL_Texture * grabTexture()
		{
			return imageTexture;
		}
		
		// point of rotation 
		SDL_Point * center= new SDL_Point;
		
		SDL_Renderer * renderer;
		int size = 0;
		
		// flipping the image 
		SDL_RendererFlip flip  = SDL_FLIP_NONE;
		
		// angle of rotation for the image 
		double angle = 0;
		
		// scale of the image 
		int scale = 1; 
		
		int height; // height and width of an image
		int width;
		SDL_Color color = {255,255,255,255}; // text color 
	protected:			
		TTF_Font *font = NULL; // font information 
		SDL_Rect renderQuad;
		int x = 0; // coordinates
		int y = 0;

		SDL_Texture* imageTexture = nullptr; // holds image information
};

// animated objects class 
class animatedAsset : public texture 
{
	public:
		animatedAsset(){};
		animatedAsset(int w, int h, int r, int maxWid, int f, string path, SDL_Renderer* gRenderer)
		{
			frames = f;
			maxW = maxWid;
			camera.w = w;
			camera.h = h;
			camera.x = 0;
			camera.y = 0;
			rate = r;	
			
			//Load image at specified path
			SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
			if( loadedSurface == NULL )
				printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
			else
			{
				//Create texture from surface pixels
				imageTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
				
				if( imageTexture == NULL )
					printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
				else
				{
					width = loadedSurface->w;
					height = loadedSurface->h;
				}
				
				//Get rid of old loaded surface
				SDL_FreeSurface( loadedSurface );
			}
		};
		
		// display animation 
		void display(SDL_Renderer * r, int x, int y)
		{
			//Set rendering space and render to screen
			renderQuad = { x, y, width*scale, height*scale };
			
			renderQuad.w = camera.w*scale;
			renderQuad.h = camera.h*scale;	
		
			//Render to screen 
			SDL_RenderCopyEx(r, imageTexture, &camera, &renderQuad, 0, center, flip);
		}
		
		// update animation 
		void update()
		{
			if(animate)
			{
				if(!down && step % rate == 0)
					counter++;
				else if(down && step % rate == 0)
					counter--;
				
				counter %= frames;
				
				if(counter > 0 && counter == frames-1)
					down = true;
				else if(counter == 0 || camera.x == 0)
					down = false;
				
			
				step+=(1*SCREEN_FPS/60);
			}
			else if(counter == frames-1)
				done = true;
			camera.x = counter*camera.w;
		}
		
		// should the image move?
		bool animate = true;
		
		bool down = false;
		
		// pause on the first frame 
		bool pause_first = false;
		
		// the current frame that we're on in the image 
		int counter = 0;
		
		bool done = false;
		
	private:
	
		// max width of the image
		int maxW;
		
		// the section of the image that we're looking at 
		SDL_Rect camera;
		
		// increment step for counting 
		int step = 0;
		
		// number of frames in image 
		int frames = 0;
		
		// the rate of each image frame per 
		int rate;
};

// input enumeration
enum input_type {	
					NONE = 0,
					EXIT = -1,
					
					SELECT = -2,
					CANCEL = -3,
					MENU = -4,
					
					UP = 1,
					DOWN = 2,
					LEFT = 3,	
					RIGHT = 4
				};

// input handler				
class input_handler
{
	public:
		input_handler()
		{
			e = new SDL_Event;
			state = NONE;
		}
		void update() // polls events 
		{
			input_type x = NONE;
			
			// get mouse info 
			SDL_GetMouseState( &mouseX, &mouseY );
			
			const Uint8* keyStates = SDL_GetKeyboardState(NULL);			
			if(keyStates[SDL_SCANCODE_SPACE])
				x = MENU;
		
			while(SDL_PollEvent(e)) // grabs input events 
			{			
				switch(e->type)
				{
					case SDL_WINDOWEVENT_MINIMIZED:
						while (SDL_WaitEvent(e))
						{
							if (e->window.event == SDL_WINDOWEVENT_RESTORED)
							{
								break;
							}
						}
					break;
					
					case SDL_KEYDOWN: // for keyboard input
						switch(e->key.keysym.sym)
						{
							case SDLK_RETURN:
								x = SELECT;
							break;
							case SDLK_BACKSPACE:
								x = CANCEL;
							break;
						
							case SDLK_d:
							case SDLK_RIGHT:
								x = RIGHT;
							break;
						
							case SDLK_a:
							case SDLK_LEFT:
								x = LEFT;
							break;
						
							case SDLK_w:
							case SDLK_UP:
								x = UP;
							break;
						
							case SDLK_s:
							case SDLK_DOWN:
								x = DOWN;
							break;
						}
					break;
					
					case SDL_JOYBUTTONDOWN: // for controller input
					
						switch(e->jbutton.button)
						{
							case SDL_CONTROLLER_BUTTON_DPAD_UP:
								x=UP;
							break;
							case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
								x=DOWN;
							break;
							case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
								x=LEFT;
							break;
							case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
								x=RIGHT;
							break;
							case SDL_CONTROLLER_BUTTON_A:
								x=SELECT;
							break;
							case SDL_CONTROLLER_BUTTON_B:
								x=CANCEL;
							break;
							case SDL_CONTROLLER_BUTTON_Y:
								x=MENU;
							break;
						}
					
					break;
	
					case SDL_WINDOWEVENT_CLOSE:
					case SDL_QUIT: // clicking the x window button
						x = EXIT;
					break;
				
					default:
						x = NONE;
					break;
				}
			}
			
			state = x;
			
		}
	
		input_type state;
		int mouseX, mouseY;
	private:
		SDL_Event * e; // gets event inputs		
};

class text 
{
	public:		
		text(){}
		text(SDL_Renderer * r,int fontType=0,int paramSize=40);
		
		// loading text letters with specific textures 
		void loadLetters();

		// for displaying text 
		void display(string t, int x=0, int y=0, double angle=0);
		
		int size = START_SIZE; // size of text 
		SDL_Color textColor = {255,255,255,255}; // text color 
		bool black = false; // does the text color just default to black?
		
	private:
		int height; // size of each letter (heigth * width)
		int width;
		
		// map of textures for letters, probably going to use a different data structure later (I hope)
		map<char,SDL_Texture*> letters; 
		
		SDL_Renderer * renderer; // renderer being used 

		SDL_Rect * renderQuad;
		
		string words;
		TTF_Font *font = NULL; // font information 
}; 

// class for handling general text information displaying
class textInfo
{
	public:
		text displayText;
		
		textInfo(){};
		textInfo(SDL_Renderer * g,int type = 0, int size=0);
		
		// start text timer
		void startTextDisplay();
		
		// add line of dialogue 
		void addLine(string s);
		
		// set current script to most recent 
		void reset();
		
		// display text 
		void display(int x, int y);
		
		// go to next line 
		void continueText();
		
		void deallocate();
		
		// can the player input stuff now that the lines are done?
		bool finishedLines = false;	
		
		// the text being displayed
		vector<string> lines = vector<string>();
		
		// is the line being read done?
		bool finishedLine = false;
		
		SDL_Renderer * target;
		
		// timer for keeping track of speed for displaying tex	t
		timer texttimer; 
		
		// rate variable for showing characters 
		int counter = 0;
		
		// current character in the string and the counter for what is displayed
		int currentPos = 0;
		
		// current line being read
		int currentScript = 0;	
};

// main game handler, general flow of the main game
class Window
{
	public:
		// PUBLIC FUNCTIONS 
		
		// initalize the game/constructor 
		Window();
		
		// display renderer to window 
		void display();
		
		// clear display 
		void clearDisplay();
		
		// ending game, deconstructor  
		void close();
		
		// PUBLIC VARIABLES 
		SDL_Renderer* renderer = NULL; // window renderer
		
		Uint8 colors[4] = {WHITE,0}; // background color of the window
		
		textInfo textHandler; // for displaying text 
		
	private:
		// did the window load successfully?
		bool success = false;
		SDL_Window* window = NULL; // window 
	
		SDL_Texture* texture = NULL; // displayed texture
	
		timer framekeeper; // timers used for frame rate information and capping framerate
		timer capkeeper;
	
		int frames = 0; // frame counter
		double avgFPS; // the average FPS calculated throughout the window operation
};


// universal input handler 
static input_handler input = input_handler();	

// stats class used by characters in the game
class character_stats;

// class layout for subsystems
class system_handler
{
	public:
		// constructor
		system_handler(){};
		
		// displaying the game system
		virtual void display(){};
	
		// handle the game system 
		virtual void handler(){};
		
		// deallocate memory 
		virtual void deallocate(){};
		
		bool systemEnd = false;
	protected:
		
		// the game system that the game is currently handling 
		Window * gameWindow;
};

class dungeon : public system_handler
{
	public:
		dungeon(){}
};

enum menuStat
{
	
};

class battle : public system_handler
{
	public:
		battle(){}
		battle(Window * g);
		
		// enemy AI actions
		void enemyAI();
		
		// handling the turns in combat 
		void turn_handler();
		
		// when the player selects a move for a party member 
		void player_selection();
		
		void display() override;
	
		void handler() override;
		
		void deallocate() override;

	private:
		// handling variables 
		
			// enemy stats
				character_stats * enemySide;
				int numEnemies;
			
			// overall combat turn order 
				vector<character_stats*> turnOrder;
			
			// target for commands/actions
				character_stats * target;
				int numTargets;
				int selectedTarget;
				
				int calculatedDamage = 0;
				
			// selected action option
				int menuOption = 0;
				int maxOptions = 5;
				
		// test variables 
			int monsterType = 0;
			bool switchMonster = false;		
				
		// assets used for UI

				// enemy sprites
				texture * enemy_sprites;
				
				// used for selecting enemy targets
				texture cursor;
				
				// menu icons 
				texture textBox;
				
				// background behind enemy sprites 
				texture battleBackground;
				
				// menu command UI 
				texture commandBox;
				
				// background for party status 
				texture partyBackground;
				
				// buff/debuff icons 
				texture buffs;
				
				// character status icons 
				texture statusBar;
};

class title_screen : public system_handler
{
	public:
		title_screen(){}
};
