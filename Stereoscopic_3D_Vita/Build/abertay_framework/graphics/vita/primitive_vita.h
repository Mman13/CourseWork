#ifndef _PRIMITIVE_VITA_H
#define _PRIMITIVE_VITA_H

#include <graphics/primitive.h>
#include <gxm.h>

namespace abfw
{
	class PrimitiveVita : public Primitive
	{
	public:
		PrimitiveVita();
		~PrimitiveVita();

		bool InitIndexBuffer(const Platform& platform, const void* indices, const UInt32 num_indices, const UInt32 index_byte_size);
		void SetIndexBuffer(SceGxmContext* context) const;

		void SetType(PrimitiveType type);
		inline SceGxmPrimitiveType topology() const { return topology_; }
		inline SceGxmIndexFormat index_format() const { return index_format_; }
		inline void* indices() const { return indices_; }


	private:
		void* indices_;
		SceUID indices_uid_;

//		ID3D11Buffer*      index_buffer_;
		SceGxmPrimitiveType topology_;
		SceGxmIndexFormat index_format_;
	};
}


#endif // _PRIMITIVE_VITA_H