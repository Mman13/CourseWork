#include <graphics/default_3d_shader_data.h>

namespace abfw
{
	Default3DShaderData::Default3DShaderData() :
		ambient_light_colour_(0.5f, 0.5f, 0.5f, 1.f)
	{
	}

	Int32 Default3DShaderData::AddPointLight(const PointLight& point_light)
	{
		point_lights_.push_back(point_light);
		return point_lights_.size()-1;
	}

	void Default3DShaderData::CleanUp()
	{
		point_lights_.clear();
	}

}
