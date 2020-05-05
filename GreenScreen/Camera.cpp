#include "Camera.h"

Camera::Camera()
{
	m_PosX = 0.0f;
	m_PosY = 0.0f;
	m_PosZ = 0.0f;

	m_RotX = 0.0f;
	m_RotY = 0.0f;
	m_RotZ = 0.0f;

}

Camera::Camera(const Camera&)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_PosX = x;
	m_PosY = y;
	m_PosZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_RotX = x;
	m_RotY = y;
	m_RotZ = z;
}

XMVECTOR Camera::GetPosition()
{
	 XMFLOAT3 temp = { m_PosX, m_PosY, m_PosZ };
	return XMLoadFloat3(&temp);
}

XMVECTOR Camera::GetRotation()
{
	XMFLOAT3 temp = { m_RotX, m_RotY, m_RotZ };
	return XMLoadFloat3(&temp);
}

bool Camera::Initialize(int screenWidth, int screenHeight)
{
	// Setup the projection matrix.
	float fieldOfView = (float)XM_PI / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	float screenNear = 0.1f;
	float screenDepth = 10.0f;
	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = XMMatrixIdentity();

	return true;
}

void Camera::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up = { 0.0f, 1.0f, 0.0f };

	// Setup the position of the camera in the world.
	position = { m_PosX,  m_PosY, m_PosZ };

	// Setup where the camera is looking by default.
	lookAt = { 0.0f, 0.0f, 1.0f };

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_RotX * 0.0174532925f;
	yaw = m_RotY * 0.0174532925f;
	roll = m_RotZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	XMVector3TransformCoord(lookAt, rotationMatrix);
	XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	//Create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void Camera::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
}

void Camera::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
}
