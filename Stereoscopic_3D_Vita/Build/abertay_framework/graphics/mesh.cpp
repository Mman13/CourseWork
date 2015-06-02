#include <graphics/mesh.h>
#include <graphics/primitive.h>

namespace abfw
{
	Mesh::Mesh() :
	num_primitives_(0),
	primitives_(NULL),
	num_vertices_(0),
	vertex_byte_size_(0)
	{
	}

	Mesh::~Mesh()
	{
		ReleasePrimitives();
	}

	bool Mesh::InitVertexBuffer(const Platform& platform, const void* vertices, const UInt32 num_vertices, const UInt32 vertex_byte_size, bool read_only)
	{
		num_vertices_ = num_vertices;
		vertex_byte_size_ = vertex_byte_size;

		// this makes the assumption that the position data is at the start of any vertex data passed into this function
		// this is okay for just now, but should probably be moved out of here so InitVertexBuffer isn't tied
		// to a particular vertex structure
		//CalculateBounds(static_cast<const Vector3*>(vertices), num_vertices, vertex_byte_size);

		return true;
	}

	void Mesh::AllocatePrimitives(const UInt32 num_primitives)
	{
		if(primitives_)
			ReleasePrimitives();
		num_primitives_ = num_primitives;
		if(num_primitives_)
		{
			primitives_ = new Primitive*[num_primitives_];
			for(UInt32 primitive_index=0;primitive_index<num_primitives_;++primitive_index)
			{
				primitives_[primitive_index] = AllocatePrimitive();
			}
		}
	}

	void Mesh::ReleasePrimitives()
	{
		if(num_primitives_ && primitives_)
		{
			for(UInt32 primitive_index=0;primitive_index<num_primitives_;++primitive_index)
			{
				delete primitives_[primitive_index];
				primitives_[primitive_index] = NULL;
			}
			delete[] primitives_;
			primitives_ = NULL;
		}
		num_primitives_ = 0;
	}

/*
	void Mesh::CalculateBounds(const Vector3* vertex_positions, const UInt32 num_vertices, const UInt32 vertex_byte_size)
	{
		// go through all the vertex positions and set up the axis aligned bounding box that encapsulates the mesh
		const Vector3* position = vertex_positions;
		for(Int32 vertex_num = 0; vertex_num < num_vertices; ++vertex_num)
		{
			aabb_.Update(*position);
			position = reinterpret_cast<const Vector3*>(reinterpret_cast<const UInt8*>(position)+vertex_byte_size);
		}

		// now the aabb is setup, it can be used to initialise the bounding sphere
		bounding_sphere_ = Sphere(aabb_);
	}
*/
}