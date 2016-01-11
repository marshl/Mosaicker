#include "common.h"

#include <iostream>

int GetFrameCount( bool _increment )
{
	static int frameCount = 0;
	if ( _increment )
	{
		frameCount++;
	}
	return frameCount;
}

// LoadBitmapFile
// desc: Returns a pointer to the bitmap image of the bitmap specified
//       by filename. Also returns the bitmap header information.
//		 No support for 8-bit bitmaps.
unsigned char* Common::LoadBitmapFile( const char* _filename, BITMAPINFOHEADER* _bitmapInfoHeader )
{
	FILE *filePtr;							// the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
	unsigned char		*bitmapImage;			// bitmap image data

	// open filename in "read binary" mode
	if ( fopen_s(&filePtr, _filename, "rb" ) )
	{
		std::cout << "Error opening file " << _filename << std::endl;
		return NULL;
	}

	// read the bitmap file header
	fread( &bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr );
	
	// verify that this is a bitmap by checking for the universal bitmap id
	if ( bitmapFileHeader.bfType != BITMAP_ID )
	{
		fclose( filePtr );
		return NULL;
	}

	// read the bitmap information header
	fread( _bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// move file pointer to beginning of bitmap data
	fseek( filePtr, bitmapFileHeader.bfOffBits, SEEK_SET );

	// allocate enough memory for the bitmap image data
	//bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);
	DWORD imageSize =  _bitmapInfoHeader->biBitCount / 8 * _bitmapInfoHeader->biWidth * _bitmapInfoHeader->biHeight ;
	bitmapImage = (unsigned char*)malloc( imageSize );

	// verify memory allocation
	if ( !bitmapImage )
	{
		free(bitmapImage);
		fclose(filePtr);
		std::cout << "Error allocating memory for image " << _filename << std::endl;
		return NULL;
	}

	// read in the bitmap image data
	fread( bitmapImage, 1, imageSize, filePtr );

	// make sure bitmap image data was read
	if ( bitmapImage == NULL )
	{
		fclose(filePtr);
		return NULL;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for ( DWORD imageIdx = 0; imageIdx < imageSize; imageIdx+=3)
	{
		unsigned char tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// close the file and return the bitmap image data
	fclose( filePtr );
	return bitmapImage;
}


unsigned int Common::LoadBitmapAsTexture( unsigned char* _bitmapData, BITMAPINFOHEADER* _bitmapInfoHeader )
{
	unsigned int texture;
	glGenTextures( 1, &texture );                  // generate texture object
	glBindTexture( GL_TEXTURE_2D, texture );       // enable our texture object

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

     // generate the texture image
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, _bitmapInfoHeader->biWidth,
				 _bitmapInfoHeader->biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _bitmapData );

	return texture;
}

float Common::GetDeltaTime()
{
	LARGE_INTEGER currentTime;           // ticks
	static LARGE_INTEGER frequency;
	static LARGE_INTEGER lastTime;
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &currentTime );
	float dt = float(currentTime.QuadPart - lastTime.QuadPart) / (float)frequency.QuadPart;
	lastTime = currentTime;
	return dt;
}