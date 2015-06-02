//GameObject.h 
//Created 6/3/15
//Author Michael Little - 1103677

#include "GameObject.h"


GameObject::GameObject()
{
	//initialise variables
	bounding_sphere_.set_radius(3.0f); // sets default bounding sphere radius
	bounding_sphere_.set_position(abfw::Vector3(0.0f, 0.0f, 0.0f)) ;
	position_ = abfw::Vector3(-40 + rand()%80, -40 + rand()%80, -1200 + rand()%600); // sets the initial position for objects
	velocity_ = abfw::Vector3(0.0f, 0.0f, 0.0f);//initialises velocity
	scale_ = abfw::Vector3(10.0f, 0.0f, 0.0f);//initialises scale
	rotation_ = abfw::Quaternion(0.0f, 0.0f, 0.0f, 0.0f);//initialises rotation
	transform_.Scale(scale_);
	transform_.SetTranslation(position_);
	set_transform(transform_);

	
	objectCount_ = 0;
	objectSpeed1_ = 0;

	obstacle_ = false;
}

GameObject::~GameObject()
{
}

abfw::Vector3 GameObject::GetPosition()//used to return an objects position
{
	return position_;
}

abfw::Vector3 GameObject::GetVelocity()//used to return an objects velocity
{
	return velocity_;
}

abfw::Quaternion GameObject::GetRotation()//used to return an objects rotation
{
	return rotation_;
}

abfw::Matrix44 GameObject::GetTransform()//used to return an objects transform
{
	return transform_;
}

int GameObject::GetObjectCount()//used to get the object count
{
	return objectCount_;
}

float GameObject::GetObjectSpeed()//used to get the objects velocity/speed
{
	return objectSpeed1_;
}

bool GameObject::GetObstacle()//used to return whether an object is an obstacle
{
	return obstacle_;
}


void GameObject::SetPosition(const abfw::Vector3 position)//used to set an objects position
{
	position_ = position;
	transform_.SetTranslation(position_);
	set_transform(transform_);
}

void GameObject::SetScale(const abfw::Vector3 scale)//used to set an objects Scale
{
	scale_ = scale;
	transform_.Scale(scale_);
	transform_.SetTranslation(position_);
	set_transform(transform_);
}

void GameObject::SetRotation(const abfw::Quaternion rotation)//used to set an objects rotation
{
	rotation_ = rotation;
	transform_.SetIdentity();
	transform_.Rotation(rotation_);
	set_transform(transform_);
}

void GameObject::SetTransform(const abfw::Matrix44 transform)//used to set an objects transform
{
	transform_ = transform;
	transform_.SetIdentity();
	set_transform(transform_);
}

void GameObject::SetObjectCount(const int count)//sets the minimum object count
{
	objectCount_ = count;
}

void GameObject::SetObjectSpeed(const float speed)// used to set the objects velocity/speed
{
	objectSpeed1_ = speed;
}

void GameObject::SetRadius(const float radius)//used to set an objects bounding sphere radius
{
	bounding_sphere_.set_radius(radius);
}
void GameObject::SetObstacle(const bool obstacle) //used to set if an object is an obstacle
{
	obstacle_ = obstacle;
}


void GameObject::UpdatePosition(const abfw::Vector3 update)//used to update an objects position
{
	position_ += update;
	transform_.SetTranslation(position_);
	set_transform(transform_);
}

void GameObject::UpdateRotation(const abfw::Quaternion rotation)//used to update an objects rotation
{
	rotation_.x += rotation.x;
	rotation_.y += rotation.y;
	rotation_.z += rotation.z;
	transform_.SetIdentity();
	transform_.Rotation(rotation_);
	set_transform(transform_);
}

void GameObject::Update() //updates the objects when called
{

	if(position_.z <= 50 ) //moves the objects towards payer and resets them whn the player passes them
	{
		velocity_.x = 0;
		velocity_.y = 0;
		velocity_.z = objectSpeed1_;
		UpdatePosition(velocity_);
	}
	else
	{
		SetPosition(abfw::Vector3(-40 + rand()%80, -40 + rand()%80, -1200 + rand()%600));
	}
	
}

