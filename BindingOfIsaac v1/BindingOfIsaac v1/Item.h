#pragma once

#include "Vector.h"

//the types of item that can occur, Trinket is a passive effect, Slot is the use item with an internal cooldown, Pickup has a one time effect
//when the player picks it up
enum ItemType{Trinket, Slot, PickUp};

class Item
{
private:

	//the position of the item
	Vector position;

	//the image data
	unsigned char* image;

	//name of the item
	char* name;

	//the internal cooldown if it is type Slot
	int cooldown;

	//the type of item
	ItemType type;

public:

	Item(float x, float y);
	Item(void);
	~Item(void);

	//update whether the item has been picked up or not
	void update(void);

	//set the cooldown 
	void setCooldown(int n);

	//draw the image data
	void draw(void);

};

