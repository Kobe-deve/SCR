#ifndef SDL_MAIN_HANDLED
#include "systems.h"
#endif
#include <vector>
#include <iostream>
using namespace std;


void text::loadLetters()
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
};
		
text::text(SDL_Renderer * r,int fontType,int paramSize)
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
};
		
// for displaying text 
void text::display(string t, int x, int y, double angle)
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
};