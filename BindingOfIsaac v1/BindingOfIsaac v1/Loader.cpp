#include "Loader.h"



Loader::Loader(void)
{
	// One time instantiations here

	tmpImage = loadBitmapImage("Sprites/skeleTest.bmp", &tmpHeader);

	player->setSpriteSheet(tmpImage);
}

Loader::~Loader(void)
{
}

//--------------------------------------------------------------------------------------------------
//	Bitmap Loader
//--------------------------------------------------------------------------------------------------

unsigned char* Loader::loadBitmapImage(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *imagePtr;							
	BITMAPFILEHEADER	bmpHeaderFile;	

	unsigned char		*bmpImage;			
	int					imageIndex = 0;		
	unsigned char		tempRGB;			

	// Open file
	
	imagePtr = fopen(filename, "rb");
	
	if (imagePtr == NULL)
	{
		return NULL;
	}

	// Read bitmap data & assign memory for image
	fread(&bmpHeaderFile, sizeof(bmpHeaderFile), 1, imagePtr);
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, imagePtr);
	fseek(imagePtr, bmpHeaderFile.bfOffBits, SEEK_SET);

	bmpImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);
	
	fread(bmpImage, 1, bitmapInfoHeader->biSizeImage, imagePtr);

	// Loop through image file - extract RGB - swap into correct locations

	for (imageIndex = 0; imageIndex < bitmapInfoHeader->biSizeImage; imageIndex+=3)
	{
		tempRGB = bmpImage[imageIndex];
		bmpImage[imageIndex] = bmpImage[imageIndex + 2];
		bmpImage[imageIndex + 2] = tempRGB;
	}

	// Close file & return data
	fclose(imagePtr);
	return bmpImage;
}