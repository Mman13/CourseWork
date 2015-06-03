////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_

//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:

	float m_x;
	float m_y;
	float m_z;
	float m_roll;
	float m_pitch;
	float m_yaw;
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;
	
	D3DXVECTOR3 forward;
	D3DXVECTOR3 backwards;
	D3DXVECTOR3 up;
	D3DXVECTOR3 left;

	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	/*void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);*/

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

	void RenderReflection(D3DXVECTOR3 mirrorPos, D3DXVECTOR3 mirrorNorm);
	D3DXMATRIX GetReflectionViewMatrix();

	float GetPitch();
	float GetYaw();
	float GetRoll();

	void SetPitch(float NewPitch);
	void SetYaw(float NewYaw);
	void SetRoll(float NewRoll);

	void AddPitch(float PitchUpdate);
	void AddYaw(float YawUpdate);
	void AddRoll(float RollUpdate);

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
	void Update();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
	D3DXVECTOR3 m_cameraPosition;
	D3DXMATRIX m_reflectionViewMatrix;
};

#endif