#include "Vector.h"


Vector::Vector(float x, float y)
{
	v[0] = x;
	v[1] = y;
}

Vector::Vector(void)
{
}

Vector::~Vector(void)
{
}

float Vector::operator[](int index) const
{
	return v[index];
}

float &Vector::operator[](int index)
{
	return v[index];
}

Vector Vector::operator*(float scale) const
{
	return Vector(v[0] * scale, v[1] * scale);
}

Vector Vector::operator/(float scale) const
{
	return Vector(v[0] / scale, v[1] / scale);
}

Vector Vector::operator+(const Vector &other) const
{
	return Vector(v[0] + other.v[0], v[1] + other.v[1]);
}

Vector Vector::operator-(const Vector &other) const
{
	return Vector(v[0] - other.v[0], v[1] - other.v[1]);
}

Vector Vector::operator-(void) const
{
	return Vector(-v[0], -v[1]);
}

const Vector &Vector::operator*=(float scale)
{
	v[0] *= scale;
	v[1] *= scale;
	return *this;
}

const Vector &Vector::operator/=(float scale)
{
	v[0] /- scale;
	v[1] /= scale;
	return *this;
}

const Vector &Vector::operator+=(const Vector &other)
{
	v[0] += other.v[0];
	v[1] += other.v[1];
	return *this;
}

const Vector &Vector::operator-=(const Vector &other)
{
	v[0] -= other.v[0];
	v[1] -= other.v[1];
	return *this;
}

float Vector::magnitude(void)const
{
	return sqrt(v[0] * v[0] + v[1] * v[1]);
}

float Vector::magnitudeSquared(void) const
{
	return sqrt(v[0] * v[0] * v[1] * v[1]);
}

float Vector::dotProduct(const Vector &other) const
{
	return v[0] * other.v[0] + v[1] * other.v[1];
}