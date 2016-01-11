//#include "stdafx.h"
#include "vector2.h"

Vector2& Vector2::operator = (const Vector2& v)
{
	this->x = v.x;
	this->y = v.y;
	return *this;
}

bool Vector2::operator == (const Vector2& v) const
{
	return this->x == v.x && this->y == v.y;
}

bool Vector2::operator >> (const Vector2& v) const
{
	return this->x > v.x && this->y > v.y;
}
bool Vector2::operator << (const Vector2& v) const
{
	return this->y < v.y && this->y < v.y;
}

bool Vector2::operator > (const Vector2& v) const
{
	return this->x > v.x || this->y > v.y;
}
bool Vector2::operator < (const Vector2& v) const
{
	return this->y < v.y || this->y < v.y;
}
Vector2 Vector2::operator + (const Vector2& v) const
{
	return Vector2(this->x + v.x, this->y + v.y);
}
Vector2 Vector2::operator - (const Vector2& v )const
{
	return Vector2(this->x - v.x, this->y - v.y);
}
Vector2 Vector2::operator - () const
{
	return Vector2(-this->x, this->y);
}
Vector2 Vector2::operator * (float f) const
{
	return Vector2(this->x * f, this->y * f);
}
Vector2 Vector2::operator / (float f) const
{
	return Vector2(this->x / f, this->y / f);
}
	
Vector2& Vector2::operator += (const Vector2& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
} 
Vector2& Vector2::operator -= (const Vector2& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}
Vector2& Vector2::operator *= (float f)
{
	this->x *= f;
	this->y *= f;
	return *this;
}
Vector2& Vector2::operator /= (float f)
{
	this->x /= f;
	this->y /= f;
	return *this;
}

float Vector2::Dot(const Vector2& v) const
{
	return this->x * v.x + this->y * v.y;
}
float Vector2::Dot(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

Vector2 Vector2::Lerp( const Vector2& _v1, const Vector2& _v2, float _t )
{
	return _v1 + ( _v2 - _v1 ) * _t;
}

float Vector2::Magnitude() const
{
	return sqrt( this->x * this->x + this->y * this-> y);
}

float Vector2::SqrMagnitude() const
{
	return this->x * this->x + this->y * this->y;
}

float Vector2::Angle() const
{
	return atan2( y, x );
}

Vector2& Vector2::Normalise()
{
	float length = this->Magnitude();
	if ( length == 0 )
		return *this;
	float invLength = 1 / length;
	this->x *= invLength;
	this->y *= invLength;
	return *this;
}

Vector2 Vector2::Normalised() const
{
	return (*this) / this->Magnitude();
}

ostream& operator << (ostream& out, const Vector2& v)
{
	return out << '(' << v.x << ',' << v.y << ')';
}