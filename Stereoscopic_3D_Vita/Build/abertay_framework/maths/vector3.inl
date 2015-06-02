namespace abfw
{

inline Vector3::Vector3()
{
}

inline Vector3::~Vector3()
{
}

inline Vector3::Vector3(const float new_x, const float new_y, const float new_z)
{
	x = new_x;
	y = new_y;
	z = new_z;
}

inline const Vector3 Vector3::operator-(const Vector3& _vec) const
{
	Vector3 result;

	result.x = x-_vec.x;
	result.y = y-_vec.y;
	result.z = z-_vec.z;

	return result;
}

inline const Vector3 Vector3::operator+(const Vector3& _vec) const
{
	Vector3 result;

	result.x = x+_vec.x;
	result.y = y+_vec.y;
	result.z = z+_vec.z;

	return result;
}

inline Vector3& Vector3::operator+=(const Vector3& _vec)
{
	x += _vec.x;
	y += _vec.y;
	z += _vec.z;

	return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& _vec)
{
	x -= _vec.x;
	y -= _vec.y;
	z -= _vec.z;

	return *this;
}

inline const Vector3 Vector3::operator*(const float _scalar) const
{
	Vector3 result;

	result.x = x*_scalar;
	result.y = y*_scalar;
	result.z = z*_scalar;

	return result;
}

inline const Vector3 Vector3::operator/(const float _scalar) const
{
	Vector3 result;

	result.x = x/_scalar;
	result.y = y/_scalar;
	result.z = z/_scalar;

	return result;
}

inline Vector3& Vector3::operator*=(const float _scalar)
{
	x *= _scalar;
	y *= _scalar;
	z *= _scalar;

	return *this;
}

inline Vector3& Vector3::operator/=(const float _scalar)
{
	x /= _scalar;
	y /= _scalar;
	z /= _scalar;

	return *this;
}

}