#include <system/debug_log.h>
#include <cstdarg>
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <maths/matrix44.h>

namespace abfw
{
	void DebugOut(const char * text, ...)
	{
		va_list args;
		char text_buffer[256];

		va_start(args, text);
		std::vsprintf(text_buffer, text, args);


		OutputDebugString(text_buffer);
	}


	void DebugOut(const Matrix44& matrix)
	{
		for(int i=0;i<4;++i)
			for(int j=0;j<4;++j)
				DebugOut("%f ", matrix.m[i][j]);
		DebugOut("\n");
	}
}