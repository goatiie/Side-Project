#include "stdafx.h"

//externs
extern RenderClass graphics;

void renderScene(void)
{
	//set a colour
	graphics.setColour(1, 1, 1);

	//set a pixel size
	graphics.setPointSize(2);

	//add stuff here to be rendered

	//render the scene
	graphics.render();
}

//keyboard process function for the game
void processKeyboard(unsigned char key, int x, int y)
{
	//process keyboard stuff in here
	switch(key)
	{
	default:
		{
			break;
		}
	}
}

//mouse processing function for the game
void processMouse(int button, int state, int x, int y)
{
	 //process mouse stuff in here
}


void processKeyUp(unsigned char btn, int x, int y)
{
	// process keyboard stuff here
}

//the update function for the game when it is idle
void update(void)
{

	// floor.update()


	//re render the scene when everything has finished updating
	renderScene();
}

//program entry point 
int main(int argc, char* argv[])
{
	graphics.create(argc, argv);

	srand((unsigned)time(0));

	//enter the game loop
	graphics.loop();

	return 0;
}