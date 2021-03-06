#pragma once
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

void Camera::SetProjMatrix(XMMATRIX projMatrix)
{
	m_projectionMatrix = projMatrix;
}

XMFLOAT3 Camera::GetPosition()
{ 
	return { m_PosX, m_PosY, m_PosZ };
}

XMFLOAT3 Camera::GetRotation()
{	
	return { m_RotX, m_RotY, m_RotZ };
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

bool Camera::Initialize(int screenWidth, int screenHeight , float SCREEN_NEAR, float  SCREEN_DEPTH)
{
	// Setup the projection matrix.
	float fieldOfView = (float)XM_PI / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH( fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);
	
	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = XMMatrixIdentity();

	
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
	
	// added by clark
	m_viewMatrix = XMMatrixInverse(nullptr, m_viewMatrix);
	
	// not used, not really needed -clark
	m_worldMatrix = XMMatrixInverse(nullptr, m_viewMatrix);

	return true;
}

void Camera::Render(XMMATRIX& viewMatrix, bool& lightSwitch, Light& dirLight, PointLight& pointLight, SpotLight& spotLight)
{
	
	CameraMove(m_viewMatrix);
	ControlLights(lightSwitch, dirLight, pointLight, spotLight);
}

void Camera::CameraMove(XMMATRIX& myCamera)
{
	XMFLOAT4X4* w = new XMFLOAT4X4;
	XMStoreFloat4x4(w, myCamera);
	POINT m_currPos;
	// W, forwards camera control

	if (GetAsyncKeyState(0x57))
	//if (GetAsyncKeyState('W'))
	{
		XMMATRIX temp = XMMatrixTranslation(0.f, 0.f, .05f);
		myCamera = XMMatrixMultiply(temp, myCamera);
	}
	// S, backwards camera control
	if (GetAsyncKeyState(0x53))
	{
		XMMATRIX temp = XMMatrixTranslation(0, 0, -.05f);
		myCamera = XMMatrixMultiply(temp, myCamera);
	}
	// A, leftwards camera control
	if (GetAsyncKeyState(0x41))
	{
		XMMATRIX temp = XMMatrixTranslation(-.05f, 0, 0);
		myCamera = XMMatrixMultiply(temp, myCamera);
	}
	// D, rightwards camera control
	if (GetAsyncKeyState(0x44))
	{
		XMMATRIX temp = XMMatrixTranslation(.05f, 0, 0);
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
	
	GetCursorPos(&m_currPos);
	
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		if (m_currPos.x != m_oldPos.x || m_currPos.y != m_oldPos.y);
		{
			// changed by clark
			float CameraW[4] = { w->_41, w->_42, w->_43, w->_44 };
			float ydelta = m_currPos.y - m_oldPos.y;
			myCamera = XMMatrixMultiply(XMMatrixRotationX(0.01 * ydelta), myCamera);
			float xdelta = m_currPos.x - m_oldPos.x;
			myCamera = XMMatrixMultiply(myCamera, XMMatrixRotationY(0.01 * xdelta));
			XMStoreFloat4x4(w, myCamera);
			
			// changed by clark
			w->_41 = CameraW[0];
			w->_42 = CameraW[1];
			w->_43 = CameraW[2];
			w->_44 = CameraW[3];
			myCamera = XMLoadFloat4x4(w);
		}
	}
	m_oldPos = m_currPos;
	w = nullptr;
}

void Camera::CameraLookAt(XMMATRIX& myCamera, XMMATRIX& myObject)
{
	XMVECTOR tempPos = myObject.r[3];
	m_viewMatrix = XMMatrixLookAtLH(tempPos, lookAt, up);
}

void Camera::ControlLights(bool& lightSwitch, Light& dirLight, PointLight& pointLight, SpotLight& spotLight)
{

	//R, dir light off
	if (GetAsyncKeyState(0x52))
	{
		dirLight.vLightColor = XMFLOAT4(0.f,0.f,0.f,0.2f);
	}
	//4, dir light on
	if (GetAsyncKeyState(0x34))
	{
		dirLight.vLightColor = XMFLOAT4(0.f, 0.f, 1.f, 0.2f);
	}
	//T, change directional light direction
	if (GetAsyncKeyState(0x54))
	{
		dirLight.vLightDir.x -= 0.1f;
	}
	//F, disable point light
	if (GetAsyncKeyState(0x46))
	{
		pointLight.light.vLightColor = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	}
	//G, enable point light
	if (GetAsyncKeyState(0x47))
	{
		pointLight.light.vLightColor = XMFLOAT4(1.f, 0.f, 0.f, 0.f);
	}
	//C, disable spot light
	if (GetAsyncKeyState(0x56))
	{
		spotLight.light.vLightColor = XMFLOAT4(0.f, 0.f, 0.f, 0.0f);
	}
	//V, enable spot light
	if (GetAsyncKeyState(0x43))
	{
		spotLight.light.vLightColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	}


	//Arrow keys to move point light
	//Left arrow, -X direction
	if (GetAsyncKeyState(0x25))
	{
		pointLight.light.vLightDir.x -= 0.01f;
	}
	//Right arrow, +X direction
	if (GetAsyncKeyState(0x27))
	{
		pointLight.light.vLightDir.x += 0.01f;
	}
	//Right shift, +Y direction
	if (GetAsyncKeyState(0xA1))
	{
		pointLight.light.vLightDir.y += 0.01f;
	}
	//Right control, -Y direction
	if (GetAsyncKeyState(0xA3))
	{
		pointLight.light.vLightDir.y -= 0.01f;
	}
	//Up arrow, +Z direction (away from camera)
	if (GetAsyncKeyState(0x26))
	{
		pointLight.light.vLightDir.z += 0.01f;
	}
	//Down arrow, -Z direction (towards camera)
	if (GetAsyncKeyState(0x28))
	{
		pointLight.light.vLightDir.z -= 0.01f;
	}

	//Keys to adjust spot light
	//numpad 4, -X direction
	if (GetAsyncKeyState(0x64))
	{
		spotLight.coneDir.x -= 0.05f;
	}
	//numpad 6, +X direction
	if (GetAsyncKeyState(0x66))
	{
		spotLight.coneDir.x += 0.05f;
	}
	//numpad 9, +Y direction
	if (GetAsyncKeyState(0x69))
	{
		spotLight.coneDir.y += 0.05f;
	}
	//numpad 7, -Y direction
	if (GetAsyncKeyState(0x67))
	{
		spotLight.coneDir.y -= 0.05f;
	}
	//numpad 8, +Z direction (away from camera)
	if (GetAsyncKeyState(0x68))
	{
		spotLight.coneDir.z += 0.05f;
	}
	//numpad 5, -Z direction (towards camera)
	if (GetAsyncKeyState(0x65))
	{
		spotLight.coneDir.z -= 0.05f;
	}
	//J, -X position
	if (GetAsyncKeyState(0x4A))
	{
		spotLight.light.vLightDir.x -= 0.05f;
	}
	//L, +X position
	if (GetAsyncKeyState(0x4C))
	{
		spotLight.light.vLightDir.x += 0.05f;
	}
	//O, +Y position
	if (GetAsyncKeyState(0x55))
	{
		spotLight.light.vLightDir.y += 0.05f;
	}
	//U, -Y position
	if (GetAsyncKeyState(0x4F))
	{
		spotLight.light.vLightDir.y -= 0.05f;
	}
	//I, +Z position (away from camera)
	if (GetAsyncKeyState(0x49))
	{
		spotLight.light.vLightDir.z += 0.05f;
	}
	//K, -Z position (towards camera)
	if (GetAsyncKeyState(0x4B))
	{
		spotLight.light.vLightDir.z -= 0.05f;
	}

}