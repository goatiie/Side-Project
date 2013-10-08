#include "Item.h"


Item::Item(void)
{
}


Item::~Item(void)
{
}

void Item::setItemName(char* n)
{
	name = n;
}

char* Item::getItemname(void)
{
	return name;
}

void Item::setCooldown(int n)
{
	cooldown = n;
}

int Item::getCooldown(void)
{
	return cooldown;
}

void Item::setItemPosition(float x, float y)
{
	position[0] = x;
	position[1] = y;
}

Vector Item::getItemPosition(void)
{
	return position;
}

void Item::setItemType(ItemType t)
{
	type = t;
}

ItemType Item::getItemType(void)
{
	return type;
}