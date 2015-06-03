#include "StuVector3.hpp"
#include "StuMath.hpp"


namespace stu
{
	//Writer &operator << (Writer &out, const v3f &sourcevec)

	// Copy
	//v3f::v3f(const v3f &v) : x(v.x), y(v.y), z(v.z) {};

	// Operators
// 	v3f &v3f::operator = (const v3f &other_vector)
// 	{
// 		// protect from self-assignment
// 		if (this == &other_vector) return *this;
// 
// 		// memcpy(this, &other_vector, sizeof(other_vector);
// 		x = other_vector.x;
// 		y = other_vector.y;
// 		z = other_vector.z;
// 
// 		return *this;
// 	}

	// compare
	bool v3f::operator == (const v3f & other_vec) const
	{
		if ( !math::areEqual(x, other_vec.x) ) return false;
		if ( !math::areEqual(y, other_vec.y) ) return false;
		if ( !math::areEqual(z, other_vec.z) ) return false;

		return true;
	}

	bool v3f::operator != (const v3f & other_vec) const
	{
		if ( math::areEqual(x, other_vec.x) 
			&& math::areEqual(y, other_vec.y) 
			&& math::areEqual(z, other_vec.z) )
		{
			return false;
		}

		return true;
	}
	bool v3f::isZero() const	// All components 0
	{
		return math::isZero(x*x + y*y + z*z);
	}
	bool v3f::isUnit() const	// Unit/normalised vector
	{
		return math::isZero( 1.0f - x*x - y*y - z*z);
	}

	// operations
	void v3f::Clean()			// Sets all near-zero elements to 0
	{
		if( math::isZero(x) )x = 0.0f;
		if ( math::isZero(y) ) y = 0.0f;
		if( math::isZero(z) ) z = 0.0f;
	}
	void v3f::Normalise()		// Vector becomes a unit vector
	{
		float magsquared = MagnitudeSquared();

		if( math::isZero( magsquared ) )
		{
			Zero();
		}
		else
		{
			float factor = math::InvSqrt( magsquared );
			x *= factor;
			y *= factor;
			z *= factor;
		}
	}
	// utility
	float v3f::Magnitude() const// Length/size/magnitude of vector
	{
		return math::Sqrt( x*x + y*y + z*z);
	}

	float v3f::MagnitudeSquared() const
	{
		return (x*x + y*y + z*z);
	}

	//console output
	//friend Writer &operator <<(Writer &out, const v3f &output_vec)

	// Vector math operators
	// addition/subtraction
	v3f v3f::operator + (const v3f &v) const
	{
		//return v3f( x + v.x, y + v.y, z + v.z);
		v3f result = {  x + v.x, y + v.y, z + v.z };
		return result;
	}

	v3f& v3f::operator += (v3f &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	v3f v3f::operator - (const v3f &v) const
	{
		//return v3f( x - v.x, y - v.y, z - v.z );
		v3f result = {  x - v.x, y - v.y, z - v.z };
		return result;
	}

	v3f& v3f::operator -= (v3f &other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	v3f v3f::operator-() const
	{
		//return v3f( -x, -y, -z);
		v3f result = {-x, -y, -z};
		return result;
	}

	// scalar multiplication
	v3f v3f::operator * (float scalar)
	{
		//return v3f( x * scalar, y * scalar, z * scalar );
		v3f result = {x * scalar, y * scalar, z * scalar };
		return result;
	}

	v3f v3f::operator *= (float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	v3f v3f::operator / (float scalar)
	{
		float invscalar = 1.0f / scalar;
		//return v3f( x*invscalar, y * invscalar, z * invscalar);
		v3f result = { x * invscalar, y * invscalar, z * invscalar };
		return result;
	}
	v3f v3f::operator /= (float scalar)
	{
		float invscalar = 1.0f / scalar;
		x *= invscalar;
		y *= invscalar;
		z *= invscalar;

		return *this;
	}

	// Math operations
	float v3f::Dot( const v3f &v )
	{
		return ( x*v.x + y*v.y + z*v.z );
	}

	float v3f::Dot( const v3f&v1, const v3f &v2)	// friend
	{
		return(v1.x*v2.x + v1.y*v2.y + v1.z+v2.z);
	}

	v3f v3f::Cross( const v3f &v )
	{
// 		return v3f( y*v.z - z*v.y,
// 					z*v.x - x*v.z,
// 					x*v.y - y*v.x );
		v3f result = {	y*v.z - z*v.y,
						z*v.x - x*v.z,
						x*v.y - y*v.x };
		return result;
	}
	v3f v3f::Cross( const v3f&v1, const v3f &v2)
	{
// 		return v3f( v1.y*v2.z - v1.z*v2.y, 
// 					v1.z*v2.x - v1.x*v2.z,
// 					v1.x*v2.y - v1.y*v2.x );
		v3f result = {	v1.y*v2.z - v1.z*v2.y, 
						v1.z*v2.x - v1.x*v2.z,
						v1.x*v2.y - v1.y*v2.x };
		return result;
	}
	float v3f::Distance(const v3f &point1, const v3f &point2)
	{
		v3f pathvec(point1 - point2);

		return pathvec.Magnitude();
	}
	float v3f::DistanceSquared(v3f const &point1, const v3f &point2)
	{
		v3f pathvec(point1 - point2);

		return pathvec.MagnitudeSquared();
	}

	v3f v3f::RandomVector()
	{
		//return v3f(math::RandF(), math::RandF(), math::RandF());
		v3f result = { math::RandF(), math::RandF(), math::RandF() };
		return result;
	}

	v3f v3f::RandomUnitVector()
	{
		v3f v = RandomVector();
		v.Normalise();
		return v;
	}

	std::ostream& operator<<( std::ostream& os, v3f& vector )
	{
		os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
		return os;
	}

}
