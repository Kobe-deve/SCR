#include "text.h"

textInfo::textInfo(SDL_Renderer * g,int type, int size)
{
	target = g;
	displayText = text(g,type,size);
}
		
// start text timer
void textInfo::startTextDisplay()
{
	texttimer.start();
	currentPos = 0;
}
		
// add line of dialogue 
void textInfo::addLine(string s)
{
	lines.push_back(s);
	finishedLines = false;	
}
		
// set current script to most recent 
void textInfo::reset()
{
	if(lines.size() > 0)
		lines.clear();
	currentScript = 0;
	currentPos = 0;
	finishedLine = false;
	finishedLines = false;
}
		
void textInfo::display(int x, int y)
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
			
};
		
void textInfo::continueText()
{
	currentScript++;
	currentPos = 0;
	finishedLine = false;
	texttimer.start();
};
		
void textInfo::deallocate()
{
	delete &displayText;
};