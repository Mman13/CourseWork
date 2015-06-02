//Camera.h 
//Created 28/3/15
//Author Michael Little - 1103677

#ifndef CAMERA_H
#define CAMERA_H

//includes
#include <abertay_framework.h>
#include <maths/vector3.h>
#include <graphics/mesh_instance.h>
#include <maths/quaternion.h>
#include <maths/matrix44.h>
#include <maths/aabb.h>
#include <maths/sphere.h>

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

/////////////////////////////////////////
// getters //
	float GetPitch();
	float GetYaw();
	float GetRoll();
	float GetNearPlane();
	float GetFarPlane();
	float GetFOV();
	float GetEyeSeperationDistance();
	float GetConvergenceDistance();
	float GetDleft();
	float GetDright();
	float GetHeight();

	abfw::Vector3 GetCameraEye();
	abfw::Vector3 GetLeftEye();
	abfw::Vector3 GetRightEye();
	abfw::Vector3 GetCameraLookat();
	abfw::Vector3 GetCameraLookatLeft();
	abfw::Vector3 GetCameraLookatRight();
	abfw::Vector3 GetCameraForward();
	abfw::Vector3 GetCameraUp();
	abfw::Vector3 GetCameraLeft();

	abfw::Quaternion GetRotation();

	abfw::Matrix44 GetViewMatrix();
	abfw::Matrix44 GetLeftViewMatrix();
	abfw::Matrix44 GetRightViewMatrix();
	const abfw::Matrix44 GetTransform();

//////////////////////////////////////////////
// setters //
	void SetPitch(const float NewPitch);
	void SetYaw(const float NewYaw);
	void SetRoll(const float NewRoll);
	void SetNearPlane(const float near);
	void SetFarPlane(const float far);
	void SetFOV(const float fov);
	void SetEyeSeperationDistance(const float esd);
	void SetConvergenceDistance(const float conDistance);
	void SetRadius(const float radius);
	void SetSetup(const bool setup);
	void SetCameraEye(const abfw::Vector3 eye);
	void SetCameraLookat(const abfw::Vector3 lookat);
	void SetCameraLookatLeft(const abfw::Vector3 lookatLeft);
	void SetCameraLookatRight(const abfw::Vector3 lookatRight);
	void SetCameraUp(const abfw::Vector3 up);
	void SetCameraLeft(const abfw::Vector3 left);

	void SetRotation(const abfw::Quaternion rotation);

	void SetViewMatrix(const abfw::Matrix44 view);
	void SetLeftViewMatrix(const abfw::Matrix44 leftView);
	void SetRightViewMatrix(const abfw::Matrix44 rightView);
	void SetTransform(const abfw::Matrix44 transform);

/////////////////////////////////////////////////////////
//bounding sphere //
	inline void set_bounding_sphere(const abfw::Sphere& sphere) { bounding_sphere_ = sphere; }
	inline const abfw::Sphere& bounding_sphere() const { return bounding_sphere_; }

/////////////////////////////////////////////////
// update //
	void UpdatePosition(const abfw::Vector3 update);
	void UpdateConDis(const float ConDis);
	void AddPitch(float PitchUpdate);
	void AddYaw(float YawUpdate);
	void AddRoll(float RollUpdate);

	void Update();
//////////////////////////////////////////
// movement //
	void MoveFWRD();
	void MoveBKWRD();
	void MoveLEFT();
	void MoveRIGHT();
	void MoveUP();
	void MoveDOWN();
	void RotateLEFT();
	void RotateRIGHT();
	void RotateDOWN();
	void RotateUP();

/////////////////////////////////////


	void Render();

private:

////////////////////////////////////////////////
// variables //
	float m_roll;
	float m_pitch;
	float m_yaw;
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;
	float near_plane_;
	float far_plane_;
	float camera_fov_;
	float eye_seperation_distance_;
	float convergence_distance_;
	bool setup_; //bool used to stop camera variables ovewriting themselves

	abfw::Vector3 camera_eye_;
	abfw::Vector3 left_eye_;
	abfw::Vector3 right_eye_;
	abfw::Vector3 camera_lookat_;
	abfw::Vector3 camera_lookat_left_;
	abfw::Vector3 camera_lookat_right_;
	abfw::Vector3 camera_up_;
	abfw::Vector3 camera_forward_;
	abfw::Vector3 camera_backwards_;
	abfw::Vector3 camera_left_;

	abfw::Quaternion rotation_;

	abfw::Matrix44 view_matrix;
	abfw::Matrix44 left_view_matrix;
	abfw::Matrix44 right_view_matrix;
	abfw::Matrix44 transform_;
	abfw::Matrix44 position_;

	abfw::Sphere bounding_sphere_; // declares sphere used for collision
};
#endif