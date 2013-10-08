#pragma once

#include <iostream>

//a 2D vector library
class Vector
{
private:

	float v[2];

public:
	Vector(float x, float y);
	Vector(void);
	~Vector(void);

	float &operator[](int index);
	float operator[](int index)const;

	Vector operator*(float scale) const;
	Vector operator/(float scale) const;
	Vector operator+(const Vector &other) const;
	Vector operator-(const Vector &other) const;
	Vector operator-(void) const; //turn the vector in the opposite direction (make it negative)

	const Vector &operator *=(float scale);
	const Vector &operator /=(float scale);
	const Vector &operator +=(const Vector &other);
	const Vector &operator -=(const Vector &other);

	float magnitude(void) const;
	float magnitudeSquared(void) const;
	Vector normalize(void) const;
	float dotProduct(const Vector &other) const;
};

