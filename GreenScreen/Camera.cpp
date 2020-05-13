#pragma once
#include "Camera.h"

//I threw my (functional) camera code from my old project at the bottom of this file. Hope it can help.

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

bool Camera::Initialize(int screenWidth, int screenHeight , float SCREEN_NEAR, float  SCREEN_DEPTH)
{
	// Setup the projection matrix.
	float fieldOfView = (float)XM_PI / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH( fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);
	

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = XMMatrixIdentity();

	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix, tempView;


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
	m_worldMatrix = XMMatrixInverse(nullptr, m_viewMatrix);

	return true;
}

void Camera::Render(XMMATRIX& viewMatrix)
{
	CameraMove(m_viewMatrix);
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void Camera::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
}

void Camera::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
}


bool Camera::CameraMove(XMMATRIX& myCamera)
{
	XMFLOAT4X4* w = new XMFLOAT4X4;
	XMStoreFloat4x4(w, myCamera);
	POINT m_currPos;
	// W, forwards camera control

	if (GetAsyncKeyState(0x57))
	{
		XMMATRIX temp = XMMatrixTranslation(0.f, 0.f, -.05f);
		myCamera = XMMatrixMultiply(temp, myCamera);
	}
	// S, backwards camera control
	if (GetAsyncKeyState(0x53))
	{
		XMMATRIX temp = XMMatrixTranslation(0, 0, .05f);
		myCamera = XMMatrixMultiply(temp, myCamera);
	}
	// A, leftwards camera control
	if (GetAsyncKeyState(0x41))
	{
		XMMATRIX temp = XMMatrixTranslation(.05f, 0, 0);
		myCamera = XMMatrixMultiply(temp, myCamera);
	}
	// D, rightwards camera control
	if (GetAsyncKeyState(0x44))
	{
		XMMATRIX temp = XMMatrixTranslation(-.05f, 0, 0);
		myCamera = XMMatrixMultiply(temp, myCamera);
	}
	// Q, upwards camera control
	if (GetAsyncKeyState(0x51))
	{
		XMMATRIX temp = XMMatrixTranslation(0, -0.05f, 0);
		myCamera = XMMatrixMultiply(temp, myCamera);
	}
	// E, downwards camera control
	if (GetAsyncKeyState(0x45))
	{
		XMMATRIX temp = XMMatrixTranslation(0, 0.05f, 0);
		myCamera = XMMatrixMultiply(temp, myCamera);
	}
	// R, toggle overhead directional light
	//lightSwitch = GetAsyncKeyState(0x52);
	
	GetCursorPos(&m_currPos);
	
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		if (m_currPos.x != m_oldPos.x || m_currPos.y != m_oldPos.y);
		{

			int CameraW[4] = { w->_14, w->_24, w->_34, w->_44 };
			float ydelta = m_currPos.y - m_oldPos.y;
			myCamera = XMMatrixMultiply(myCamera, XMMatrixRotationX(0.01 * ydelta));
			float xdelta = m_currPos.x - m_oldPos.x;
			myCamera = XMMatrixMultiply(myCamera, XMMatrixRotationY(0.01 * xdelta));
			XMStoreFloat4x4(w, myCamera);
			w->_14 = CameraW[0];
			w->_24 = CameraW[1];
			w->_34 = CameraW[2];
			w->_44 = CameraW[3];
			myCamera = XMLoadFloat4x4(w);
		}
	}
	m_oldPos = m_currPos;
	w = nullptr;
	//return lightSwitch;
	return true;
}