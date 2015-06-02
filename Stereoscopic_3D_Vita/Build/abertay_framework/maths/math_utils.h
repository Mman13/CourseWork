#ifndef _ABFW_MATH_UTILS_H
#define _ABFW_MATH_UTILS_H

#define FRAMEWORK_PI 3.14159265358979323846264338327950288419716939937510
#define FRAMEWORK_DEG_TO_RAD ((float)FRAMEWORK_PI/180.0f)
#define FRAMEWORK_RAD_TO_DEG ((float)180.f/(float)FRAMEWORK_PI)

namespace abfw
{
	inline float DegToRad(float angleInDegrees)
	{
		return angleInDegrees*FRAMEWORK_DEG_TO_RAD;
	}

	inline float RadToDeg(float angleInRadians)
	{
		return angleInRadians*FRAMEWORK_RAD_TO_DEG;
	}

	inline float Lerp(float start, float end, float time)
	{
		return start*(1.0f - time) + time*end;
	}
}

#endif // _ABFW_MATH_UTILS_H