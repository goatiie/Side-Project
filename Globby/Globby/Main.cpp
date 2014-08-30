#include "Helper.h"

//the shader object
Shaders* s;
//the array of particles
Particle *particles;
//the total number of particles
int numberOfParticles = 1;
//the array of pixel information
float *buffer;
//the size of the pixels 
int pointSize;

//the colour struct
Colour c;

//finish zone of each level
Zone finish; 

//the counts for how many of each line can be drawn
int rotateLineCount;
int elevationLineCount;
int normalLineCount;

//the current count for how many lines per type can be drawn
int currentRotateLineCount;
int currentElevationLineCount;
int currentNormalLineCount;

//mouse click count
int mouseClickCount = 0;

//variables for clipping lines
int code, lineStartCode, lineEndCode;

//variables for the clipping region
int top, bottom, left, right;
//variables for clickingRegions (elevation, rotating, drawing, deleting)
int drTop, drBottom, drLeft, drRight;
int rrTop, rrBottom, rrLeft, rrRight;
int erTop, erBottom ,erLeft, erRight;
int delTop, delBottom, delLeft, delRight;
int clearTop, clearBottom, clearLeft, clearRight;

//variables simulation clicking region
int simTop, simBottom, simLeft, simRight;

//variables for working out clipped lines remainder for drawing
float slope, slopeX, slopeY;

//gravity for the game
const float gravity = 2.0f;

//state variables
//for manipulating the lines (eg elavating, rotating, drawing lines)
bool rotLineMode = false;
bool eleLineMode = false;
bool drawLineMode = false;
bool deleteMode = false;

//to start/stop the simulation
bool simulationMode = true;

//temp line
Line l;

//define the line list
std::list<Line> lines;

//the number of lines for that level
int levelLineCount;

//methods
//drawing a pixel at a certain x,y position
void drawPixel(int, int);
//clear the buffer of last frame
void clearBuffer(void);
//edit information within the buffer
void drawIntoBuffer(void);
//the particlePotential algorithm
void particlePotential(void);
//circle mid point algorithm
void circlePlotPoints(int, int, int, int);
void circleMidPoint(int, int, int);
//brensenhams line drawing algorithm
void brensenhams(int, int, int, int);
//clipping method
void clipping(int, int, int, int);
//assigns the line code for the clipping method
int lineCode(int, int, int, int, int, int);
//method for drawing the region of the game
void drawRegion(void);
//method for converting degrees into radians
double degreesToRadians(float degrees);
//method for converting radians into degrees again
float radiansToDegrees(double radians);
//second update for updating the rotating lines at a slower
//rate, compared to the rest of the simulation
void secondUpdate(int);
//method for point line collision
bool pointLineCollision(int x, int y);
//method for reading in text files
void readTXTFile(char* filename);
//method for drawing the level
void drawLevel(void);
//method for the dotProduct
float dotProduct(Vector, Vector);
/*-----THINGS TO DO-----
apply proper movement to globby ie. gravity
start collision detection
level design

UPDATE
decided not to use marching squares since its needlessly complicated, just doing to use circle mid point algorithm
to draw globby, although he is not globby anymore but a perfectly drawn circle

*/

float dotProduct(Vector x, Vector y)
{
	return (x.x * y.x) + (x.y * y.y);
}

bool removeNonStaticLines(const Line& value)
{
	return (value.staticLine == false);
}

void drawLevel(void)
{
	//finish zone
	brensenhams(finish.position[0].x, finish.position[0].y, finish.position[1].x, finish.position[1].y);
	brensenhams(finish.position[1].x, finish.position[1].y, finish.position[2].x, finish.position[2].y);
	brensenhams(finish.position[2].x, finish.position[2].y, finish.position[3].x, finish.position[3].y);
}

void readTXTFile(char* filename)
{
	FILE *f = fopen(filename, "r");
	int i = 0;
	char c;

	Line tempLine;

	fscanf(f, "%d \n", &i);

	printf("Loaded in level: %d!\n", i);

	fscanf(f, "%d\n", &levelLineCount);

	printf("Line count: %d\n", levelLineCount);

	for(int k = 0; k < levelLineCount; k++)
	{
		fscanf(f, "%c", &c);

		if(c != 'L')
		{
			printf("Fail to read line: %d for level: %d!\n", k+1, i);
			exit(1);
		}

		fscanf(f, "%d %d %d %d \n" , &tempLine.startPos.x, &tempLine.startPos.y,
			&tempLine.endPos.x, &tempLine.endPos.y);

		//set the line as static, cant be deleted
		tempLine.staticLine = true;

		lines.push_back(tempLine);
	}

	fscanf(f, "%c", &c);

	if(c != 'C')
	{
		printf("Failed to read in line counts for level: %d!\n", i);
		exit(1);
	}

	fscanf(f, "%d %d %d\n", &normalLineCount, &elevationLineCount, 
		&rotateLineCount);

	fscanf(f, "%c", &c);

	if(c != 'S')
	{
		printf("Failed to read in particle starting position for the level: %d\n", i);
		exit(1);
	}

	fscanf(f, "%d %d\n", &particles[0].position.x, &particles[0].position.y);

	fscanf(f, "%c", &c);

	if(c != 'Z')
	{
		printf("Failed to read in Zone positions for the level: %d\n", i);
		exit(1);
	}

	fscanf(f, "%d %d %d %d %d %d %d %d %d\n", &finish.position[0].x, &finish.position[0].y, &finish.position[1].x,
		&finish.position[1].y, &finish.position[2].x, &finish.position[2].y, &finish.position[3].x,
		&finish.position[3].y);
	fclose(f);
}

bool pointLineCollision(int x, int y)
{
	//check all lines in the line list
	for(auto lineIt = lines.begin(); lineIt != lines.end(); ++lineIt)
	{
		float a, b, c, d;

		a = x - lineIt->startPos.x;
		b = y - lineIt->startPos.y;
		c = lineIt->endPos.x - lineIt->startPos.x;
		d = lineIt->endPos.y - lineIt->startPos.y;

		float dot = a * c + b * d;
		float len = c * c + d * d;
		float para = dot / len;

		float g, h;

		if(para < 0)
		{
			g = lineIt->startPos.x;
			h = lineIt->startPos.y;
		}
		else if(para > 1)
		{
			g = lineIt->endPos.x;
			h = lineIt->endPos.y;
		}
		else
		{
			g = lineIt->startPos.x + para * c;
			h = lineIt->startPos.y + para * d;
		}

		float xDif = x - g;
		float yDif = y - h;
		float hypo = sqrt((xDif * xDif) + (yDif * yDif));

		if(hypo < 2)
		{
			//deleting a line so need to add to the total
			//amount of allowed lines to be drawn in the game
			if(lineIt->elevationActive == true)
				currentElevationLineCount++;
			else if(lineIt->rotationActive == true)
				currentRotateLineCount++;
			else
				currentNormalLineCount++;
			
			lineIt = lines.erase(lineIt);
			return true;
		}
	}

	return false;
}

float radiansToDegrees(double radians)
{
	return (radians * 180) / PI;
}

double degreesToRadians(float degrees)
{
	return (degrees*PI) / 180;
}

void drawRegion(void)
{
	brensenhams(left, bottom, left, top);
	brensenhams(left, top, right, top);
	brensenhams(left, bottom, right, bottom);
	brensenhams(right, bottom, right, top);

	//blue
	c.r = 0;
	c.b = 255;
	c.g = 0;
	brensenhams(drLeft, drBottom, drLeft, drTop);
	brensenhams(drLeft, drTop, drRight, drTop);
	brensenhams(drLeft, drBottom, drRight, drBottom);
	brensenhams(drRight, drBottom, drRight, drTop);

	//green
	c.r = 0;
	c.b = 0;
	c.g = 255;
	brensenhams(erLeft, erBottom, erLeft, erTop);
	brensenhams(erLeft, erTop, erRight, erTop);
	brensenhams(erLeft, erBottom, erRight, erBottom);
	brensenhams(erRight, erBottom, erRight, erTop);

	//some random colour
	c.r = 100;
	c.b = 100;
	c.g = 100;
	brensenhams(rrLeft, rrBottom, rrLeft, rrTop);
	brensenhams(rrLeft, rrTop, rrRight, rrTop);
	brensenhams(rrLeft, rrBottom, rrRight, rrBottom);
	brensenhams(rrRight, rrBottom, rrRight, rrTop);

	//some random colour
	c.r = 50;
	c.g = 50;
	c.b = 50;
	brensenhams(simLeft, simBottom, simLeft, simTop);
	brensenhams(simLeft, simTop, simRight, simTop);
	brensenhams(simLeft, simBottom, simRight, simBottom);
	brensenhams(simRight, simBottom, simRight, simTop);

	c.r = 150;
	c.b = 100;
	c.g = 0;
	brensenhams(delLeft, delBottom, delLeft, delTop);
	brensenhams(delLeft, delTop, delRight, delTop);
	brensenhams(delLeft, delBottom, delRight, delBottom);
	brensenhams(delRight, delBottom, delRight, delTop);

	c.r = 60;
	c.b = 100;
	c.g = 150;
	brensenhams(clearLeft, clearBottom, clearLeft, clearTop);
	brensenhams(clearLeft, clearTop, clearRight, clearTop);
	brensenhams(clearLeft, clearBottom, clearRight, clearBottom);
	brensenhams(clearRight, clearBottom, clearRight, clearTop);

	//set it back to default
	c.r = 255;
	c.g = 0;
	c.b = 0;
}

int lineCode(int x, int y, int x1, int y1, int x2, int y2)
{
	int lineCode = 0;
	
	if(y > y1)
	{
		lineCode |= 8; //1000
	}

	if(y < y2)
	{
		lineCode |= 4; //0100
	}

	if(x > x2)
	{
		lineCode |= 2; //0010
	}

	if(x < x1)
	{
		lineCode |= 1; //0001
	}

	return lineCode;
}

void clipping(int x1, int y1, int x2, int y2)
{
	bool acceptLine = false;

	while(true)
	{
		if((lineStartCode | lineEndCode) == 0)
		{	
			//accept the line
			acceptLine = true;
			break;
		}
		else if(lineStartCode & lineEndCode)
			//reject the line
			break;
		
		else
		{
			if(lineStartCode != 0)
				code = lineStartCode;
			
			else
				code = lineEndCode;
			
			//if the line position is greater than the top of the clipping window
			if(code & 8)
			{
				slopeX = x1 + (x2 - x1) * (top - y1) / (y2 - y1);
				slopeY = top;
			}
			//if the line position is less than the bottom of the clipping window
			else if(code & 4)
			{
				slopeX = x1 + (x2 - x1) * (bottom - y1) / (y2 - y1);
				slopeY = bottom;
			}
			//if the line position is greater than the right of the clipping window
			else if(code & 2)
			{
				slopeY = y1 + (y2 - y1) * (right - x1) / (x2 - x1);
				slopeX = right;
			}
			//if the line position is less than the left of the clipping window
			else if(code & 1)
			{
				slopeY = y1 + (y2 - y1) * (left - x1) / (x2 - x1);
				slopeX = left;
			}

			if(code == lineStartCode)
			{
				x1 = slopeX;
				y1 = slopeY;
				lineStartCode = lineCode(x1, y1, left, top, right, bottom);
			}
			else
			{
				x2 = slopeX;
				y2 = slopeY;
				lineEndCode = lineCode(x2, y2, left, top, right, bottom);
			}
		}
	}

	//if both the start and end positions of the line are within the clipping window
	if(acceptLine)
		brensenhams(x1, y1, x2, y2);
}

void brensenhams(int x1, int y1, int x2, int y2)
{
	int w = x2 - x1;
    int h = y2 - y1;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;

    if (w<0)
	{
		dx1 = -1;
	} 
	else if (w>0) 
	{
		dx1 = 1;
	}

    if (h<0) 
	{
		dy1 = -1;
	}
	else if (h>0)
	{
		dy1 = 1;
	}

    if (w<0) 
	{
		dx2 = -1;
	}
	else if (w>0)
	{
		dx2 = 1;
	}

    int longest = abs(w);
    int shortest = abs(h);

    if (!(longest>shortest)) 
	{
        longest = abs(h);
        shortest = abs(w);
        if (h<0) 
		{
			dy2 = -1;
		} 
		else if (h>0) 
		{
			dy2 = 1;
		}

        dx2 = 0;            
    }

    int numerator = longest >> 1;

    for (int i=0;i<=longest;i++) 
	{
		drawPixel(x1, y1);
        numerator += shortest;

        if (!(numerator<longest)) 
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

void circlePlotPoints(int centerX, int centerY, int circleX, int circleY)
{
	drawPixel(centerX + circleX, centerY + circleY);
	drawPixel(centerX - circleX, centerY + circleY);
	drawPixel(centerX + circleX, centerY - circleY);
	drawPixel(centerX - circleX, centerY - circleY);
	drawPixel(centerX + circleY, centerY + circleX);
	drawPixel(centerX - circleY, centerY + circleX);
	drawPixel(centerX + circleY, centerY - circleX);
	drawPixel(centerX - circleY, centerY - circleX);
}

void circleMidPoint(int xCenter, int yCenter, int radius)
{
	int circleX;
	int circleY;
	int circleP;
	
	circleP  = 1 - radius;

	circleX = 0;
	circleY = radius;

	circlePlotPoints(xCenter, yCenter, circleX, circleY);

	while (circleX < circleY) 
	{
		circleX++;
		if(circleP < 0)
		{
			circleP += 2 * circleX + 1;
		}

		else
		{
			circleY--;
			circleP += 2 * (circleX - circleY) + 1;
		}

		circlePlotPoints(xCenter, yCenter, circleX, circleY);
	}
}

void particlePotential(void)
{
	//the energy value itself
	float temp = 0;

	int offset = 0;

	for(int i = 0; i < numberOfParticles; i++)
	{
		for(int j = particles[i].b.bottomLeft.x; j < particles[i].b.bottomRight.x; j++)
		{
			for(int k = particles[i].b.bottomLeft.x; k < particles[i].b.topLeft.x; k++)
			{
				drawPixel(j, k);

				//reset the energy value after checking a pixel
				temp = 0;

				//check pixels between bounding boxes
				for(int h = 0; h < numberOfParticles; h++)
				{
					temp += particles[h].equation(j, k);
				}

				//if energy value is between the two thresholds then draw that pixel
				if(temp >= MIN_THRESHOLD && temp <= MAX_THRESHOLD)
					drawPixel(j, k);
			}
		}
	}

	for(int i = 0; i < SCREEN_WIDTH; i++)
	{
		for(int j = 0; j < SCREEN_HEIGHT; j++)
		{
			temp = 0;

			for(int h = 0; h < numberOfParticles; h++)
			{
				temp += particles[h].equation(i, j);
			}

			if(temp >= MIN_THRESHOLD && temp <= MAX_THRESHOLD)
					drawPixel(i,j);
		}
	}
}

void drawPixel(int x, int y)
{
	int start, end;

	start = -(pointSize/2);
	end	= (int)((pointSize/2.0f) + 0.5);

	for( int i=start; i<end; i++ )
	{
		for( int j=start; j<end; j++ )
		{
			// reject values outside buffer range
			if( (x+j < 0) || (x+j >= SCREEN_WIDTH) )
				continue;

			if( (y+i < 0) || (y+i >= SCREEN_HEIGHT) )
				continue;

			buffer[(((y+i)*SCREEN_WIDTH+(x+j))*3) + 0] = c.r;
			buffer[(((y+i)*SCREEN_WIDTH+(x+j))*3) + 1] = c.g;
			buffer[(((y+i)*SCREEN_WIDTH+(x+j))*3) + 2] = c.b;
		}
	}
}

void clear(void)
{
	memset(buffer, 0, sizeof(float) * SCREEN_WIDTH * SCREEN_HEIGHT * 3);
}

void drawIntoBuffer(void)
{
	//render the lines
	for(auto lineIt = lines.begin(); lineIt != lines.end(); ++lineIt)
	{
		lineStartCode = lineCode(lineIt->startPos.x, lineIt->startPos.y,
			left, top, right, bottom);
		lineEndCode = lineCode(lineIt->endPos.x, lineIt->endPos.y,
			left, top, right, bottom);
		clipping(lineIt->startPos.x, lineIt->startPos.y, lineIt->endPos.x, lineIt->endPos.y);
	}

	drawLevel();

	drawRegion();

	for(int i = 0; i < numberOfParticles; i++)
	{
		//drawPixel(particles[i].b.topLeft.x, particles[i].b.topLeft.y);
		//drawPixel(particles[i].b.topRight.x, particles[i].b.topRight.y);
		//drawPixel(particles[i].b.bottomRight.x, particles[i].b.bottomRight.y);
		//drawPixel(particles[i].b.bottomLeft.x, particles[i].b.bottomLeft.y);
		circleMidPoint(particles[i].position.x, particles[i].position.y, particles[i].radius);
	}
}

void processMouse(int button, int state, int x, int y)
{
	//get the inverted Y
	int newY = SCREEN_HEIGHT - y;

	if(simulationMode)
	{
		//on first LMB click assign the first position of the line
		if(button == GLUT_LEFT_BUTTON && mouseClickCount == 0 && state == GLUT_UP)
		{
			l.startPos.x = x;
			l.startPos.y = newY;
			l.frontelevationEnd.x = x;
			l.frontelevationEnd.y = newY + 30;
			l.frontelevationStart.y = newY - 30;
			l.frontelevationStart.x = x;
			mouseClickCount = 1;
		}
		//on the second LMB click assign the second position of the line and then add it to
		//the line list
		else if(button == GLUT_LEFT_BUTTON && mouseClickCount == 1 && state == GLUT_UP)
		{
			//assign values to the line variables
			l.endPos.x = x;
			l.endPos.y = newY;
			mouseClickCount = 0;
			l.backelevationEnd.x = x;
			l.backelevationEnd.y = newY + 30;
			l.backelevationStart.y = newY - 30;
			l.backelevationStart.x = x;
			l.elevationSpeed = 1;
			l.centerPoint.x = (l.startPos.x + l.endPos.x) / 2;
			l.centerPoint.y = (l.startPos.y + l.endPos.y) / 2;

			//user made lines can be deleted
			l.staticLine = false;

			//set certain lines actives based on if the co-assigning button region is
			//active
			if(eleLineMode)
			{
				if(currentElevationLineCount > 0)
				{
					l.elevationActive = true;
					l.rotationActive = false;
					l.direction = 0;
					lines.push_back(l);
					currentElevationLineCount--;
				}
			}
			else 
				l.elevationActive = false;

			if(rotLineMode)
			{
				if(currentRotateLineCount > 0)
				{
					l.rotationActive = true;
					l.elevationActive = false;
					lines.push_back(l);
					currentRotateLineCount--;
				}
			}

			else
				l.rotationActive = false;
			
			if(drawLineMode)
			{
				if(currentNormalLineCount > 0)
				{
					l.rotationActive = false;
					l.elevationActive = false;
					lines.push_back(l);
					currentNormalLineCount--;
				}
			}
		}
	}
	else
		//make sure to reset the mouseClick count
		mouseClickCount = 0;

	//checking mouse input between the button regions
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && x >= drLeft && x <= drRight &&
		newY >= drBottom && newY <= drTop)
	{
		if(drawLineMode == false)
		{
			drawLineMode = true;
			rotLineMode = false;
			eleLineMode = false;
			deleteMode = false;
		}
		else
			drawLineMode = false;

		//reset the mouse click count
		mouseClickCount = 0;
	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && x >= erLeft && x <= erRight &&
		newY >= erBottom && newY <= erTop)
	{
		if(eleLineMode == false)
		{
			//cant have both modes on at the same time
			eleLineMode = true;
			rotLineMode = false;
			drawLineMode = false;
			deleteMode = false;
		}
		else
			eleLineMode = false;

		//reset the mouse click count
		mouseClickCount = 0;
	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && x >= rrLeft && x <= rrRight &&
		newY >= rrBottom && newY <= rrTop)
	{
		if(rotLineMode == false)
		{
			//cant have both mode on at the same time
			rotLineMode = true;
			eleLineMode = false;
			drawLineMode = false;
			deleteMode = false;
		}
		else
			rotLineMode = false;

		//reset the mouse click count
		mouseClickCount = 0;
	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && x >= simLeft && x <= simRight &&
		newY >= simBottom && newY <= simTop)
	{
		if(simulationMode == true)
		{
			simulationMode = false;
			deleteMode = false;
		}
		else
			simulationMode = true;

		//reset the mouse click count
		mouseClickCount = 0;
	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && x >= clearLeft && x <= clearRight &&
		newY >= clearBottom && newY <= clearTop)
	{
		//clear all the lines that have been drawn by the user
		lines.remove_if(removeNonStaticLines);

		currentNormalLineCount = normalLineCount;
		currentRotateLineCount = rotateLineCount;
		currentElevationLineCount = elevationLineCount;

		//reset the mouse click count
		mouseClickCount = 0;
	}

	//using the mouse input to delete lines when deleteMode is true
	if(deleteMode)
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			if(pointLineCollision(x, newY))
			{
				printf("Collision Detection Occured!\n");
			}
			else
			{
				printf("No Collision!\n");
			}
		}
	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && x >= delLeft && x <= delRight &&
		newY >= delBottom && newY <= delTop)
	{
		if(deleteMode == false)
		{
			deleteMode = true;
			rotLineMode = false;
			eleLineMode = false;
			drawLineMode = false;
		}
		else
			deleteMode = false;

		//reset the mouse click count
		mouseClickCount = 0;
	}
}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//particlePotential();

	drawIntoBuffer();

	glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_FLOAT, buffer);

	glutSwapBuffers();

	clear();
}

void winReshapeFunc(GLint w, GLint h)
{
	//tell opengl how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	//set the camera perspective
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:
		exit(1);
	}
}

void updateFunc(int value)
{
	//only process things when the simulation is running 
	if(simulationMode)
	{
		for(int i = 0; i < numberOfParticles; i++)
		{
			if(particles[i].position.x < (left + particles[i].radius))
			{
				particles[i].position.x++;
				particles[i].velocity.x *= -1;
			}

			if(particles[i].position.x > (right - particles[i].radius))
			{
				particles[i].position.x--;
				particles[i].velocity.x *= -1;
			}

			if(particles[i].position.y > (top - particles[i].radius))
			{
				particles[i].position.y--;
				particles[i].velocity.y *= -1;
			}

			if(particles[i].position.y < (bottom + particles[i].radius))
			{
				particles[i].position.y++;
				particles[i].velocity.y *= -1;
				//particles[i].velocity.y = 0;
			}

			//the particle line collision detection 
			for(auto lineIt = lines.begin(); lineIt != lines.end(); ++lineIt)
			{
				////compute the direction vector of the line
				//Vector d;
				//d.x = lineIt->endPos.x - lineIt->startPos.x;
				//d.y = lineIt->endPos.y - lineIt->startPos.y;

				////compute the vector from center of particle to line start
				//Vector f;
				//f.x = lineIt->startPos.x - particles[i].position.x;
				//f.y = lineIt->startPos.y - particles[i].position.y;

				///*
				//The intersection is found by the following parametric equation

				//this:
				//Px = Ex + TDx;
				//Py = Ey + TDy;
				//into:
				//(x - h)Squared + (y - k)Squared = RSquared
				//(h, k) = center of circle
				//x = x position of collision on line
				//y = y position of collision on line 
				//t = parameter of the line equation

				//combining the two equations you get the following:
				//tSquared * (d DOT d) + 2t* (f DOT d) + (f DOT f - RSquared) = 0

				//Below is code to solve the quadratic equation

				//*/

				//float a = dotProduct(d, d);
				//float b = 2 * dotProduct(f, d);
				//float c = dotProduct(f, f) - particles[i].radius * particles[i].radius;

				////compute the discriminant
				//float discriminant = b * b - 4 * a * c;

				//if(discriminant < 0)
				//{
				//	//no intersection
				//}
				//else
				//{
				//	//ray didnt totally miss sphere,
				//	//so there is a solution to the equation

				//	discriminant = sqrt(discriminant);

				//	//either solution may be on or off the ray so need to test both.
				//	//t1 is always the smaller value, because BOTH discriminant and a
				//	//are nonnegative.
				//	float t1 = (-b - discriminant) / (2 * a);
				//	float t2 = (-b + discriminant) / (2 * a);

				//	/*there a total of three hit cases 

				//	Impale
				//	-0-> = t1 hit, t2 hit
				//	Poke
				//	--|--> | = t1 hit, t2 > 1
				//	ExitWound
				//	| --|--> = t1 < 0, t2 hit

				//	Also there are three miss cases
				//	FallShort
				//	-> 0  = t1 > 1, t2 > 1
				//	Past
				//	0 -> = t1 < 0, t2 < 0
				//	CompletelyInside
				//	| -> | = t1 < 0, t2 > 1

				//	*/

				//	//poke and impale collision
				//	if(t1 >= 0 && t1 <=1)
				//	{
				//		//t1 is the intersection, and its closer than t2
				//		//since t1 uses -b - discriminant
				//		/*printf("Poke and Impale Collision!\n");

				//		particles[i].position.y++;
				//		particles[i].position.x++;
				//		particles[i].velocity.y *= -1;
				//		particles[i].velocity.x *= -1;*/
				//	}

				//	if(t1 >= 0 && t2 > 1)
				//	{
				//		printf("Poke Collision\n");
				//		//particles[i].position.y++;
				//		//particles[i].position.x++;
				//		particles[i].velocity.y *= -1;
				//		particles[i].velocity.x *= -1;
				//	}

				//	//exit wound collision
				//	if(t2 >= 0 && t2 <= 1)
				//	{
				//		//here t1 didnt intersect so we are either started
				//		//inside the sphere or completely past it
				//		printf("Exit Wound Collision!\n");

				//		//particles[i].position.y--;
				//		//particles[i].position.x--;
				//		particles[i].velocity.x *= -1;
				//		particles[i].velocity.y *= -1;
				//	}
				//}

				//another method for calculating an intersection between a line and a circle
				
				//closest point to the particle on the line
				Vector closest;

				//calculate the vector between start of the line and circle center
				Vector d;

				d.x = particles[i].position.x - lineIt->startPos.x;
				d.y = particles[i].position.y - lineIt->startPos.y;

				//calculate the direction vector of the line
				Vector f;

				f.x = lineIt->endPos.x - lineIt->startPos.x;
				f.y = lineIt->endPos.y - lineIt->startPos.y;

				//create the unit vector for F
				Vector unitF;

				float fLength = f.magnitude();

				float l = abs(fLength);

				unitF.x = f.x / l;
				unitF.y = f.y / l;

				//compute the projected vectors length
				float projLength = dotProduct(d, unitF);

				//quick checks for the ends of the line
				if(projLength < 0)
				{
					closest.x = lineIt->startPos.x;
					closest.y = lineIt->startPos.y;
				}

				if(projLength > fLength)
				{
					closest.x = lineIt->endPos.x;
					closest.y = lineIt->endPos.y;
				}

				//compute the actual projection vector and not just its length
				Vector proj;

				proj.x = projLength * unitF.x;
				proj.y = projLength * unitF.y;

				//convert this into world coordinates
				closest.x = lineIt->startPos.x + proj.x;
				closest.y = lineIt->startPos.y + proj.y;

				//compute the distance between the circle and the closest point
				//i.e. the orthogonal vector of the line
				Vector ortho;
				ortho.x = particles[i].position.x - closest.x;
				ortho.y = particles[i].position.y - closest.y;
				
				float mag = ortho.magnitude();

				//get the magnitude of this vector
				if(mag < particles[i].radius)
				{
					//collision 
					printf("Collision Detection has occured!\n");
					
					//time for the response
					/*Vector offset;

					offset.x = (particles[i].radius - mag) * (ortho.x / mag);
					offset.y = (particles[i].radius - mag) * (ortho.y / mag);

					particles[i].position.x += offset.x;
					particles[i].position.y += offset.y;*/
					particles[i].velocity.y *= -1;
					break;
				}
			}

			//particles[i].position.y -= gravity;

			/*particles[i].b.bottomLeft.x += particles[i].velocity.x;
			particles[i].b.bottomLeft.y += particles[i].velocity.y;
			particles[i].b.bottomRight.x += particles[i].velocity.x;
			particles[i].b.bottomRight.y += particles[i].velocity.y;
			particles[i].b.topRight.x += particles[i].velocity.x;
			particles[i].b.topRight.y += particles[i].velocity.y;
			particles[i].b.topLeft.x += particles[i].velocity.x;
			particles[i].b.topLeft.y += particles[i].velocity.y;*/

			particles[i].position.x += particles[i].velocity.x;
			particles[i].position.y += particles[i].velocity.y;
		}

		for(auto lineIt = lines.begin(); lineIt != lines.end(); ++lineIt)
		{
			if(lineIt->elevationActive == true)
			{
				//draw the elevation boundaries

				pointSize = 4;

				drawPixel(lineIt->frontelevationStart.x, lineIt->frontelevationStart.y);
				drawPixel(lineIt->frontelevationEnd.x, lineIt->frontelevationEnd.y);
				drawPixel(lineIt->backelevationStart.x, lineIt->backelevationStart.y);
				drawPixel(lineIt->backelevationEnd.x, lineIt->backelevationEnd.y);

				pointSize = 2;
			}

			if(lineIt->elevationActive == true && lineIt->direction == 0)
			{
				lineIt->startPos.y += lineIt->elevationSpeed;
				lineIt->endPos.y -= lineIt->elevationSpeed;
				if(lineIt->startPos.y >= lineIt->frontelevationEnd.y)
				{
					lineIt->direction = 1;
				}
			}
		
			if(lineIt->elevationActive == true && lineIt->direction == 1)
			{
				lineIt->startPos.y -= lineIt->elevationSpeed;
				lineIt->endPos.y += lineIt->elevationSpeed;
				if(lineIt->startPos.y <= lineIt->frontelevationStart.y)
				{
					lineIt->direction = 0;
				}
			}
		}
	}

	glutPostRedisplay();
	glutTimerFunc(1,updateFunc,0);
}

void secondUpdate(int value)
{
	if(simulationMode)
	{
		for(auto lineIt = lines.begin(); lineIt != lines.end(); ++lineIt)
		{
			if(lineIt->rotationActive == true)
			{
				//convert the angle from degrees to radians
				float theta = degreesToRadians(11.1);

				//get the cos and sin of the angle
				double cosang = cos(theta);
				double sinang = sin(theta);

				//rotate the line around the center point
				float tx1 = lineIt->startPos.x - lineIt->centerPoint.x;
				float ty1 = lineIt->startPos.y - lineIt->centerPoint.y;
				float tx2 = lineIt->endPos.x - lineIt->centerPoint.x;
				float ty2 = lineIt->endPos.y - lineIt->centerPoint.y;

				lineIt->startPos.x = (tx1 * cosang + ty1 * sinang) + lineIt->centerPoint.x;
				lineIt->startPos.y = (-tx1 * sinang + ty1 * cosang) + lineIt->centerPoint.y;

				lineIt->endPos.x = (tx2 * cosang + ty2 * sinang) + lineIt->centerPoint.x;
				lineIt->endPos.y = (-tx2 * sinang + ty2 * cosang) + lineIt->centerPoint.y;
			}
		}
	}

	glutTimerFunc(50, secondUpdate, 0);
}

void init()
{
	secondUpdate(100);

	s = new Shaders();
	particles = new Particle[numberOfParticles];

	//assign the clipping boundaries and clicking regions
	left = 50, top = 550, bottom = 50, right = 550;
	erLeft = 50, erTop = 40, erBottom = 10, erRight = 90;
	rrLeft = 100, rrTop = 40, rrBottom = 10, rrRight = 130;
	drLeft = 140, drTop = 40, drBottom = 10, drRight = 170;
	simLeft = 200, simTop = 40, simBottom = 10, simRight = 240;
	delLeft = 270, delTop = 40, delBottom = 10, delRight = 300;
	clearLeft = 330, clearTop = 40, clearBottom = 10, clearRight = 370;
	//set the colour for the pixels
	c.r = 255;
	c.g = 0;
	c.b = 0;

	unsigned int i;
	int totalCount = SCREEN_WIDTH * SCREEN_HEIGHT * 3;
	buffer = new float[totalCount];
	pointSize = 2;

	for(i = 0; i < (unsigned)totalCount; i++)
	{
		buffer[i] = 1;
	}

	for(i = 0; i < (unsigned)numberOfParticles; i++)
	{
		particles[i].position.x = 50 + (i*5);
		particles[i].position.y = 50 + (i*5);
		particles[i].velocity.x = rnd(2)+0.01;
		particles[i].velocity.y = rnd(2)+0.01;
		particles[i].radius = 20;

		//assign bounding box values per particle
		particles[i].b.topLeft.x = particles[i].position.x - particles[i].radius;
		particles[i].b.topLeft.y = particles[i].position.y + particles[i].radius;
		particles[i].b.bottomLeft.x = particles[i].position.x - particles[i].radius;
		particles[i].b.bottomLeft.y = particles[i].position.y - particles[i].radius;
		particles[i].b.bottomRight.x = particles[i].position.x + particles[i].radius;
		particles[i].b.bottomRight.y = particles[i].position.y - particles[i].radius;
		particles[i].b.topRight.x = particles[i].position.x + particles[i].radius;
		particles[i].b.topRight.y = particles[i].position.y + particles[i].radius;
	}

	readTXTFile("level1.txt");

	//assign the current counts of the types of lines
	currentNormalLineCount = normalLineCount;
	currentRotateLineCount = rotateLineCount;
	currentElevationLineCount = elevationLineCount;
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(100, 100);

	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	glutCreateWindow("Globby by Liam Callow");

	init();

	glutReshapeFunc(winReshapeFunc);

	glutDisplayFunc(render);

	glutKeyboardFunc(keyboardFunc);

	glutMouseFunc(processMouse);

	glutTimerFunc(25,updateFunc,0);

	glutMainLoop();
}