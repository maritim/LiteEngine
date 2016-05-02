/*************************************************************************** 
 Vector3
 ***************************************************************************/

#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <string>

#include "Core/Interfaces/Object.h"

class Vector3 : public Object
{
public:
	static Vector3 Up;
	static Vector3 Down;
	static Vector3 Left;
	static Vector3 Right;
	static Vector3 Back;
	static Vector3 Forward;
	static Vector3 One;
	static Vector3 Zero;
	float x, y, z;

public:
	Vector3 (void);
	Vector3 (const Vector3& other);
	Vector3 (float x, float y, float z = 0);
	
	// Much faster that extraction and magnitude
	static float SqrDistance (const Vector3& a, const Vector3& b);
	static float Distance (const Vector3& a, const Vector3& b);

	void Normalize(void);
	
	Vector3 Cross(const Vector3& other);
	Vector3 Normalized ();
	float Magnitude (void) const;
	float SqrMagnitude (void) const;
	
	Vector3 & operator=(const Vector3& other);
	Vector3 & operator-=(const Vector3& other);
	Vector3 & operator+=(const Vector3& other);
	Vector3 & operator*=(const Vector3& other);
	Vector3 & operator*=(float other);
	
	Vector3 operator-(const Vector3& other) const;
	Vector3 operator+(const Vector3& other) const;
	Vector3 operator*(const Vector3& other) const;
	Vector3 operator*(float other) const;
	
	bool operator==(const Vector3& other);
	bool operator!=(const Vector3& other);	

	friend std::ostream &operator<<(std::ostream &out, Vector3 v);
	friend std::istream &operator>>(std::istream &in, Vector3 &v);

	// From Object
	std::string ToString () const;
};

#endif