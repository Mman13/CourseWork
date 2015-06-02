#ifndef _ABFW_VECTOR3_H
#define _ABFW_VECTOR3_H


namespace abfw
{
class Vector3
{
public:

	Vector3();
	~Vector3();
	Vector3(const float new_x, const float new_y, const float new_z);

	const Vector3 operator - (const Vector3& _vec) const;
	const Vector3 operator + (const Vector3& _vec) const;
	Vector3& operator -= (const Vector3& _vec);
	Vector3& operator += (const Vector3& _vec);
	const Vector3 operator * (const float _scalar) const;
	const Vector3 operator / (const float _scalar) const;
	Vector3& operator *= (const float _scalar);
	Vector3& operator /= (const float _scalar);

	void Normalise();
	float LengthSqr() const;
	float Length() const;
	float DotProduct(const Vector3& _vec) const;
	const Vector3 CrossProduct(const Vector3& _vec) const;
	const Vector3 Transform(const class Matrix44& _mat) const;
	const Vector3 TransformNoTranslation(const class Matrix44& _mat) const;
	void Lerp(const Vector3& start, const Vector3& end, const float time);

	float x;
	float y;
	float z;

	static const Vector3 kZero;
};

}

#include "maths/vector3.inl"

#endif // _ABFW_VECTOR3_H