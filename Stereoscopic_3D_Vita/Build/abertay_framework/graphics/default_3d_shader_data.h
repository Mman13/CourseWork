#ifndef _ABFW_DEFAULT_3D_SHADER_DATA_H
#define _ABFW_DEFAULT_3D_SHADER_DATA_H

#include <graphics/colour.h>
#include <vector>
#include <graphics/point_light.h>

namespace abfw
{
	class Default3DShaderData
	{
	public:
		Default3DShaderData();

		Int32 AddPointLight(const PointLight& point_light);
		UInt32 GetNumPointLights() const { return point_lights_.size(); }

		inline const PointLight& GetPointLight(const Int32 light_num) const { return point_lights_[light_num]; }
		inline const Colour& ambient_light_colour() const { return ambient_light_colour_; }
		inline void set_ambient_light_colour(const Colour& colour) { ambient_light_colour_ = colour; }

		void CleanUp();
	private:

		Colour ambient_light_colour_;
		std::vector<PointLight> point_lights_;
	};
}
#endif // _ABFW_DEFAULT_3D_SHADER_H

