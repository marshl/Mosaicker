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

    ImageField( LONG width, LONG height );

    ~ImageField();

    Colour* GetPixel( LONG x, LONG y );

    Colour GetAverage( LONG x, LONG y, LONG tilesWide, LONG tilesHigh );

    void LoadData( unsigned char* data );
};

#endif