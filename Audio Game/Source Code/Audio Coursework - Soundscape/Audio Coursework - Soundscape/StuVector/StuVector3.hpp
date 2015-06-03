#pragma once
#ifndef STU_VECTOR3_INCLUDED
#define STU_VECTOR3_INCLUDED

// Stuart Milne Aug 2011
// Fast vector class
#ifdef _MSC_VER
#pragma once
#define USE_MSCV_INTRINSICS
#endif

#include <string>	// for memcpy
#include <ostream>	// for << overloading

namespace stu
{
	__declspec(align(16)) class v3f
	{
	public:
// 		inline v3f(){};
// 		inline v3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z){}
// 		inline v3f(float val) : x(val), y(val), z(val) {}
// 		inline ~v3f(){}
// // 
// 		//copy
// 		v3f(const v3f &other_vec);
// 		v3f &operator = (const v3f &other_vector);

		float x, y, z;
		// accessors
		inline float &operator[](unsigned int i) { return (&x)[i]; }
		inline const float &operator[](unsigned int i) const { return (&x)[i]; }
		// Get as pointer to float array
		inline float *Ptr() { return &x; }

		// setters
		inline void SetAll(float _x, float _y, float _z);
		inline void IncAll(float x_inc, float y_inc, float z_inc);	// Increment values
		inline void Set(float *floatarray);

		// compare
		bool operator == (const v3f & other_vec) const;
		bool operator != (const v3f & other_vec) const;
		bool isZero() const;	// All components 0
		bool isUnit() const;	// Unit/normalised vector

		// operations
		inline void Zero();		// Sets all elements to 0
		void Clean();			// Sets all near-zero elements to 0
		void Normalise();		// Vector becomes a unit vector

		// utility
		float Magnitude() const;// Length/size/magnitude of vector
		float MagnitudeSquared() const;

		//Make compatible with standard output
		//friend Writer &operator <<(Writer &out, const v3f &output_vec)

		// Vector math operators
		// addition/subtraction
		v3f operator + (const v3f &v) const;
		v3f &operator += (v3f &other);
		v3f operator - (const v3f &v) const;
		v3f &operator -= (v3f &other);
		v3f operator-() const;

		// scalar multiplication
		v3f operator * (float scalar);
		v3f operator *= (float scalar);
		v3f operator / (float scalar);
		v3f operator /= (float scalar);

		friend std::ostream& operator <<(std::ostream& os, v3f& vector);

		// Math operations
		float			Dot( const v3f &v );		
		v3f				Cross( const v3f &v );
		static float	Dot( const v3f&v1, const v3f &v2) ;
		static v3f		Cross( const v3f&v1, const v3f &v2) ;
		static float	Distance(const v3f &point1, const v3f &point2);
		static float	DistanceSquared(v3f const &point1, const v3f &point2);
		static v3f		RandomVector();
		static v3f		RandomUnitVector();
	};

	// Handy 'presets'
	const v3f ZeroVector3 = {0.0f, 0.0f, 0.0f};
	const v3f xAxisVector3 = {1.0f, 0.0f, 0.0f};
	const v3f yAxisVector3 = {0.0f, 1.0f, 0.0f};
	const v3f zAxisVector3 = {0.0f, 0.0f, 1.0};


	// Inlined functions
	inline void stu::v3f::SetAll(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}

	inline void stu::v3f::IncAll(float x_inc, float y_inc, float z_inc)
	{
		x += x_inc;
		y += y_inc;
		z += z_inc;
	}

	inline void v3f::Set(float *floatarray)
	{
		memcpy(&x, floatarray, sizeof(v3f)); //-V512
	}

	inline void v3f::Zero()
	{
		x = y = z = 0.0f;
	}
}//namespace
#endif // STU_VECTOR3_INCLUDED