#pragma once

#include <DirectXMath.h>
#include <windows.h>
#include "Structs.cpp"

using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetProjMatrix(XMMATRIX);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	bool Initialize(int screenWidth, int screenHeight, float SCREEN_NEAR, float  SCREEN_DEPTH);
	void Render(XMMATRIX&, bool&, Light&, PointLight&, SpotLight&);
	void GetViewMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetProjectionMatrix(XMMATRIX&);
	void CameraMove(XMMATRIX&);
	void CameraLookAt(XMMATRIX&, XMMATRIX&);
	void ControlLights(bool&, Light&, PointLight&, SpotLight&);

private:

	float m_PosX, m_PosY, m_PosZ;
	float m_RotX, m_RotY, m_RotZ;
	XMVECTOR up, position, lookAt;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_projectionMatrix;
	POINT m_oldPos;
};

