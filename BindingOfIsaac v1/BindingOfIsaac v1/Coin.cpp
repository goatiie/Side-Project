#include "Coin.h"


Coin::Coin(int v)
{
	value = v;
}

Coin::Coin(void)
{
}


Coin::~Coin(void)
{
}

//set the position of the coin
void Coin::setPosition(float x, float y)
{
	position[0] = x;
	position[1] = y;
}

//return the current position of the coin
Vector Coin::getPosition(void)
{
	return position;
}

//return the value of the coin
int Coin::getValue(void)
{
	return value;
}

void Coin::update(void)
{
	//update whether the coin has been picked up or not
}

void Coin::draw(void)
{
	//draw the coin to screen
}