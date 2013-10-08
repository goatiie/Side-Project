#pragma once

#include "Vector.h"

#include "stdafx.h"

//which direction frame to draw
enum Direction{LEFT, RIGHT, UP, DOWN};




class Isaac
{
private:

	//position of isaac
	Vector position;
	//how fast he will move in a certain direction
	Vector velocity;
	//how many hearts he will have
	int health;
	//how fast he moves
	int speed;
	//the range on the projectiles he shoots
	int range;
	//how fast he can shoot projectiles
	int attackSpeed;
	//how much damage the projectiles will do
	int damage;
	//luck of isaac
	int luck;
	//amount of experience
	int experience;
	//the current level of isaac
	int currentLevel;

	//frame number
	int frameNumber;

	//this will hold the current image data for isaac
	unsigned char* image;

	// Animation Info
	Direction dir;

	// Pointer to renderer instance
	
	RenderClass* gfxInstance;


	//whether the worm is dead or not
	bool deadFlag;

public:

	Isaac(int health, int speed, int range, int attackSpeed, int damage, int luck, int experience, int currentLevel);
	Isaac(void);
	~Isaac(void);

	//mutator methods 
	int getHealth(void);
	void setHealth(int h);
	int getDamage(void);
	void setDamage(int d);
	int getLuck(void);
	void setLuck(int l);
	int getExperience(void);
	void setExperience(int e);
	int getSpeed(void);
	void setSpeed(int s);
	int getRange(void);
	void setRange(int r);
	int getAttackSpeed(void);
	void setAttackSpeed(int aS);
	int getCurrentLevel(void);
	void setCurrentLevel(int level);

	int getFrameNumber(void);
	void setFrameNumber(int f);

	void setSpriteSheet(unsigned char* bmpImage);

	bool getDeadFlag(void);
	void setDeadFlag(bool d);

	//update movement
	void update(void);

	//draw isaac to screen
	void draw(int x, int y);



};