#pragma once

#include "Vector.h"

class Bomb
{
private:

	//how long it takes for the explosion to happen
	//after being planted
	int countdown;
	//the size of the explosion
	int explosionRadius;
	//the current position
	Vector position;
	//directional velocity vector
	//Vector velocity;
	//the amount of damage it will deal
	int damage;

	//the image data for the bomb
	unsigned char* image;

public:

	Bomb(int countDown, int explosionRadius, int damage);
	Bomb(void);
	~Bomb(void);

	//update the bombs movement and explosion
	void update(void);

	//draw the bomb and explosion of the bomb
	void draw(void);

	//mutator methods
	void setPosition(float x, float y);
	//void setVelocity(void);
	Vector getPosition(void);
	int getDamage(void);
	int getExplosionRadius(void);
	int getCountdown(void);
};

