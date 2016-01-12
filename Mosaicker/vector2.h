#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <cmath>
#include <ostream>

using namespace std;

class Vector2
{
public:
    float x, y;    

    Vector2() : x(0.0f), y(0.0f) { }
    Vector2( float _x, float _y ) : x( _x ), y( _y ) { }

    void Set( float _x, float _y ) { x = _x; y = _y; }
    void Set( int _x, int _y ) { x = (float)_x; y = (float)_y; }
    void SetZero() { x = y = 0.0f; }

    float Angle() const;
    static Vector2 Rotation(float rot) { return Vector2(cos(rot), sin(rot)); }

    static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
    static Vector2 One() { return Vector2(1.0f, 1.0f); }

    Vector2& operator = (const Vector2& v);
    bool operator == (const Vector2& v) const;

    bool operator >> (const Vector2& v) const;
    bool operator << (const Vector2& v) const;

    bool operator > (const Vector2& v) const;
    bool operator < (const Vector2& v) const;

    Vector2 operator + (const Vector2& v)const; 
    Vector2 operator - (const Vector2& v)const;
    Vector2 operator - () const;
    Vector2 operator * (float f) const;
    Vector2 operator / (float f) const;
    
    Vector2& operator += (const Vector2& v); 
    Vector2& operator -= (const Vector2& v);
    Vector2& operator *= (float f);
    Vector2& operator /= (float f);

    float Dot(const Vector2& v) const;
    static float Dot(const Vector2& v1, const Vector2& v2);

    static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);
    float Magnitude() const;
    float SqrMagnitude() const;
    Vector2& Normalise();
    Vector2 Normalised() const;

    friend ostream& operator << (ostream& out, const Vector2& v);
};

#endif