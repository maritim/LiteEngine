#include <iostream>
#include <cmath>

#include "Vector3.h"

Vector3::Vector3(void) :
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
	// Nothing
}

Vector3::Vector3(float newX, float newY, float newZ) :
	x(newX),
	y(newY),
	z(newZ)
{
	// Nothing
}

Vector3::Vector3(const Vector3& other) :
	x (other.x),
	y (other.y),
	z (other.z)
{

}

// Constant Vectors that I use more often
Vector3 Vector3::Up(0.0, 1.0, 0.0);
Vector3 Vector3::Down(0.0, -1.0, 0.0);
Vector3 Vector3::Left(-1.0, 0.0, 0.0);
Vector3 Vector3::Right(1.0, 0.0, 0.0);
Vector3 Vector3::Back(0.0, 0.0, -1.0);
Vector3 Vector3::Forward(0.0, 0.0, 1.0);
Vector3 Vector3::One(1.0, 1.0, 1.0);
Vector3 Vector3::Zero(0.0, 0.0, 0.0);

float Vector3::SqrDistance (const Vector3& a, const Vector3& b)
{
	return (a.x - b.x) * (a.x - b.x) +
		(a.y - b.y) * (a.y - b.y) +
		(a.z - b.z) * (a.z - b.z);
}

float Vector3::Distance (const Vector3& a, const Vector3& b)
{
	return sqrt (SqrDistance (a, b));
}

Vector3 Vector3::Cross(const Vector3& other) {
	Vector3 vector;

	vector.x = this->y * other.z - this->z * other.y;
	vector.y = this->z * other.x - this->x * other.z;
	vector.z = this->x * other.y - this->y * other.x;

	return vector;
}

void Vector3::Normalize(void) {
	float length = sqrt (x * x + y * y + z * z);

	x /= length;
	y /= length;
	z /= length;
}

Vector3 Vector3::Normalized (void) {
	Vector3 result;

	float length = sqrt (x * x + y * y + z * z);

	result.x = x / length;
	result.y = y / length;
	result.z = z / length;

	return result;
}

float Vector3::Magnitude (void) const {
	return sqrt (SqrMagnitude ());
}

float Vector3::SqrMagnitude (void) const {
	return x * x + y * y + z * z;
}

Vector3 & Vector3::operator=(const Vector3& other) {

	this->x = other.x;
	this->y = other.y;
	this->z = other.z;

	return *this;
}


Vector3 & Vector3::operator-=(const Vector3 &other) {

	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;

	return *this;
}

Vector3 Vector3::operator-(const Vector3 &other) const {
    Vector3 result;
    result.x = x - other.x;
    result.y = y - other.y;
    result.z = z - other.z;
    return result;              // All done!
}

Vector3 & Vector3::operator+=(const Vector3 &other) {

	this->x += other.x;
	this->y += other.y;
	this->z += other.z;

	return *this;
}

Vector3 Vector3::operator+(const Vector3 &other) const {
	Vector3 result = *this;
	result += other;
	return result;
}

Vector3 & Vector3::operator*=(const Vector3 &other) {

	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

Vector3 Vector3::operator*(const Vector3 &other) const {
    Vector3 result = *this;     // Make a copy of myself.  Same as MyClass result(*this);
    result *= other;            // Use += to add other to the copy.
    return result;              // All done!
}

Vector3 & Vector3::operator*=(float other) {

	x *= other;
	y *= other;
	z *= other;

	return *this;
}

Vector3 Vector3::operator*(float other) const {
	Vector3 result = *this;
	result *= other;
	return result;
}

bool Vector3::operator==(const Vector3& other) {
	return x == other.x && y == other.y && z == other.z;
}

bool Vector3::operator!=(const Vector3& other) {
	return !(*this == other);
}

std::ostream &operator<<(std::ostream &out, Vector3 v) {
	out << v.x << " " << v.y << " " << v.z;	
    return out;
}

std::istream &operator>>(std::istream &in, Vector3 &v) {
	in >> v.x >> v.y >> v.z;
    return in;
}

std::string Vector3::ToString () const
{
	std::string result = std::to_string (x) + ",";
	result += std::to_string (y) + ",";
	result += std::to_string (z);
	result = "(" + result + ")";

	return result;
}