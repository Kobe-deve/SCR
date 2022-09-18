#ifndef SDL_MAIN_HANDLED
#include "systems.h"
#endif
#include <vector>
#include <iostream>
using namespace std;

class text 
{
	public:
		// loading text letters with specific textures 
		void loadLetters()
		{
			SDL_Texture ** texture = new SDL_Texture * [127]; // holds texture of text 
			string character = " ";
			
			// loads characters from value 29 to 125 
			for(int i = 29;i<126;i++)
			{
				character[0] = char(i);
				
				SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, character.c_str(), textColor );
				
				if(textSurface == NULL)
					printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
				else
				{
					//Create texture from surface pixels
					texture[i] = SDL_CreateTextureFromSurface(renderer, textSurface );
					
					if(texture[i] == NULL )
						printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
					else
					{
						width = textSurface->w;
						height = textSurface->h;
					}
					
					letters.insert(pair<char,SDL_Texture*>((char)i,texture[i]));
					
					//Get rid of old surface
					SDL_FreeSurface( textSurface );
				}
			}
		}
		text(){}
		text(SDL_Renderer * r,int fontType=0,int paramSize=40)
		{
			renderer = r;
			
			switch(fontType)
			{
				case 0:
				font = TTF_OpenFont("resources/fonts/ARCADE.ttf",paramSize);
				break;
				default:
				case 1:
				font = TTF_OpenFont("resources/fonts/chiaro.ttf",paramSize);	
				break;
				case 2:
				font = TTF_OpenFont("resources/fonts/Dotty.ttf",paramSize);	
				break;
			}
			
			if(font == NULL)
				cout << "Error with loading font!";
			renderQuad = new SDL_Rect;
			
			loadLetters();
		}
		
		// for displaying text 
		void display(string t, int x=0, int y=0, double angle=0)
		{
			SDL_Rect renderQuad;
			
			// char used for getting parts of the t string 
			char chara = ' ';
			
			for(int i=0;i<t.length();i++)
			{
				// render the text, changing the size if it's different from the default 
				renderQuad = {x+(i*width*abs(size-(START_SIZE-1))), y, width*abs(size-(START_SIZE-1)), height*abs(size-(START_SIZE-1))};
				
				// set current character to a part of the string 
				chara = t[i];
				
				// set text color 
				if(black)
					SDL_SetTextureColorMod(letters.at(chara), BLACK);
				else
					SDL_SetTextureColorMod(letters.at(chara), textColor.r, textColor.g, textColor.b);
				
				// present alpha and texture piece 
				SDL_SetTextureAlphaMod(letters.at(chara), textColor.a);
				SDL_RenderCopyEx(renderer, letters.at(chara), NULL, &renderQuad, angle,NULL,SDL_FLIP_NONE);
			
			}		
		}

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
		textInfo(SDL_Renderer * g,int type = 0, int size=0)
		{
			target = g;
			displayText = text(g,type,size);
		}
		
		// start text timer
		void startTextDisplay()
		{
			texttimer.start();
			currentPos = 0;
		}
		
		// add line of dialogue 
		void addLine(string s)
		{
			lines.push_back(s);
			finishedLines = false;	
		}
		
		// set current script to most recent 
		void reset()
		{
			if(lines.size() > 0)
				lines.clear();
			currentScript = 0;
			currentPos = 0;
			finishedLine = false;
			finishedLines = false;
		}
		
		void display(int x, int y)
		{
			int counter = (int)(20*texttimer.getTicks()/1000.f);		

			// if there are more characters, continue displaying new characters 
			if(counter > currentPos && currentPos < lines[currentScript].size())
				currentPos++;
			
			// checks if text is done 
			if(currentPos >= lines[currentScript].size())
			{
				finishedLine = true;
				texttimer.stop();
			}
		
			// checks if the text is done displaying 
			if((currentScript == lines.size()-1 || (currentScript == 0 && lines.size() == 0) )&& finishedLine)
				finishedLines = true;
			
			// displays the actual text 
			displayText.display(lines[currentScript].substr(0,currentPos),x,y);		
			
		}
		
		void continueText()
		{
			currentScript++;
			currentPos = 0;
			finishedLine = false;
			texttimer.start();
		}
		
		void deallocate()
		{
			delete &displayText;
		}
		
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