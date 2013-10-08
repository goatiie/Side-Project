#pragma once

//the types of pickups
enum Type{Bomb, Coin, Heart, Pill};

class Pickups
{
private:

	//the amount of pickups per room
	Type* pickups;

	//pickup count
	int totalCount;

	//types of pickups count
	int bombCount;
	int coinCount;
	int heartCount;
	int pillCount;

public:

	Pickups(int bombCount, int coinCount, int heartCount, int pillCount);
	Pickups(void);
	~Pickups(void);
	
	//mutator methods
	int getTotalCount(void);
	int getBombCount(void);
	int getCoinCount(void);
	int getHeartCount(void);
	int getPillCount(void);
	

	//check for if isaac makes contact and is able to pick it up
	void update(Type p);

	//draw the pickup to screen
	void draw(Type p);
};

