#ifndef _SCENE_H
#define _SCENE_H

#include <list>
#include <system/string_id.h>
#include <graphics/mesh_data.h>
#include <ostream>
#include <istream>
#include <map>

namespace abfw
{
	class Mesh;
	class Texture;
	class Animation;
	class Platform;


	class Scene
	{
	public:
		~Scene();

		Mesh* CreateMesh(const Platform& platform, const MeshData& mesh_data);
		void CreateMaterials(const Platform& platform);

		bool WriteSceneToFile(const Platform& platform, const char* filename) const;
		bool ReadSceneFromFile(const Platform& platform, const char* filename);

		bool ReadScene(std::istream& Stream);
		bool WriteScene(std::ostream& Stream) const;
//		void WriteStringTable(std::istream& Stream) const;
//		void ReadStringTable(std::istream& Stream);

		class Skeleton* FindSkeleton(const MeshData& mesh_data);
		void FixUpSkinWeights();

		Animation* FindAnimation(const abfw::StringId anim_name_id);

		std::list<MeshData> meshes;
		std::list<MaterialData> materials;
		std::list<Texture*> textures;
		std::list<Skeleton*> skeletons;
		std::map<abfw::StringId, Animation*> animations;
		StringIdTable string_id_table;

		std::map<abfw::StringId, MaterialData*> materials_map;
		std::map<abfw::StringId, Texture*> textures_map;

		std::vector<abfw::StringId> skin_cluster_name_ids;
	};
}

#endif // _SCENE_H