#pragma once
#include <DirectXMath.h>

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

	bool Initialize(int, int);
	void Render();
	void GetViewMatrix(XMMATRIX&);
	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);

private:
	float m_PosX, m_PosY, m_PosZ;
	float m_RotX, m_RotY, m_RotZ;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
};

