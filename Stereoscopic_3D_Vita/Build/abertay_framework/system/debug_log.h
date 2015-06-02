#ifndef _ABFW_DEBUG_LOG_H
#define _ABFW_DEBUG_LOG_H

namespace abfw
{
	class Matrix44;

	void DebugOut(const char * text, ...);
	void DebugOut(const Matrix44& matrix);
}

#endif // _ABFW_DEBUG_LOG_H