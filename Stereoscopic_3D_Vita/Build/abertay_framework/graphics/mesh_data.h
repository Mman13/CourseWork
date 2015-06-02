#ifndef _ABFW_MESH_DATA_H
#define _ABFW_MESH_DATA_H

#include <vector>
#include <maths/vector3.h>
#include <graphics/primitive.h>
#include <string>
#include <system/string_id.h>
#include <maths/aabb.h>

#include <ostream>
#include <istream>

namespace abfw
{
	struct MaterialData
	{
		std::string diffuse_texture;
		abfw::StringId name_id;

		bool Read(std::istream& stream);
		bool Write(std::ostream& stream) const;
	};

	struct PrimitiveData
	{
		PrimitiveData();
		~PrimitiveData();

		bool Read(std::istream& stream);
		bool Write(std::ostream& stream) const;

		void* indices;
		MaterialData* material;
		UInt32 num_indices;
		UInt32 index_byte_size;
		PrimitiveType type;
	};

	struct VertexData
	{
		VertexData();
		~VertexData();

		bool Read(std::istream& stream);
		bool Write(std::ostream& stream) const;

		void* vertices;
		UInt32 num_vertices;
		UInt32 vertex_byte_size;
	};


	struct MeshData
	{
		MeshData();
		~MeshData();

		bool Read(std::istream& stream);
		bool Write(std::ostream& stream) const;

		VertexData vertex_data;
		std::vector<PrimitiveData*> primitives;
		abfw::StringId name_id;

		Aabb aabb;
	};
}

#endif // _ABFW_MESH_DATA_H