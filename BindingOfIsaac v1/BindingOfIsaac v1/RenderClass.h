#pragma once
class RenderClass
{
private:

	int width, height;
	int pointSize;
	float* buffer;
	RGB colour;

public:

	void create(int argc, char* argv[]);
	void loop();

	void clear();

	void render();
	void setColour(float, float, float);
	RGB getColour(int x, int y);
	void setPointSize(int);
	void drawPixel(int, int);
	bool friend operator==(RGB one, RGB two);


	void drawBitmap(int xPos, int yPos, int xOff, int yOff, unsigned char* bmp);

	//cirlce mid point drawing algorithms for drawing the explosions for the bombs
	//and the prjectiles
	void circleMidPoint(int centerX, int centerY, int radius);
	void circlePlotPoints(int centerX, int centerY, int circleX, int circleY);

	//this is the floodfill algorithm 
	void floodFill(int x, int y, RGB oldColour, RGB newColour);

	//line drawing algorithms
	//DDA
	void DDA(int x1, int y1, int x2, int y2);
	//brensenhams
	void Brensenhams(int x1, int y1, int x2, int y2);

	RenderClass(const int, const int);
	~RenderClass(void);
};

