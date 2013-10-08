#include "Pill.h"


Pill::Pill(bool t)
{
	type = t;
}

Pill::Pill(void)
{
}


Pill::~Pill(void)
{
}

void Pill::update(void)
{
	//update to see if the pill has been picked up or not
	switch(type)
	{
	case true:
		{
			//randomly increment one of isaacs attributes
			//when picked up
			break;
		}
	case false:
		{
			//randomly decrement one of isaacs attributes
			//when picked up
			break;
		}
	}
}

void Pill::draw(void)
{
	//draw the pill to the screen
}

//return the type of the pill
bool Pill::getType(void)
{
	return type;
}