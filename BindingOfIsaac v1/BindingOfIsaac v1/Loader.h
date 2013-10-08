#pragma once

#include <stdio.h>
// Windows Libs - needed for bmp
#include <Windows.h>
//the openal libraries
#include "includes\al.h"
#include "includes\alut.h"
#include "includes\alc.h"

// Game Includes
#include "Isaac.h"

class Loader
{
private:

	Isaac *player;

	//attributes for the openal sound segment
	int numberOfBuffers;
	int numberOfSources;
	ALuint* buffers;
	ALuint* sources;
	int countBuffers;
	int countSources;
	ALfloat listenerPos[3];
	ALfloat listenerVel[3];
	ALfloat listenerOri[6];

	// Temporary Storage Variables
	BITMAPINFOHEADER tmpHeader;
	unsigned char* tmpImage;


public:

	Loader(int buffer, int source);
	Loader(void);
	~Loader(void);

	unsigned char* loadBitmapImage(char*, BITMAPINFOHEADER*);

	////////////////////////////////////////////////////////////////////////////////
	//all types of methods for openal sound (ripped straight out of the sound class)
	////////////////////////////////////////////////////////////////////////////////
	void setListenerPos(float, float, float);
	float* getListenerPos(void);
	void setListenerVel(float, float, float);
	float* getListenerVel(void);
	void setListenerOri(float, float, float, float, float, float);
	float* getListenerOri(void);
	ALuint* getBuffers(void);
	ALuint* getSources(void);
	//create a buffer from a file and add it to the buffer array
	int addBuffer(char*);
	//play a specific source from the sources array
	void playSource(int Source);
	//checks if the specified buffer is running
	bool isPlaying(int buffer);
	//add sources to the source array and assign the specified buffer
	int addSources(int buffer, float* sourcePos, float* sourceVel, bool loop);
	//checks what type of error has been handed to it and prints it out
	static void printAlError(ALenum error);
	//stops the specified source from playing
	void stopSource(int source);
	ALfloat* getSourcePos(int source);
	ALfloat* getSourceVel(int source);
	//this function adds a source with the specified shared buffer
	int sharedBuffer(int sharingBuffer, int newBuffer, float* sourcePos, float* sourceVel, bool loop);
};

