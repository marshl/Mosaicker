#include "colour.h"

#include <cmath>

Colour& Colour::operator += ( const Colour& other )
{
    this->r += other.r;
    this->g += other.g;
    this->b += other.b;
    return *this;
}

Colour Colour::operator / ( float _f )
{
    Colour c = *this;
    
    c.r /= _f;
    c.g /= _f;
    c.b /= _f;

    return c;
}

float ColourDiff( const Colour& _c1, const Colour& _c2 )
{
    return abs( _c2.r - _c1.r ) + abs( _c2.g - _c1.g ) + abs( _c2.b - _c1.b ); 
}
