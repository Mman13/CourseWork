#ifndef _ABFW_PRIMITIVE_H
#define _ABFW_PRIMITIVE_H

#include <abertay_framework.h>

namespace abfw
{
	class Platform;

	enum PrimitiveType
	{
		UNDEFINED = -1,
		TRIANGLE_LIST = 0,
		TRIANGLE_STRIP,
		LINE_LIST,
	};

	class Primitive
	{
	public:


		Primitive();
		virtual  ~Primitive();
		virtual bool InitIndexBuffer(const Platform& platform, const void* indices, const UInt32 num_indices, const UInt32 index_byte_size) = 0;
		virtual void SetType(PrimitiveType type) = 0;

		inline UInt32 num_indices() const {return num_indices_;}
		inline const class Texture* texture() const {return texture_;}
		inline void set_texture(const class Texture* texture) { texture_ = texture; }
		inline PrimitiveType type() const { return type_; }
	protected:
		inline void set_type(PrimitiveType type) { type_ = type; }

		UInt32 num_indices_;
		const class Texture* texture_;
		PrimitiveType type_;
	};

}
#endif // _ABFW_PRIMITIVE_H