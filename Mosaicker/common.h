#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdlib>

#include <vector>
#include <list>

#define BITMAP_ID 0x4D42

namespace Common
{
    int GetFrameCount( bool _increment = false );

    unsigned char* LoadBitmapFile( const char* _filename, BITMAPINFOHEADER* _bitmapInfoHeader );

    unsigned int LoadBitmapAsTexture( unsigned char* _bitmapData, BITMAPINFOHEADER* _bitmapInfoHeader );

    float GetDeltaTime();
}