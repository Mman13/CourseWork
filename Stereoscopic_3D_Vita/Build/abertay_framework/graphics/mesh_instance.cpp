#include <graphics/mesh_instance.h>
#include <cstddef>

namespace abfw
{
	MeshInstance::MeshInstance() :
		mesh_(NULL)
	{
		transform_.SetIdentity();
	}
}
