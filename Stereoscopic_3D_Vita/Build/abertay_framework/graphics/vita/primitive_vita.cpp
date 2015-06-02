#include <graphics/vita/primitive_vita.h>
#include <system/vita/platform_vita.h>

void *graphicsAlloc(SceKernelMemBlockType type, uint32_t size, uint32_t alignment, uint32_t attribs, SceUID *uid);
void graphicsFree(SceUID uid);

namespace abfw
{
	PrimitiveVita::PrimitiveVita() :
		indices_(NULL),
		indices_uid_(0)
	{
	}

	PrimitiveVita::~PrimitiveVita()
	{
		graphicsFree(indices_uid_);
	}

	bool PrimitiveVita::InitIndexBuffer(const Platform& platform, const void* indices, const UInt32 num_indices, const UInt32 index_byte_size)
	{
		num_indices_ = num_indices;
//		PlatformVita& platform_d3d = static_cast<PlatformVita&>(platform);
		bool success = true;

			indices_ = (uint32_t *)graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		num_indices*sizeof(uint32_t),
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&indices_uid_);

		memcpy(indices_, indices, num_indices*sizeof(uint32_t));
		index_format_ = SCE_GXM_INDEX_FORMAT_U32;

		return success;
	}

	void PrimitiveVita::SetIndexBuffer(SceGxmContext* context) const
	{
		if(indices_)
		{
			// Set index buffer
		}
	}

	void PrimitiveVita::SetType(PrimitiveType type)
	{
		type_ = type;

		switch(type_)
		{
		case TRIANGLE_LIST:
			topology_ = SCE_GXM_PRIMITIVE_TRIANGLES;
			break;

		case LINE_LIST:
			topology_ = SCE_GXM_PRIMITIVE_LINES;

		case TRIANGLE_STRIP:
			topology_ = SCE_GXM_PRIMITIVE_TRIANGLE_STRIP;
			break;
		}
	}
}