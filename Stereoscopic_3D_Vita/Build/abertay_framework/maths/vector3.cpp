#include <maths/vector3.h>
#include <maths/matrix44.h>
#include <maths/math_utils.h>
#include <cmath>

namespace abfw
{
	const Vector3 Vector3::kZero(0.0f, 0.0f, 0.0f);

	float Vector3::LengthSqr() const
	{
		return x*x + y*y + z*z;
	}

	float Vector3::Length() const
	{
		return std::sqrtf(LengthSqr());
	}

	void Vector3::Normalise()
	{
		float length = Length();
		x = x / length;
		y = y / length;
		z = z / length;
	}

	float Vector3::DotProduct(const Vector3& _vec) const
	{
		return x*_vec.x + y*_vec.y + z*_vec.z;
	}

	const Vector3 Vector3::CrossProduct(const Vector3& _vec) const
	{
		Vector3 result;

		result.x = y*_vec.z - z*_vec.y;
		result.y = z*_vec.x - x*_vec.z;
		result.z = x*_vec.y - y*_vec.x;

		return result;
	}

	const Vector3 Vector3::Transform(const class Matrix44& _mat) const
	{
		Vector3 result = Vector3(0.0f, 0.0f, 0.0f);

		result.x = x*_mat.m[0][0]+y*_mat.m[1][0]+z*_mat.m[2][0]+_mat.m[3][0];
		result.y = x*_mat.m[0][1]+y*_mat.m[1][1]+z*_mat.m[2][1]+_mat.m[3][1];
		result.z = x*_mat.m[0][2]+y*_mat.m[1][2]+z*_mat.m[2][2]+_mat.m[3][2];

		return result;
	}

	const Vector3 Vector3::TransformNoTranslation(const class Matrix44& _mat) const
	{
		Vector3 result = Vector3(0.0f, 0.0f, 0.0f);

		result.x = x*_mat.m[0][0]+y*_mat.m[1][0]+z*_mat.m[2][0];
		result.y = x*_mat.m[0][1]+y*_mat.m[1][1]+z*_mat.m[2][1];
		result.z = x*_mat.m[0][2]+y*_mat.m[1][2]+z*_mat.m[2][2];

		return result;
	}

	void Vector3::Lerp(const Vector3& start, const Vector3& end, const float time)
	{
		x = abfw::Lerp(start.x, end.x, time);
		y = abfw::Lerp(start.y, end.y, time);
		z = abfw::Lerp(start.z, end.z, time);
	}
}