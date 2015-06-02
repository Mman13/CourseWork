#ifndef _ABFW_MEMORY_STREAM_BUFFER_H
#define _ABFW_MEMORY_STREAM_BUFFER_H

#include <streambuf>

namespace abfw
{
	class MemoryStreamBuffer : public std::streambuf
	{
	public:
		MemoryStreamBuffer(char* buffer, size_t size);
	};
}

#endif // _ABFW_MEMORY_STREAM_BUFFER_H