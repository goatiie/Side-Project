#include "stdafx.h"

//declarations

RenderClass graphics(SCREEN_WIDTH, SCREEN_HEIGHT);

RenderClass::RenderClass(const int w, const int h)
{
	width = w;
	height = h;

	int totalCount = w * h * 3;

	buffer = new float[totalCount];

	for(unsigned int i = 0; i < totalCount; i++)
	{
		buffer[i] = 0;
	}

	pointSize = 1;
}

//main game loop
void RenderClass::loop()
{
	glutMainLoop();
}

//DDA line drawing algorithm
void RenderClass::DDA(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	float m = dy/dx;

	for(unsigned int x = x1; x < x2; x++)
	{
		int y = m * x + y1 + 0.5;
		drawPixel(x, y);
	}
}

//circle mid point algorithm
void RenderClass::circleMidPoint(int centerX, int centerY, int radius)
{
	int circleX, circleY, circleP;

	circleP = 1 - radius;

	circleX = 0;
	circleY = radius;

	circlePlotPoints(centerX, centerY, circleX, circleY);

	while(circleX < circleY)
	{
		circleX++;
		if(circleP < 0)
		{
			circleP += 2 * circleX + 1;
		}
		else
		{
			circleY--;
			circleP += 2 * circleX - circleY + 1;
		}

		circlePlotPoints(centerX, centerY, circleX, circleY);
	}
}

//brensenhams line drawing algorithm
void RenderClass::Brensenhams(int x1, int y1, int x2, int y2)
{
	int w = x2 - x1;
	int h = y2 - y1;
	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

	if(w < 0)
	{
		dx1 = -1;
		//dx2 = -1;
	}
	else if(w > 0)
	{
		dx1 = 1;
		//dx2 = 1;
	}

	if(h < 0)
	{
		dy1 = -1;
	}
	else if(h > 0)
	{
		dy1 = 1;
	}

	if(w < 0)
	{
		dx2 = -1;
	}
	else if(w > 0)
	{
		dx2 = 1;
	}

	int longest = abs(w);
	int shortest = abs(h);

	if(!(longest > shortest))
	{
		longest = abs(h);
		shortest = abs(w);
		if(h < 0)
		{
			dy2 = -1;
		}
		else if(h > 0)
		{
			dy2 = 1;
		}

		dx2 = 0;
	}

	int numerator = longest >> 1;

	for(unsigned int i = 0; i <= longest; i++)
	{
		drawPixel(x1, y1);
		numerator += shortest;

		if(!(numerator < longest))
		{
			numerator -= longest;
			x1 += dx1;
			y1 += dy1;
		}
		else
		{
			x1 += dx2;
			y1 += dy2;
		}
	}
}

//method for plotting the points of the circle provided from 
void RenderClass::circlePlotPoints(int centerX, int centerY, int circleX, int circleY)
{
	graphics.drawPixel(centerX + circleX, centerY + circleY);
	graphics.drawPixel(centerX - circleX, centerY + circleY);
	graphics.drawPixel(centerX + circleX, centerY - circleY);
	graphics.drawPixel(centerX - circleX, centerY - circleY);
	graphics.drawPixel(centerX + circleX, centerY + circleY);
	graphics.drawPixel(centerX - circleX, centerY + circleY);
	graphics.drawPixel(centerX + circleX, centerY - circleY);
	graphics.drawPixel(centerX - circleX, centerY - circleY);
}

//flood fill algorithm for colouring in shapes
void RenderClass::floodFill(int x, int y, RGB oldColour, RGB newColour)
{
	//temp colour struct for getting the current
	//colour of the x y co-ordinates
	RGB temp;

	setColour(newColour.r, newColour.g, newColour.b);

	if(oldColour == newColour)
	{
		return;
	}

	int y1;
	y1 = y;

	temp = getColour(x, y1);

	//draw current scanline from start position to the top
	while(temp == oldColour)
	{
		drawPixel(x, y1);
		y1++;
		temp = getColour(x, y1);
	}

	y1 = y - 1;
	temp = getColour(x, y1);

	//draw current scanline from the start position to the bottom
	while(temp == oldColour)
	{
		drawPixel(x, y1);
		y1--;
		temp = getColour(x, y1);
	}

	y1 = y;
	temp = getColour(x, y1);

	//test for new scanlines to the left
	while(temp == newColour)
	{
		 temp = getColour(x - 1, y1);
		 if(temp == oldColour)
		 {
			 floodFill(x - 1, y1, oldColour, newColour);
		 }
		 y1++;
		 temp = getColour(x, y1);
	}

	y1 = y - 1;
	temp = getColour(x, y1);

	while(temp == newColour)
	{
		temp = getColour(x - 1, y1);
		if(temp == oldColour)
		{
			floodFill(x - 1, y1, oldColour, newColour);
		}
		y1--;
		temp = getColour(x, y1);
	}

	y1 = y;
	temp = getColour(x, y1);

	//test fr new scanlines to the right
	while(temp == newColour)
	{
		temp = getColour(x+1, y1);
		if(temp == oldColour)
		{
			floodFill(x + 1, y1, oldColour, newColour);
		}
		y1++;
		temp = getColour(x, y1);
	}

	y1 = y - 1;
	temp = getColour(x, y1);

	while(temp == newColour)
	{
		temp = getColour(x + 1, y1);
		if(temp == oldColour)
		{
			floodFill(x + 1, y1, oldColour, newColour);
		}
		y1--;
		temp = getColour(x, y1);
	}
}

//initialise the window using glut
void RenderClass::create(int argc, char* argv[])
{
	//initialise the glut library
	glutInit(&argc, argv);

	//set up the initial display mode
	//need both double buffering and z-buffering
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//set the initial window position
	glutInitWindowPosition(100, 100);

	//set the initial window size
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//create and name the widow
	glutCreateWindow("Binding of Isaac Remake v1");

	//reshape callback for current window
	glutReshapeFunc(winReshapeFunc);

	//set the display callback for the current window
	glutDisplayFunc(renderScene);

	//set the mouse callback for the current window
	glutMouseFunc(processMouse);

	//set the keyboard callback for the current window
	glutKeyboardFunc(processKeyboard);

	//to handle the key up methods for the current window
	glutKeyboardUpFunc(processKeyUp);

	//set the global idle callback 
	glutIdleFunc(update);
}




//clear out the render buffer
void RenderClass::clear()
{
	memset(buffer, 0, sizeof(float) * width * height * 3);
}

//set the pixel size 
void RenderClass::setPointSize(int size)
{
	pointSize = size;
}

//set the rgb values from the colour struct to the buffer based on the position 
//given on the screen
void RenderClass::drawPixel(int x, int y)
{
	int start, end;

	start = -(pointSize / 2);
	end = (int)((pointSize / 2.0f) + 0.5);

	for(unsigned int i = start; i < end; i++)
	{
		for(unsigned int j = start; j < end; j++)
		{
			if((x + j < 0) || (x + j >= width))
				continue;
			if((y + i < 0) || (y + i >= height))
				continue;
			
			buffer[(((y+i)* width + (x + j)) * 3) + 0] = colour.r;
			buffer[(((y+i)* width + (x + j)) * 3) + 1] = colour.b;
			buffer[(((y+i)* width + (x + j)) * 3) + 2] = colour.g;
		}
	}
}

void RenderClass::render()
{
	//clear the back buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw pixels
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, buffer);

	//swap the buffers of the current window
	glutSwapBuffers();

	//clear out the buffer
	clear();
}

//get a pixel from the screen and return its respected
//r, g, b values
RGB RenderClass::getColour(int x, int y)
{
	RGB colour;

	colour.r = buffer[(((x) * width + (y)) * 3) + 0];
	colour.g = buffer[(((x) * width + (y)) * 3) + 1];
	colour.b = buffer[(((x) * width + (y)) * 3) + 2];

	return colour;
}

//bool checker for comparing to pixel colour values are the same or not
bool operator==(RGB one, RGB two)
{
	if(one.r == two.r && one.g == two.g && one.b == two.b)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//set the colour for the pixel buffer
void RenderClass::setColour(float r, float g, float b)
{
	colour.r = r;
	colour.g = g;
	colour.b = b;
}

//gets called initially and whenever the window gets resized
//resizing has been locked
void winReshapeFunc(GLint w, GLint h)
{
	//specify the current matrix
	glMatrixMode(GL_PROJECTION);

	//load the identity matrix
	glLoadIdentity();

	//create a projection matrix (2D projection onto xy plane)
	glOrtho(-SCREEN_WIDTH, SCREEN_WIDTH, -SCREEN_HEIGHT, SCREEN_HEIGHT, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//set up the viewport
	glViewport(0, 0, (GLsizei) SCREEN_WIDTH, (GLsizei) SCREEN_HEIGHT);
}





RenderClass::~RenderClass(void)
{
}


void RenderClass::drawBitmap(int xLocation, int yLocation, int xOffset, int yOffset, unsigned char* image)
{
	// Loop counters
	int i;
	int j;

	// Drawing Position Variables
	int xDraw = xLocation;
	int yDraw = yLocation;

	RGB newCol;

	for(i = yOffset * 3; i < 32 * 3 + yOffset * 3; i+=3)
	{
		for(j = xOffset * 3; j < 32 * 3 + xOffset * 3; j+=3)
		{
			// Set new colour according to array data
			
			newCol.r = image[i * 32 + j];
			newCol.g = image[i * 32 + j + 1];
			newCol.b = image[i * 32 + j + 2];

			// Convert from 255-0 to 1-0
			newCol.r = newCol.r/255;
			newCol.g = newCol.g/255;
			newCol.b = newCol.b/255;

			if(newCol == TRANSCOLOUR)
			{
				// Do nothing				
			}
			else
			{
				setColour( newCol.r, newCol.g, newCol.b);
				drawPixel(xDraw,yDraw);
			}
			// increment drawing location
			xDraw++;
		}
		// Completed Line - reset to left and move down 1
		xDraw = xLocation;
		yDraw++;
	}

}