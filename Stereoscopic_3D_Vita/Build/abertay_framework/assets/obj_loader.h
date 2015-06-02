#ifndef _ABFW_OBJ_LOADER_H
#define _ABFW_OBJ_LOADER_H

#include <abertay_framework.h>
#include <map>
#include <string>
#include <vector>

namespace abfw
{
	class Platform;
	class Model;
	class Texture;

	class OBJLoader
	{
	public:
		bool Load(const char* filename, const Platform& platform, Model& model);
	private:
		bool LoadMaterials(const Platform& platform, const char* filename, std::map<std::string, Int32>& materials, std::vector<Texture*>& textures);
	};
}


#endif // _OBJ_LOADER_H