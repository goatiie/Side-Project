#include "Bomb.h"


Bomb::Bomb(int c, int eR, int d)
{
	countdown = c;
	explosionRadius = eR;
	damage = d;
}

Bomb::Bomb(void)
{
}


Bomb::~Bomb(void)
{
}

int Bomb::getDamage(void)
{
	return damage;
}

int Bomb::getCountdown(void)
{
	return countdown;
}

int Bomb::getExplosionRadius(void)
{
	return explosionRadius;
}

Vector Bomb::getPosition(void)
{
	return position;
}

void Bomb::setPosition(float x, float y)
{
	position[0] = x;
	position[1] = y;
}

void Bomb::update(void)
{
	//update the bomb countdown and explosion
}

void Bomb::draw(void)
{
	//draw the bomb and the bomb explosion
}