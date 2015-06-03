////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Camera.h"

Camera::Camera()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 180.0f;
	m_rotationZ = 0.0f;

}


Camera::Camera(const Camera& other)
{
}


Camera::~Camera()
{
}

float Camera::GetPitch()
{
	return m_pitch;
}

float Camera::GetYaw()
{
	return m_yaw;
}

float Camera::GetRoll()
{
	return m_roll;
}


void Camera::SetPitch(float NewPitch)
{
	m_pitch = NewPitch;
}

void Camera::SetYaw(float NewYaw)
{
	m_yaw = NewYaw;
}

void Camera::SetRoll(float NewRoll)
{
	m_roll = NewRoll;
}



void Camera::AddPitch(float PitchUpdate)
{
	m_pitch += PitchUpdate;
}

void Camera::AddYaw(float YawUpdate)
{
	m_yaw += YawUpdate;
}

void Camera::AddRoll(float RollUpdate)
{
	m_roll += RollUpdate;
}

void Camera::MoveFWRD()
{
	m_positionX += forward.x;
	m_positionY += forward.y;
	m_positionZ += forward.z;
}

void Camera::MoveBKWRD()
{
	m_positionX -= forward.x;
	m_positionY -= forward.y;
	m_positionZ -= forward.z;
}

void Camera::MoveLEFT()
{
	m_positionX += left.x;
	m_positionY += left.y;
	m_positionZ += left.z;
}

void Camera::MoveRIGHT()
{
	m_positionX -= left.x;
	m_positionY -= left.y;
	m_positionZ -= left.z;
}

void Camera::MoveUP()
{
	m_positionX += up.x;
	m_positionY += up.y;
	m_positionZ += up.z;
}

void Camera::MoveDOWN()
{
	m_positionX -= up.x;
	m_positionY -= up.y;
	m_positionZ -= up.z;
}

void Camera::RotateLEFT()
{
	m_rotationY -= 1.5;
}

void Camera::RotateRIGHT()
{
	m_rotationY += 1.5;
}

void Camera::RotateDOWN()
{
	m_rotationX += 1.5;
}

void Camera::RotateUP()
{
	m_rotationX -= 1.5;
}

void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
	return m_cameraPosition;
}

D3DXVECTOR3 Camera::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	// yaw = y  pitch = X  roll = Z 
	D3DXMATRIX rotationMatrix;

	float degToRad = 0.0174532925f;
	float cosY = cosf(m_rotationY * degToRad);
	float cosP = cosf(m_rotationX * degToRad);
	float cosR = cosf(m_rotationZ * degToRad);
	float sinY = sinf(m_rotationY * degToRad);
	float sinP = sinf(m_rotationX * degToRad);
	float sinR = sinf(m_rotationZ * degToRad);

	forward.x = sinY * cosP;
	forward.y = sinP;
	forward.z = cosP*-cosY;

	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY * sinR - sinP * cosR * -cosY;
	
	// uses D3DXVec3Cross to calculate a value thats perpendicular to both up and forward
	D3DXVec3Cross ( &left, &forward, &up);

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = m_positionX + forward.x;
	lookAt.y = m_positionY + forward.y;
	lookAt.z = m_positionZ + forward.z;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void Camera::RenderReflection(D3DXVECTOR3 mirrorPos, D3DXVECTOR3 mirrorNorm)
{
	D3DXVECTOR3 up, position, lookAt;
	
	lookAt = GetPosition() + ( -2 *D3DXVec3Dot(&mirrorNorm, &forward) * mirrorNorm + forward);

	//positionx, y z = camera, x y-*2var, z
	// difference in height, between the floor shiney surface and camera
	position.x = m_positionX;
	position.y = m_positionY; //- (2*3);
	position.z = m_positionZ;

	//lookAt = (mirrorPos - m_positionX) + ((position.y)*(position.y));
	lookAt.x = position.x + forward.x;
	lookAt.y = position.y + forward.y;
	lookAt.z = position.z + forward.z;

	// Setup the vector that points upwards.
	up.x = 0; //-cosY * sinR - sinY * sinP * cosR;
	up.y = 1; // cosP * cosR;
	up.z = 0; //-sinY * sinR - sinP * cosR * -cosY;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&m_reflectionViewMatrix, &position, &lookAt, &up);

	return;
}

D3DXMATRIX Camera::GetReflectionViewMatrix()
{
	return m_reflectionViewMatrix;
}
