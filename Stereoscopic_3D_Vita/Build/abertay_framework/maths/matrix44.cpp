#include <maths/matrix44.h>
#include <maths/vector3.h>
#include <maths/vector4.h>
#include <maths/quaternion.h>
#include <cmath>

namespace abfw
{

void Matrix44::SetIdentity()
{
	for(Int32 i=0;i<4;++i)
		for(Int32 j=0;j<4;++j)
			if (i == j)
				m[i][j] = 1.0f;
			else
				m[i][j] = 0.0f;

}

void Matrix44::SetZero()
{
	for(Int32 i=0;i<4;++i)
		for(Int32 j=0;j<4;++j)
			m[i][j] = 0.0f;
}

const Matrix44 Matrix44::operator*(const Matrix44& matrix) const
{
	Matrix44 result;

	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			result.m[i][j] =  m[i][0] * matrix.m[0][j] + m[i][1] * matrix.m[1][j] + m[i][2] * matrix.m[2][j] + m[i][3] * matrix.m[3][j];

	return result; 
}

void Matrix44::LookAt(const Vector3& eye, const Vector3& lookat, const Vector3& up)
{
	SetIdentity();
	Vector3 forward = eye - lookat;
	forward.Normalise();

	Vector3 side;
	side = up.CrossProduct(forward);
	side.Normalise();

	Vector3 calculated_up;
	calculated_up = forward.CrossProduct(side);

	m[0][0] = side.x;
	m[1][0] = side.y;
	m[2][0] = side.z;
	m[0][1] = calculated_up.x;
	m[1][1] = calculated_up.y;
	m[2][1] = calculated_up.z;
	m[0][2] = forward.x;
	m[1][2] = forward.y;
	m[2][2] = forward.z;
	m[3][0] = -side.DotProduct(eye);
	m[3][1] = -calculated_up.DotProduct(eye);
	m[3][2] = -forward.DotProduct(eye);
}

void Matrix44::PerspectiveFrustumGL(const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance)
{
	// transform to openGL clip space
	// cube dimensions (-1, -1, -1) to (1, 1, 1)
	float a, b, c, d;

	a = (right + left) / (right - left);
	b = (top + bottom) / (top - bottom);
	c = -(far_distance+near_distance) / (far_distance - near_distance);
	d = -(2.0f*far_distance*near_distance) / (far_distance - near_distance);

	SetZero();
	m[0][0] = (2.0f*near_distance) / (right - left);
	m[1][1] = (2.0f*near_distance) / (top - bottom);
	m[2][0] = a;
	m[2][1] = b;
	m[2][2] = c;
	m[2][3] = -1.0f;
	m[3][2] = d;
}

void Matrix44::PerspectiveFovGL(const float fov, const float aspect_ratio, const float near_distance, const float far_distance)
{
	float height = std::tanf(fov*0.5f)*near_distance;
	float width = height*aspect_ratio;
	PerspectiveFrustumGL(-width, width, height, -height, near_distance, far_distance);
}

	
void Matrix44::OrthographicFrustumGL(const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance)
{
	// transform to openGL clip space
	// cube dimensions (-1, -1, -1) to (1, 1, 1)
	SetIdentity();
	m[0][0] = (2.0f) / (right - left);
	m[3][0] = -(right+left) / (right - left);
	m[1][1] = (2.0f) / (top - bottom);
	m[3][1] = -(top+bottom) / (top - bottom);
	m[2][2] = (2.0f) / (far_distance - near_distance);
	m[3][2] = -(far_distance+near_distance) / (far_distance - near_distance);
}

void Matrix44::PerspectiveFrustumD3D(const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance)
{
	// transform to D3D clip space
	// cube dimensions (-1, -1, 0) to (1, 1, 1)

	float a, b, c, d;

	a = (right + left) / (right - left);
	b = (top + bottom) / (top - bottom);
	c = (far_distance) / (near_distance - far_distance);
	d = (far_distance*near_distance) / (near_distance - far_distance);

	SetZero();
	m[0][0] = (2.0f*near_distance) / (right - left);
	m[1][1] = (2.0f*near_distance) / (top - bottom);
	m[2][0] = a;
	m[2][1] = b;
	m[2][2] = c;
	m[2][3] = -1.0f;
	m[3][2] = d;
}

void Matrix44::PerspectiveFovD3D(const float fov, const float aspect_ratio, const float near_distance, const float far_distance)
{
	float height = std::tanf(fov*0.5f)*near_distance;
	float width = height*aspect_ratio;
	PerspectiveFrustumD3D(-width, width, height, -height, near_distance, far_distance);
}

	
void Matrix44::OrthographicFrustumLHD3D(const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance)
{
	// transform to D3D clip space
	// cube dimensions (-1, -1, 0) to (1, 1, 1)
	SetIdentity();
	m[0][0] = (2.0f) / (right - left);
	m[3][0] = (right+left) / (left - right);
	m[1][1] = (2.0f) / (top - bottom);
	m[3][1] = (top+bottom) / (bottom - top);
	m[2][2] = (1.0f) / (far_distance - near_distance);
	m[3][2] = (near_distance) / (near_distance - far_distance);
}

void Matrix44::OrthographicFrustumRHD3D(const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance)
{
	// transform to D3D clip space
	// cube dimensions (-1, -1, 0) to (1, 1, 1)
	SetIdentity();
	m[0][0] = (2.0f) / (right - left);
	m[3][0] = (right+left) / (left - right);
	m[1][1] = (2.0f) / (top - bottom);
	m[3][1] = (top+bottom) / (bottom - top);
	m[2][2] = (1.0f) / (near_distance - far_distance);
	m[3][2] = (near_distance) / (near_distance - far_distance);
}

void Matrix44::RotationX(float _radiansRot)
{
	m[0][0] = 1.0f;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = std::cosf(_radiansRot);
	m[1][2] = std::sinf(_radiansRot);
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = -std::sinf(_radiansRot);
	m[2][2] = std::cosf(_radiansRot);
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

void Matrix44::RotationY(float _radiansRot)
{
	m[0][0] = std::cosf(_radiansRot);
	m[0][1] = 0.0f;
	m[0][2] = -std::sinf(_radiansRot);
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 1.0f;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = std::sinf(_radiansRot);
	m[2][1] = 0.0f;
	m[2][2] = std::cosf(_radiansRot);
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

void Matrix44::RotationZ(float _radiansRot)
{
	m[0][0] = std::cosf(_radiansRot);
	m[0][1] = std::sinf(_radiansRot);
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = -std::sinf(_radiansRot);
	m[1][1] = std::cosf(_radiansRot);
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

void Matrix44::Rotation(const class Quaternion& quat)
{
	// this function assumes the quaternion is normalised
	float sqw = quat.w*quat.w;
    float sqx = quat.x*quat.x;
    float sqy = quat.y*quat.y;
    float sqz = quat.z*quat.z;

    m[0][0] = ( sqx - sqy - sqz + sqw);
    m[1][1] = (-sqx + sqy - sqz + sqw);
    m[2][2] = (-sqx - sqy + sqz + sqw);
    
    float tmp1 = quat.x*quat.y;
    float tmp2 = quat.z*quat.w;
    m[0][1] = 2.0f * (tmp1 + tmp2);
    m[1][0] = 2.0f * (tmp1 - tmp2);
    
    tmp1 = quat.x*quat.z;
    tmp2 = quat.y*quat.w;
    m[0][2] = 2.0f * (tmp1 - tmp2);
    m[2][0] = 2.0f * (tmp1 + tmp2);
    tmp1 = quat.y*quat.z;
    tmp2 = quat.x*quat.w;
    m[1][2] = 2.0f * (tmp1 + tmp2);
    m[2][1] = 2.0f * (tmp1 - tmp2);

	m[0][3] = 0.0f;
	m[1][3] = 0.0f;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

void Matrix44::Scale(const Vector3& scale)
{
	SetIdentity();
	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
}

Vector3 Matrix44::GetScale() const
{
	Vector3 scale_x(m[0][0], m[0][1], m[0][2]);
	Vector3 scale_y(m[1][0], m[1][1], m[1][2]);
	Vector3 scale_z(m[2][0], m[2][1], m[2][2]);

	return Vector3(scale_x.Length(), scale_y.Length(), scale_z.Length());
}

void Matrix44::SetTranslation(const Vector3& _trans)
{
	m[3][0] = _trans.x;
	m[3][1] = _trans.y;
	m[3][2] = _trans.z;
}


const Vector3 Matrix44::GetTranslation() const
{
	return Vector3(m[3][0], m[3][1], m[3][2]);
}

void Matrix44::Transpose(const Matrix44& matrix)
{
	for(Int32 rowNum=0;rowNum<4;++rowNum)
		for(Int32 columnNum=0;columnNum<4;++columnNum)
			m[rowNum][columnNum] = matrix.m[columnNum][rowNum];
}

void Matrix44::AffineInverse(const Matrix44& matrix)
{
	Transpose(matrix);
	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[0][3] = 0.0f;
	m[1][3] = 0.0f;
	m[2][3] = 0.0f;

	Vector3 pos, invTrans;

	pos = matrix.GetTranslation();
	pos = pos * -1.0f;

	invTrans = pos.Transform(*this);

	SetTranslation(invTrans);
}

void Matrix44::NormaliseRotation()
{
	abfw::Quaternion rotation;
	rotation.SetFromMatrix(*this);
	abfw::Vector3 translation = GetTranslation();

	rotation.Normalise();
	Rotation(rotation);
	SetTranslation(translation);
}

	float Matrix44::CalculateDeterminant() const
	{
		float t[3], v[4];

		t[0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
		t[1] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
		t[2] = m[1][2] * m[2][3] - m[1][3] * m[2][2];
		v[0] = m[1][1] * t[0] - m[2][1] * t[1] + m[3][1] * t[2];
		v[1] = -m[1][0] * t[0] + m[2][0] * t[1] - m[3][0] * t[2];

		t[0] = m[1][0] * m[2][1] - m[2][0] * m[1][1];
		t[1] = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		t[2] = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		v[2] = m[3][3] * t[0] - m[2][3] * t[1] + m[1][3] * t[2];
		v[3] = -m[3][2] * t[0] + m[2][2] * t[1] - m[1][2] * t[2];

		return m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3];
	}

	void Matrix44::Inverse(const Matrix44 matrix, float* determinant)
	{
		int a, i, j;
		Vector4 v, vec[3];
		float det;

		det = matrix.CalculateDeterminant();
		if ( det!= 0.0f )
		{
			for (i=0; i<4; i++)
			{
				for (j=0; j<4; j++)
				{
					if (j != i )
					{
						a = j;
						if ( j > i )
							a = a-1;
						vec[a].x = matrix.m[j][0];
						vec[a].y = matrix.m[j][1];
						vec[a].z = matrix.m[j][2];
						vec[a].w = matrix.m[j][3];
					}
				}

				v = vec[0].CrossProduct(vec[1], vec[2]);
				m[0][i] = std::pow(-1.0f, i) * v.x / det;
				m[1][i] = std::pow(-1.0f, i) * v.y / det;
				m[2][i] = std::pow(-1.0f, i) * v.z / det;
				m[3][i] = std::pow(-1.0f, i) * v.w / det;
			}
	   }

		if(determinant)
			*determinant = det;
	}


}