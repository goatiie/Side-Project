#include "Projectiles.h"


Projectiles::Projectiles(void)
{
	speed = 0.0f;
}


Projectiles::~Projectiles(void)
{
}

//set the starting position for the projectile
void Projectiles::setOrigin(float x, float y)
{
	origin[0] = x;
	origin[1] = y;
}

//set the directional velocity vector for the projectiles
void Projectiles::setVelocity(void)
{
	//working out the rotation of the velocity vector
	//and its length
	float rot;
	float toRadians = 3.14 / 180;
	float dx = targetPosition[0] - origin[0];
	float dy = targetPosition[1] - origin[1];

	rot = (atan2(dy, dx) * (57.2957795130823)) + 90;

	velocity[0] = cos(((rot - 90) * toRadians)) * speed;
	velocity[1] = sin(((rot - 90) * toRadians)) * speed;
}

//set where the projectile is going to go 
void Projectiles::setTargetPosition(float x, float y)
{
	targetPosition[0] = x;
	targetPosition[1] = y;
}

//update the projectiles movement 
void Projectiles::update(void)
{
	origin += velocity;
}

//get the speed value
float Projectiles::getSpeed(void)
{
	return speed;
}

//get the distance between the start and end point of the projectile
Vector Projectiles::getDistanceVector(void)
{
	Vector distanceVector;

	distanceVector = targetPosition - origin;

	return distanceVector;
}

//set the speed of the projectile
void Projectiles::setSpeed(float f)
{
	speed = f;
}

//get the starting position of the projectile
Vector Projectiles::getOrigin(void)
{
	return origin;
}

//get the target position for the projectile
Vector Projectiles::getTargetPosition(void)
{
	return targetPosition;
}

//get the directional velocity vector
Vector Projectiles::getVelocity(void)
{
	return velocity;
}