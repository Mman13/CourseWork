#include <maths/vector2.h>
#include <cmath>

namespace abfw
{

void Vector2::Normalise()
{
	float length = Length();

	x /= length;
	y /= length;
}

float Vector2::LengthSqr() const
{
	return (x*x + y*y);
}

float Vector2::Length() const
{
	return std::sqrtf(x*x + y*y);
}

}