#ifndef _ABFW_VECTOR2_H
#define _ABFW_VECTOR2_H

namespace abfw
{
class Vector2
{
public:

	Vector2();
	~Vector2();
	Vector2(const float x, const float y);

	const Vector2 operator - (const Vector2& vec) const;
	const Vector2 operator + (const Vector2& vec) const;
	Vector2& operator -= (const Vector2& vec);
	Vector2& operator += (const Vector2& _vec);
	const Vector2 operator * (const float scalar) const;
	const Vector2 operator / (const float scalar) const;
	Vector2& operator *= (const float scalar);
	Vector2& operator /= (const float scalar);

	void Normalise();
	float LengthSqr() const;
	float Length() const;

	float x;
	float y;
};

}

#include "vector2.inl"

#endif // _ABFW_VECTOR2_H