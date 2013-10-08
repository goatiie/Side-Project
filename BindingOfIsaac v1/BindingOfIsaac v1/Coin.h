#pragma once

#include "Vector.h"

class Coin
{
private:

	//the current position of the coin
	Vector position;

	//the total worth of the coin
	int value;

	//the image data for the coin
	unsigned char* image;

public:

	Coin(int value);
	Coin(void);
	~Coin(void);

	//mutator methods
	void setPosition(float x, float y);
	Vector getPosition(void);
	int getValue(void);

	//update the coin, to see if has been picked up
	void update(void);

	//draw the coin
	void draw(void);
};

