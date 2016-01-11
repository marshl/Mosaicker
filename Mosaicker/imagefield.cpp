#include "imagefield.h"

#include <cmath>
#include <cassert>

#include "colour.h"

ImageField::ImageField( LONG _width, LONG _height )
{
	this->width = _width;
	this->height = _height;

	this->image = new Colour*[this->height];
	
	for ( LONG i = 0; i < this->height; ++i )
	{
		this->image[i] = new Colour[this->width];
	}
}

ImageField::~ImageField()
{
	free( this->data );
	for ( LONG i = 0; i < this->height; ++i )
	{
		delete this->image[i];
	}
	delete this->image;
	
}

Colour* ImageField::GetPixel( LONG _x, LONG _y )
{
	assert(_x >= 0 );
	assert( _x < this->width );
	assert( _y >= 0 );
	assert( _y < this->height );
	return &this->image[_y][_x];
}

/*
Colour ImageField::GetAverage( LONG _x, LONG _y, LONG _xOffset, LONG _yOffset )
{
	Colour total;
	int count = 0;
	for ( LONG y = _y; y < _y + _yOffset; ++y )
	{
		for ( LONG x = _x; x < _x + _xOffset; ++x )
		{
			Colour* c = this->GetPixel( x, y );

			if ( c->r != 1.0f || c->b != 0.0f || c->g != 1.0f )
			{
				total += *c;
				++count;
			}
			
		}
	}

	return count == 0 ? Colour(-1, -1, -1) : total / (float)count;
}*/

Colour ImageField::GetAverage( LONG _x, LONG _y, LONG _tilesWide, LONG _tilesHigh )
{
	Colour total;
	int count = 0;
	float cellWidth = (float)this->width /(float) _tilesWide;
	float cellHeight = (float)this->height / (float)_tilesHigh;

	for ( LONG y = (int)(float(_y) * cellHeight); y < int(float(_y+1)*cellHeight) && y < this->height; ++y )
	{
		for ( LONG x = (int)(float(_x) * cellWidth); x < int(float(_x+1)*cellWidth) && x < this->width; ++x )
		{
			Colour* c = this->GetPixel( x, y );
			// Ignore cells coloured magenta (transparent colour)
			if ( c->r != 1.0f || c->g != 0.0f || c->b != 1.0f )
			{
				total += *c;
				++count;
			}
		}
	}

	// Negative return values indicate an entirely empty cell
	return count == 0 ? Colour(-1, -1, -1) : total / (float)count;
}

void ImageField::LoadData( unsigned char* _data )
{
	this->data = _data;
	const size_t pixelSize = sizeof(char) * 3;
	/*for ( LONG y = 0; y < this->height; ++y )
	{
		for ( LONG x = 0; x < this->width; ++x )
		{
			unsigned char* p = _data + ( (y * this->height) + x ) * 3;
			unsigned char r = *p;
			unsigned char g = *(p+1);
			unsigned char b = *(p+2);
			
			this->image[y][x].r = float(r) / 255.0f;
			this->image[y][x].g = float(g) / 255.0f;
			this->image[y][x].b = float(b) / 255.0f;
		}
	}*/

	int x = 0, y = 0;

	//for ( int i = 0; i < this->bitmapInfo.biSize; )
	for ( int i = 0; ; )
	{
		unsigned char r = *(_data+i);
		unsigned char g = *(_data+i+1);
		unsigned char b = *(_data+i+2);

		this->image[y][x].r = float(r) / 255.0f;
		this->image[y][x].g = float(g) / 255.0f;
		this->image[y][x].b = float(b) / 255.0f;

		i += 3;
		++x;
		
		if ( x >= this->width )
		{
			++y;
			x = 0;
			if ( y >= this->height )
			{
				break;
			}
		}
	}
}