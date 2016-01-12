#include "imagefield.h"

#include <cmath>
#include <cassert>

#include "colour.h"

ImageField::ImageField( LONG width, LONG height )
{
    this->width = width;
    this->height = height;

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

Colour* ImageField::GetPixel( LONG x, LONG y )
{
    assert(x >= 0 );
    assert(x < this->width );
    assert(y >= 0 );
    assert(y < this->height );
    return &this->image[y][x];
}

Colour ImageField::GetAverage( LONG startX, LONG startY, LONG tilesWide, LONG tilesHigh )
{
    Colour total;
    int count = 0;
    float cellWidth = (float)this->width / (float)tilesWide;
    float cellHeight = (float)this->height / (float)tilesHigh;

    for ( LONG y = (int)(float(startY) * cellHeight); y < int(float(startY+1)*cellHeight) && y < this->height; ++y )
    {
        for ( LONG x = (int)(float(startX) * cellWidth); x < int(float(startX+1)*cellWidth) && x < this->width; ++x )
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

void ImageField::LoadData( unsigned char* data )
{
    this->data = data;
    const size_t pixelSize = sizeof(char) * 3;

    int x = 0, y = 0;

    for ( int i = 0; ; )
    {
        unsigned char r = *(data+i);
        unsigned char g = *(data+i+1);
        unsigned char b = *(data+i+2);

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