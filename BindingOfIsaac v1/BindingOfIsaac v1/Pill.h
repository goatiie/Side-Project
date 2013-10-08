#pragma once

#include "Vector.h"

class Pill
{
private:

	//the current position of the pill
	Vector position;

	//true = good pill, false = bad pill
	bool type;

	//the image data for the pill
	unsigned char* image;

public:

	Pill(bool type);
	Pill(void);
	~Pill(void);

	//get the type of the pilld
	bool getType(void);

	void update(void);

	void draw(void);
};

