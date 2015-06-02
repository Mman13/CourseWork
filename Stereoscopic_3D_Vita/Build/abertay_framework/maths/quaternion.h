#ifndef _ABFW_QUATERNION_H
#define _ABFW_QUATERNION_H

#include <cmath>

namespace abfw
{

class Quaternion
{
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	void SetFromMatrix(const class Matrix44& matrix);
	const Quaternion operator * (const Quaternion& quaternion) const;
	const Quaternion operator -() const;
	const Quaternion operator *(float scale) const;
	const Quaternion operator /(float scale) const;
	const Quaternion operator +(const Quaternion &q) const;
	float LengthSquared() const;
	float Length() const;
	void Normalise();
	void Identity();
	void Lerp(const Quaternion& startQ, const Quaternion& endQ, float time);
	void Slerp(const Quaternion& startQ, const Quaternion& endQ, float time);
	void Conjugate(const Quaternion& quaternion);
	

	float x;
	float y;
	float z;
	float w;
};

}

#include "Quaternion.inl"


#endif // _ABFW_QUATERNION_H