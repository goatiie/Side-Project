//include neccessary libs
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <string.h>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include "include\glew.h"
//#include "include\openglut.h"
#include "include\glut.h"
#include "Shaders.h"

//screen width and height for the window
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

//thresholds for the marching squares algorithm
const float MAX_THRESHOLD = 1.010f;
const float MIN_THRESHOLD = 1.001f;

//PIE
#define PI 3.14159

//random function
#define rnd(x) (x * rand() / RAND_MAX)

//simple 2D vector struct
struct Vector
{
	int x, y;

	//overloading some operators for simple vector maths

	Vector operator-(Vector other)
	{
		Vector temp;
		temp.x = x - other.x;
		temp.y = y - other.y;
		return temp;
	}

	Vector operator+(Vector other)
	{
		Vector temp;
		temp.x = x + other.x;
		temp.y = y + other.y;
		return temp;
	}

	float magnitudeSquared()
	{
		return x * x + y * y;
	}

	float magnitude()
	{
		double temp = x * x + y * y;
		return sqrt(temp);
	}
};

//simple boundingBox struct
struct BoundingBox
{
	Vector topLeft, topRight, bottomRight, bottomLeft;
};

//declare the particle struct
struct Particle
{
	Vector position;
	Vector velocity;
	int radius;
	BoundingBox b;
	float equation(float x0, float y0)
	{
		return (radius / sqrt((x0 - position.x) * (x0 - position.x) +
			(y0 - position.y) * (y0 - position.y)));
	}
};

//simple line struct
struct Line
{
	Vector startPos;
	Vector endPos;

	//variables for elevation
	bool elevationActive;
	Vector frontelevationStart;
	Vector frontelevationEnd;
	Vector backelevationEnd;
	Vector backelevationStart;
	float elevationSpeed;
	int direction;

	//variables for rotation
	//center point for the lines
	Vector centerPoint;
	bool rotationActive;

	//static line, cant be deleted (loaded in level lines)
	bool staticLine;
};

//simple colour struct
struct Colour
{
	float r, g, b;
};

//zone struct for the start and finish zones
struct Zone
{
	Vector position[4];
};