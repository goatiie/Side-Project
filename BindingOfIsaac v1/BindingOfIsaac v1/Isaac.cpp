#include "Isaac.h"

Isaac::Isaac(int h, int s, int r, int aS, int d, int l, int e, int level)
{
	health = h;
	speed = s;
	range = r;
	attackSpeed= aS;
	damage = d;
	luck = l;
	experience = e;
	currentLevel = level;

	//set the deadflag to false, start alive
	deadFlag = false;

	
}

// Default Constructor 

Isaac::Isaac(void)
{
	// Debugging & Testing

	position[0] = 100;
	position[1] = 200;
}


Isaac::~Isaac(void)
{
}

//return the experience value
int Isaac::getExperience(void)
{
	return experience;
}

//set experience to the value given
void Isaac::setExperience(int e)
{
	experience = e;
}

//return the attackspeed value
int Isaac::getAttackSpeed(void)
{
	return attackSpeed;
}

//set attackSpeed to the value given
void Isaac::setAttackSpeed(int aS)
{
	attackSpeed = aS;
}

//return the damage value
int Isaac::getDamage(void)
{
	return damage;
}

//set damage to the value given
void Isaac::setDamage(int d)
{
	damage = d;
}

//return the range value
int Isaac::getRange(void)
{
	return range;
}

//set range to the value given
void Isaac::setRange(int r)
{
	range = r;
}

//return the current level value
int Isaac::getCurrentLevel(void)
{
	return currentLevel;
}

//set current level to the given value
void Isaac::setCurrentLevel(int level)
{
	currentLevel = level;
}

//return the luck value 
int Isaac::getLuck(void)
{
	return luck;
}

//set luck to the given value
void Isaac::setLuck(int l)
{
	luck = l;
}

//return the current health value
int Isaac::getHealth(void)
{
	return health;
}

//set health to the value given
void Isaac::setHealth(int h)
{
	health = h;
}

//return current speed value
int Isaac::getSpeed(void)
{
	return speed;
}

//set the speed to the value given
void Isaac::setSpeed(int s)
{
	speed = s;
}

//set the current frame number with the given value
void Isaac::setFrameNumber(int frame)
{
	frameNumber = frame;
}

//return the current frame number
int Isaac::getFrameNumber(void)
{
	return frameNumber;
}

//return the dead status of isaac
bool Isaac::getDeadFlag(void)
{
	return deadFlag;
}

//set the deadflag with the given value
void Isaac::setDeadFlag(bool b)
{
	deadFlag = b;
}

// Set Image Data
void Isaac::setSpriteSheet(unsigned char* bmpImage)
{
	image = bmpImage;
}


void Isaac::update(void)
{
	//update movement and frame number
}

void Isaac::draw(int xLoc, int yLoc)
{
	// Offset Data - can calculate x now
	int yOff;
	int xOff = frameNumber * 32;

	// Then calculate y offset and draw

	switch(dir)
	{
	case DOWN:

		yOff = 0;
		gfxInstance->drawBitmap(position[0], position[1], xOff, yOff, image);

		break;

	case LEFT:

		yOff = 32;
		gfxInstance->drawBitmap(position[0], position[1], xOff, yOff, image);

		break;

	case RIGHT:

		yOff = 64;
		gfxInstance->drawBitmap(position[0], position[1], xOff, yOff, image);

		break;

	case UP:

		yOff = 96;
		gfxInstance->drawBitmap(position[0], position[1], xOff, yOff, image);

		break;

	default:
		break;

	}

}