#include "Camera.h"

Camera::Camera()
{
	 m_posX = 0, m_posY = 0, m_posZ = 0;
	 m_rotX = 0, m_rotY = 0, m_rotZ = 0;
}

Camera::~Camera() {}

void Camera::SetPos(float x, float y, float z)
{
	m_posX = x; m_posY = y; m_posZ = z;
	return;
}

void Camera::SetRot(float x, float y, float z)
{
	m_rotX = x; m_rotY = y; m_rotZ = z;
	return;
}

XMFLOAT3 Camera::GetPos()
{
	return XMFLOAT3(m_posX, m_posY, m_posZ);
}

XMFLOAT3 Camera::GetRot()
{
	return XMFLOAT3(m_rotX, m_rotY, m_rotZ);
}

void Camera::Render()
{
	XMVECTOR up , pos, lookAt;
	XMMATRIX rotationMatrix;
	float yaw, pitch, roll;
	float radian = 0.0174532925f;

	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	pos = XMVectorSet(m_posX, m_posY, m_posZ, 0.0f);
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	
	pitch = m_rotX * radian;
	yaw = m_rotY * radian;
	roll = m_rotZ * radian;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	XMVector3TransformCoord(lookAt, rotationMatrix);
	XMVector3TransformCoord(up, rotationMatrix);

	lookAt = lookAt + pos;

	m_viewMatrix = XMMatrixLookAtLH(pos, lookAt, up);

	return;
}

XMMATRIX Camera::GetViewMatrix()
{
	return m_viewMatrix;
}