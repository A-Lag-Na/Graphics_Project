#pragma once

#include <DirectXMath.h>
#include <windows.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMVECTOR GetPosition();
	XMVECTOR GetRotation();

	bool Initialize(int screenWidth, int screenHeight, float SCREEN_NEAR, float  SCREEN_DEPTH);
	void Render(XMMATRIX&);
	void GetViewMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetProjectionMatrix(XMMATRIX&);
	bool CameraMove(XMMATRIX&);

private:
	float m_PosX, m_PosY, m_PosZ;
	float m_RotX, m_RotY, m_RotZ;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_projectionMatrix;
	POINT m_oldPos;
};

