//GameObject.h 
//Created 6/3/15
//Author Michael Little - 1103677

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

//includes


#include <graphics/mesh_instance.h>
#include <system/application.h>
#include <maths/vector3.h>
#include <maths/quaternion.h>
#include <maths/matrix44.h>
#include <maths/aabb.h>
#include <maths/sphere.h>

class GameObject : public abfw::MeshInstance
{
public:
	GameObject();
	//GameObject(System* systempointer);
	~GameObject();
////////////////////////////////////////////////////////////
// getters //
	abfw::Vector3 GetPosition();
	abfw::Vector3 GetVelocity();
	abfw::Vector3 WorldPosition();
	abfw::Quaternion GetRotation();
	abfw::Matrix44 GetTransform();
	int GetObjectCount();
	float GetObjectSpeed();
	bool GetObstacle();

////////////////////////////////////////////////////////////
// setters //
	void SetPosition(const abfw::Vector3 position);
	void SetScale(const abfw::Vector3 scale);
	void SetRotation(const abfw::Quaternion rotation);
	void SetTransform(const abfw::Matrix44 transform);
	void SetObjectCount(const int count);
	void SetObjectSpeed(const float speed);
	void SetRadius(const float radius);
	void SetObstacle(const bool obstacle);

////////////////////////////////////////////////////////////
// updates //
	void UpdatePosition(const abfw::Vector3 update);
	void UpdateRotation(const abfw::Quaternion rotation);
//System* systempointer
	void Update();
	//void ParticleTranslation(const abfw::Vector3 position);
/////////////////////////////////////////////////////////////
	
	//sets up the bounding sphere user for collision
	inline void set_bounding_sphere(const abfw::Sphere& sphere) { bounding_sphere_ = sphere; }
	inline const abfw::Sphere& bounding_sphere() const { return bounding_sphere_; }

private:

	//variables
	abfw::Vector3 position_;
	abfw::Vector3 velocity_;
	abfw::Vector3 scale_;

	abfw::Quaternion rotation_;

	abfw::Matrix44 transform_;

	abfw::Platform* platform;

	abfw::Mesh* mesh_;
	abfw::Sphere bounding_sphere_; // initialises sphere used for bounding sphere

	int objectCount_;

	float objectSpeed1_;
	float objectSpeed2_;

	bool obstacle_; //used to differentiate targets from obstacles

};

#endif