#include <maths/vector4.h>
#include <maths/matrix44.h>

namespace abfw
{

const Vector4 Vector4::Transform(const class Matrix44& _mat) const
{
	Vector4 result;

	result.x = x*_mat.m[0][0]+y*_mat.m[1][0]+z*_mat.m[2][0]+w*_mat.m[3][0];
	result.y = x*_mat.m[0][1]+y*_mat.m[1][1]+z*_mat.m[2][1]+w*_mat.m[3][1];
	result.z = x*_mat.m[0][2]+y*_mat.m[1][2]+z*_mat.m[2][2]+w*_mat.m[3][2];
	result.w = x*_mat.m[0][3]+y*_mat.m[1][3]+z*_mat.m[2][3]+w*_mat.m[3][3];

	return result;
}

	const Vector4 Vector4::CrossProduct(const Vector4& v1, const Vector4& v2) const
	{
		Vector4 result;
		result.x = y * (v1.z * v2.w - v2.z * v1.w) - z * (v1.y * v2.w - v2.y * v1.w) + w * (v1.y * v2.z - v1.z *v2.y);
		result.y = -(x * (v1.z * v2.w - v2.z * v1.w) - z * (v1.x * v2.w - v2.x * v1.w) + w * (v1.x * v2.z - v2.x * v1.z));
		result.z = x * (v1.y * v2.w - v2.y * v1.w) - y * (v1.x *v2.w - v2.x * v1.w) + w * (v1.x * v2.y - v2.x * v1.y);
		result.w = -(x * (v1.y * v2.z - v2.y * v1.z) - y * (v1.x * v2.z - v2.x *v1.z) + z * (v1.x * v2.y - v2.x * v1.y));
		return result;
	}

}