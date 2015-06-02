#ifndef _ABFW_VECTOR4_H
#define _ABFW_VECTOR4_H

namespace abfw
{

class Vector4
{
public:
	Vector4();
	Vector4(const float x, const float y, const float z, const float w = 1.0f);

	const Vector4 Transform(const class Matrix44& _mat) const;
	const Vector4 CrossProduct(const Vector4& v2, const Vector4& v3) const;

	float x;
	float y;
	float z;
	float w; 
};

}

#include "vector4.inl"

#endif // _ABFW_VECTOR4_H
