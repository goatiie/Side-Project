#include "Pickups.h"


Pickups::Pickups(int bCount, int hCount, int cCount, int pCount)
{
	bombCount = bCount;
	coinCount = cCount;
	heartCount = hCount;
	pillCount = pCount;

	totalCount = bombCount + pillCount + heartCount + coinCount;
}

Pickups::Pickups(void)
{
}


Pickups::~Pickups(void)
{
}

//draw the type of pickup
void Pickups::draw(Type p)
{
	switch(p)
	{
	case Pill:
		{
			//draw the pill
			break;
		}
	case Coin:
		{
			//draw the coin
			break;
		}
	case Heart:
		{
			//draw the heart 
			break;
		}
	case Bomb:
		{
			//draw the bomb 
			break;
		}
	}
}

//update which type of pickup 
void Pickups::update(Type p)
{
	switch(p)
	{
	case Bomb:
		{
			//update the bomb
			break;
		}
	case Coin:
		{
			//update the coin
			break;
		}
	case Heart:
		{
			//update the heart
			break;
		}
	case Pill:
		{
			//update the pill
			break;
		}
	}
}

