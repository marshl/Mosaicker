#ifndef COLOUR_H_
#define COLOUR_H_

class Colour
{
public:
    float r, g, b;

    Colour() : r( 0.0f ), g( 0.0f ), b( 0.0f ) { }
    Colour( float _r, float _g, float _b ) : r(_r), g(_g), b(_b) { }

    Colour& operator += ( const Colour& _other );

    Colour operator / ( float _f );
};

float ColourDiff( const Colour& _c1, const Colour& _c2 );

#endif