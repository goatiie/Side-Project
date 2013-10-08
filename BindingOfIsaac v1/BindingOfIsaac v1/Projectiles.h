#pragma once

#include "Vector.h"

class Projectiles
{
private:

	//vectors for the projectiles

	//direction of the projectile
	Vector direction;
	//where the projectile will fire from
	Vector origin;
	//the velocity of the projectile
	Vector velocity;
	//the target position for the projectile
	Vector targetPosition;

	//the speed of the projectile
	float speed;


public:

	Projectiles(void);
	~Projectiles(void);

	//set the origin
	void setOrigin(float x, float y);

	//set the velocity
	void setVelocity(void);

	//set the target position
	void setTargetPosition(float x, float y);

	//update the projectiles movement
	void update(void);

	//set the speed of the projectile
	void setSpeed(float s);

	//get the methods for the attributes of the projectile
	float getSpeed(void);
	Vector getOrigin(void);
	Vector getTargetPosition(void);
	Vector getVelocity(void);

	//get the distance vector
	Vector getDistanceVector(void);
};