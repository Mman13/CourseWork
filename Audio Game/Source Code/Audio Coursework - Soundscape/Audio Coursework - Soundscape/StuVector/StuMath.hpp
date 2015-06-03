// Stuart Milne Aug 2011
// Basic low level float math functionality
// Float equality, Abs, square root, sin/cos/tan etc
// amended February 2014 by Allan C. Milne
// - added includes for <cstdlib> and <float>

#pragma once
#ifndef STU_MATH_INCLUDED
#define STU_MATH_INCLUDED

#include <cmath>
#include <cstdlib>
#include <cfloat>



namespace stu
{	namespace math
{
	// Math constants
	const float FloatZeroBarrier	= 1.0e-6f;
	const float PI					= 3.1415926535897932384626433832795f;
	const float HalfPI				= 1.5707963267948966192313216916398f;
	const float TwoPI				= 2.0f*PI;
	const float TORADS				= 0.01748329f;
	const float TODEGREES			= 57.1974725580826f;
	const float Infinity			= FLT_MAX;

	inline float Abs( float f) { return fabs(f); }

	inline bool	isZero(float f)
	{
		return ( Abs(f) < FloatZeroBarrier );
	}

	inline bool areEqual(float a, float b)
	{
		return (isZero( a - b ));
	}

	inline float Sqrt(float val)
	{
		return sqrtf(val);
	}

	inline float InvSqrt(float val)
	{
		return 1.0f / Sqrt(val);
	}

	inline float Sin(float f)
	{
		return sinf(f);
	}

	inline float Cos(float f)
	{
		return cosf(f);
	}

	inline float Tan(float f)
	{
		return tanf(f);
	}

	inline void SinCos( float a, float &sina, float &cosa)
	{
		sina = sinf(a);
		cosa = cosf(a);
	}

	inline float DegreesToRadians(const float degrees)
	{
		return degrees * TORADS;
	}

	inline float RadiansToDegrees(const float radians)
	{
		return radians * TODEGREES;
	}

	inline float RandF()
	{
		return static_cast<float>( static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	}

	inline float RandF(const float min, const float max)
	{
		return min + RandF()*(max-min);
	}

	// Taken from GPU Pro virtual texturing 101 example code
	inline int log2_floored(const int x)
	{
		int bit = 31;
		for (; bit >= 0; --bit)	{
			if (x & (1 << bit)) {
				break;
			}
		}
		return bit;
	}

	// TEMPLATED	///////////////////////////
	template<typename T>
	inline T Min(const T& first, const T& second)
	{
		return first < second ? first : second;
	}

	template<typename T>
	inline T Max(const T& first, const T& second)
	{
		return first > second ? first : second;
	}

	template<typename T>
	inline T Lerp(const T& start, const T& end, const float t)
	{
		return start + ((end - start) * t);
	}

	template<typename T>
	inline T Clamp(const T& value, const T& min, const T& max)
	{
		return value < min ? min : (value > max ? max : value);
	}

	template<typename T>
	inline T Wrap(const T& value, const T& min, const T& max)
	{
		return value < min ? max : (value > max ? min : value);
	}
}
}
#endif // STU_MATH_INCLUDED