/*
	File name:	Camera.hpp
	Version:	1.0
	Date:		09/04/2014
	Autor:		Michael Little 

	Requires: d3dx10maths

	Desription:
	The camera class used in my DirectX project, it allows for smooth movement and can be rotated in any direction if need be.
	It utilises D3DX vectors to manipulate the camera and have it move around the map. 
*/
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

	// floats used to store the cameras positions
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

	// --- Constructor/Destructor
	Camera();
	Camera(const Camera&);
	~Camera();

	// -- Getters and Setters
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 GetForward();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

	float GetPitch();
	float GetYaw();
	float GetRoll();

	void SetPitch(float NewPitch);
	void SetYaw(float NewYaw);
	void SetRoll(float NewRoll);

	void AddPitch(float PitchUpdate);
	void AddYaw(float YawUpdate);
	void AddRoll(float RollUpdate);

	// Movement functions
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
}; // Camera class end

#endif