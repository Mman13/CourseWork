#ifndef _ABFW_MATRIX_44_H
#define _ABFW_MATRIX_44_H

#include <abertay_framework.h>
#include <cstdlib>

namespace abfw
{
	class Vector3;
	class Quaternion;
	class Vector4;

	class Matrix44
	{
	public:
		void SetIdentity();
		void SetZero();
		void LookAt(const abfw::Vector3& eye, const abfw::Vector3& lookat, const abfw::Vector3& up);
		void PerspectiveFrustumGL(const float left, const float right, const float top, const float bottom, const float near_dist, const float far_dist);
		void PerspectiveFovGL(const float fov, const float aspect_ratio, const float near_dist, const float far_dist);
		void OrthographicFrustumGL(const float left, const float right, const float top, const float bottom, const float near_dist, const float far_dist);
		void PerspectiveFrustumD3D(const float left, const float right, const float top, const float bottom, const float near_dist, const float far_dist);
		void PerspectiveFovD3D(const float fov, const float aspect_ratio, const float near_dist, const float far_dist);
		void OrthographicFrustumLHD3D(const float left, const float right, const float top, const float bottom, const float near_dist, const float far_dist);
		void OrthographicFrustumRHD3D(const float left, const float right, const float top, const float bottom, const float near_dist, const float far_dist);
		inline void PerspectiveFrustumVita(const float left, const float right, const float top, const float bottom, const float near_dist, const float far_dist)
		{
			PerspectiveFrustumGL(left, right, top, bottom, near_dist, far_dist);
		}
	
		inline void PerspectiveFovVita(const float fov, const float aspect_ratio, const float near_dist, const float far_dist)
		{
			PerspectiveFovGL(fov, aspect_ratio, near_dist, far_dist);
		}
	
		inline void OrthographicFrustumVita(const float left, const float right, const float top, const float bottom, const float near_dist, const float far_dist)
		{
			OrthographicFrustumGL(left, right, top, bottom, near_dist, far_dist);
		}

		inline void PerspectiveFrustumPS4(const float left, const float right, const float top, const float bottom, const float near_dist, const float far_dist)
		{
			PerspectiveFrustumGL(left, right, top, bottom, near_dist, far_dist);
		}
	
		inline void PerspectiveFovPS4(const float fov, const float aspect_ratio, const float near_dist, const float far_dist)
		{
			PerspectiveFovGL(fov, aspect_ratio, near_dist, far_dist);
		}
	
		inline void OrthographicFrustumPS4(const float left, const float right, const float top, const float bottom, const float near_dist, const float far_dist)
		{
			OrthographicFrustumGL(left, right, top, bottom, near_dist, far_dist);
		}
	
		void RotationX(float radiansRot);
		void RotationY(float radiansRot);
		void RotationZ(float radiansRot);
		void Rotation(const abfw::Quaternion& quat);
		void Scale(const abfw::Vector3& scale);
		Vector3 GetScale() const;
		void SetTranslation(const abfw::Vector3& trans);
		const Vector3 GetTranslation() const;
		void Transpose(const Matrix44& matrix);
		void AffineInverse(const Matrix44& matrix);
		void NormaliseRotation();
		float CalculateDeterminant() const;
		void Inverse(const Matrix44 matrix, float* determinant = NULL);


		const Matrix44 operator*(const Matrix44& matrix) const;

		float m[4][4];
	};
}

#endif // _ABFW_MATRIX_44_H