//Camera.cpp 
//Created 28/3/15
//Author Michael Little - 1103677

#include "Camera.h"

Camera::Camera()
{
	//initialise variables
	setup_ = false;
	bounding_sphere_.set_position(abfw::Vector3(0.0f, 0.0f, 0.0f));
	bounding_sphere_.set_radius(2.0f);
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

///////////////////////////////////////////////////////////////
// getters //

float Camera::GetPitch() //Used to return the pitch of the camera
{
	return m_pitch;
}

float Camera::GetYaw()//Used to return the yaw the camera
{
	return m_yaw;
}

float Camera::GetRoll()//Used to return the roll of the camera
{
	return m_roll;
}

float Camera::GetNearPlane()//Used to return the near plane 
{
	return near_plane_;
}

float Camera::GetFarPlane()//Used to return the far plane
{
	return far_plane_;
}

float Camera::GetFOV()//Used to return the fov
{
	return camera_fov_;
}

float Camera::GetEyeSeperationDistance() //Used to return the eye seperation distance
{
	return eye_seperation_distance_;
}

float Camera::GetConvergenceDistance() //Used to return the convergence distance
{
	return convergence_distance_;
}

abfw::Vector3 Camera::GetCameraEye() //Used to return the camera eye (position)
{
	return camera_eye_;
}

abfw::Vector3 Camera::GetLeftEye() //Used to return the left eye (left eye position)
{
	return left_eye_;
}

abfw::Vector3 Camera::GetRightEye() //Used to return the right eye (right eye position)
{
	return right_eye_;
}

abfw::Vector3 Camera::GetCameraLookat() //Used to return the lookat (direction camera faces)
{
	return camera_lookat_;
}

abfw::Vector3 Camera::GetCameraLookatLeft() //Used to return the left lookat (direction left camera faces)
{
	return camera_lookat_left_;
}

abfw::Vector3 Camera::GetCameraLookatRight() //Used to return the right lookat (direction right camera faces)
{
	return camera_lookat_right_;
}

abfw::Vector3 Camera::GetCameraForward() //Used to return the cameras forward direction
{
	return camera_forward_;
}

abfw::Vector3 Camera::GetCameraUp() // Used to return the cameras up direction
{
	return camera_up_;
}

abfw::Vector3 Camera::GetCameraLeft() //Used to return the cameras left direction
{
	return camera_left_;
}

abfw::Quaternion Camera::GetRotation() //Used to return the cameras rotation
{
	return rotation_;
}

abfw::Matrix44 Camera::GetViewMatrix() //used to return the cameras view matrix
{
	return view_matrix;
}

abfw::Matrix44 Camera::GetLeftViewMatrix() //used to return the cameras left view matrix
{
	return left_view_matrix;
}

abfw::Matrix44 Camera::GetRightViewMatrix() //used to return the cameras right view matrix
{
	return right_view_matrix;
}

const abfw::Matrix44 Camera::GetTransform() //used to return the cameras transform matrix
{
	return transform_;
}

/////////////////////////////////////////////////////////////////////

void Camera::SetPitch(const float NewPitch) //used to set the cameras pitch
{
	m_pitch = NewPitch;
}

void Camera::SetYaw(const float NewYaw) //used to set the cameras yaw
{
	m_yaw = NewYaw;
}

void Camera::SetRoll(const float NewRoll) //used to set the cameras roll
{
	m_roll = NewRoll;
}

void Camera::SetNearPlane(const float near) //used to set the cameras near plane
{
	near_plane_ = near;
}

void Camera::SetFarPlane(const float far) //used to set the cameras far plane
{
	far_plane_ = far;
}

void Camera::SetFOV(const float fov) //used to set the cameras fov
{
	camera_fov_ = fov;
}

void Camera::SetEyeSeperationDistance(const float esd) //used to set the distance between the different camera eyes
{
	eye_seperation_distance_ = esd;
}

void Camera::SetRadius(const float radius)
{
	bounding_sphere_.set_radius(radius);
}

void Camera::SetSetup(const bool setup)
{
	setup_ = setup;
}

void Camera::SetConvergenceDistance(const float conDistance) //used to set the convegrgence distance between the different 3D images
{
	convergence_distance_ = conDistance;
}

void Camera::SetCameraEye(const abfw::Vector3 eye) //used to set the cameras eye (position)
{
	camera_eye_ = eye;
}

void Camera::SetCameraLookat(const abfw::Vector3 lookat)//used to set the cameras look at (direction)
{
	camera_lookat_ = lookat;
}

void Camera::SetCameraLookatLeft(const abfw::Vector3 lookatLeft) //used to set the left eye cameras lookat (direction)
{
	camera_lookat_left_ = lookatLeft;
}

void Camera::SetCameraLookatRight(const abfw::Vector3 lookatRight) //used to set the right eye cameras lookat (direction)
{
	camera_lookat_right_ = lookatRight;
}

void Camera::SetCameraUp(const abfw::Vector3 up) //used to set the cameras up direction
{
	camera_up_ = up;
}

void Camera::SetRotation(const abfw::Quaternion rotation) //used to set the left eye cameras rotation
{
	rotation_ = rotation;
}

void Camera::SetViewMatrix(const abfw::Matrix44 view) //used to set the cameras view matrix
{
	view_matrix = view;
}

void Camera::SetLeftViewMatrix(const abfw::Matrix44 leftView) //used to set the cameras left view matrix
{
	left_view_matrix = leftView;
}

void Camera::SetRightViewMatrix(const abfw::Matrix44 rightView) //used to set the cameras right view matrix
{
	right_view_matrix = rightView;
}

void Camera::SetTransform(const abfw::Matrix44 transform) //used to set the cameras transform matrix
{
	transform_ = transform;
}

///////////////////////////////////////////////////////////////////////////

void Camera::UpdatePosition(const abfw::Vector3 update) // used to update the cameras poisition
{
	camera_eye_ += update;
	camera_lookat_ += update;
}

void Camera::UpdateConDis(const float ConDis) // used to change the cameras convergence distance
{
	convergence_distance_ += ConDis;
}

void Camera::AddPitch(float PitchUpdate) //used to change the cameras pitch
{
	m_pitch += PitchUpdate;
}

void Camera::AddYaw(float YawUpdate) //used to change the cameras yaw
{
	m_yaw += YawUpdate;
}

void Camera::AddRoll(float RollUpdate) //used to change the cameras roll
{
	m_roll += RollUpdate;
}

////////////////////////////////////////////////////////////////////

void Camera::MoveFWRD() //when called the camera moves along the forward direction
{
	camera_eye_ += camera_forward_;

	camera_lookat_ -= camera_forward_;
}

void Camera::MoveBKWRD() //when called the camera moves backwards on the forward direction
{
	camera_eye_ -= camera_forward_;

	camera_lookat_ -= camera_forward_;
}

void Camera::MoveLEFT() //when called the camera moves along the left direction
{
	camera_eye_ -= (camera_left_ - abfw::Vector3(0.1, 0, 0));

	camera_lookat_ -= (camera_left_ - abfw::Vector3(0.1, 0, 0));
}

void Camera::MoveRIGHT() //when called the camera moves backwards along the left direction
{
	camera_eye_ += (camera_left_ - abfw::Vector3(0.1, 0, 0));

	camera_lookat_ += (camera_left_ - abfw::Vector3(0.1, 0, 0));
}

void Camera::MoveUP() //when called the camera moves along the up direction
{
	camera_eye_ += (camera_up_ - abfw::Vector3(0.1, 0, 0));

	camera_lookat_ += (camera_up_- abfw::Vector3(0.1, 0, 0));
}

void Camera::MoveDOWN() //when called the camera moves backwards along the up direction
{
	camera_eye_ -= (camera_up_ - abfw::Vector3(0.1, 0, 0));

	camera_lookat_ -= (camera_up_ - abfw::Vector3(0.1, 0, 0));
}

void Camera::RotateLEFT() // when called rotates the camera left
{
	//camera_lookat_.y -= 1.5;
}

void Camera::RotateRIGHT() // when called rotates the camera right
{
	//camera_lookat_.y += 1.5;
}

void Camera::RotateDOWN() // when called rotates the camera down
{
	//rotation_.x += 1.5;
}

void Camera::RotateUP() // when called rotates the camera up
{
	//rotation_.x -= 1.5;
}

///////////////////////////////////////////////////////////////////////

void Camera::Update() // used to stop the camera from leaving the designated area and returns it to the centre
{
	// camera restrictions
	if(camera_eye_.x >= 80)
	{
		camera_eye_ -= camera_left_;
		camera_lookat_ -= camera_left_;
	}
	else if(camera_eye_.x <= -80)
	{
		camera_eye_ += camera_left_;
		camera_lookat_ += camera_left_;
	}

	if(camera_eye_.y >= 80)
	{
		camera_eye_ -= camera_up_;
		camera_lookat_ -= camera_up_;
	}
	else if(camera_eye_.y <= -80)
	{
		camera_eye_ += camera_up_;
		camera_lookat_ += camera_up_;
	}

	// returns camera to centre of screen
	if(camera_eye_.x > 0)
	{
		camera_eye_ -= (camera_left_ - abfw::Vector3(0.9, 0, 0));
		camera_lookat_ -= (camera_left_- abfw::Vector3(0.9, 0, 0));
	}
	else if(camera_eye_.x < 0)
	{
		camera_eye_ += (camera_left_ - abfw::Vector3(0.9, 0, 0));
		camera_lookat_ += (camera_left_ - abfw::Vector3(0.9, 0, 0));
	}

	if(camera_eye_.y > 0)
	{
		camera_eye_ -= (camera_up_ - abfw::Vector3(0, 0.9, 0));
		camera_lookat_ -= (camera_up_ - abfw::Vector3(0, 0.9, 0));
	}
	else if(camera_eye_.y < 0)
	{
		camera_eye_ += (camera_up_ - abfw::Vector3(0, 0.9, 0));
		camera_lookat_ += (camera_up_ - abfw::Vector3(0, 0.9, 0));
	}
}

void Camera::Render() //renders and updates the camera variables when called
{

	if(setup_ == false)
	{
		// yaw = y  pitch = X  roll = Z 
		abfw::Matrix44 rotationMatrix;

		view_matrix.LookAt(camera_eye_, camera_lookat_, camera_up_);
		//view_matrix.LookAt(camera_eye_, camera_lookat_, abfw::Vector3(0.0f, 1.0f, 0.0f));
	
		float degToRad = 0.0174532925f;
		float cosY = cosf(rotation_.y * degToRad);
		float cosP = cosf(rotation_.x * degToRad);
		float cosR = cosf(rotation_.z * degToRad);
		float sinY = sinf(rotation_.y * degToRad);
		float sinP = sinf(rotation_.x * degToRad);
		float sinR = sinf(rotation_.z * degToRad);

		camera_forward_.x = sinY * cosP;
		camera_forward_.y = sinP;
		camera_forward_.z = cosP*-cosY;

		abfw::Vector3 camera_forward = camera_lookat_ - camera_eye_;
		camera_forward_.Normalise();

		camera_up_.x = -cosY * sinR - sinY * sinP * cosR;
		camera_up_.y = cosP * cosR;
		camera_up_.z = -sinY * sinR - sinP * cosR * -cosY;
		//camera_up_ = abfw::Vector3(0,1,0);
	
		// calculates a value thats perpendicular to both up and forward
		camera_left_ = camera_forward_.CrossProduct(camera_up_);

		// Setup where the camera is looking by default.
		camera_lookat_.x = camera_eye_.x + camera_forward_.x;
		camera_lookat_.y = camera_eye_.y + camera_forward_.y;
		camera_lookat_.z = camera_eye_.z + camera_forward_.z;

		//MoveBKWRD();
		setup_ = true;
	}
	
	camera_lookat_left_ = abfw::Vector3(camera_lookat_.x, camera_lookat_.y, camera_lookat_.z - (eye_seperation_distance_ * 0.5));
	camera_lookat_right_ = abfw::Vector3(camera_lookat_.x, camera_lookat_.y, camera_lookat_.z + (eye_seperation_distance_ * 0.5));

	// Finally create the view matrix from the three updated vectors.
	
////////////////////////////////////
//3D //

	left_eye_ = camera_eye_ + (camera_left_ * (eye_seperation_distance_ * 0.5));
	right_eye_ = camera_eye_ - (camera_left_ * (eye_seperation_distance_ * 0.5));

	left_view_matrix.LookAt(left_eye_, camera_lookat_left_, camera_up_);
	right_view_matrix.LookAt(right_eye_, camera_lookat_right_, camera_up_);

	position_.SetIdentity();
	position_.SetTranslation(camera_eye_);
	SetTransform(position_);

	return;
}