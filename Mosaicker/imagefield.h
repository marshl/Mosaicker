#ifndef IMAGEFIELD_H_
#define IMAGEFIELD_H_

#include <Windows.h>
#include <string>

#include "colour.h"

class ImageField
{
public:
	Colour** image;

	std::string filename;

	int texture;
	unsigned char* data;

	LONG height;
	LONG width;
	BITMAPINFOHEADER bitmapInfo;

	Colour average;

	int count;

	ImageField( LONG _width, LONG _height );
	ImageField();
	~ImageField();

	Colour* GetPixel( LONG _x, LONG _y );

	//Colour GetAverage( LONG _x, LONG _y, LONG _xOffset, LONG _yOffset );

	Colour GetAverage( LONG _x, LONG _y, LONG _tilesWide, LONG _tilesHigh );

	void LoadData( unsigned char* _data );
};

#endif