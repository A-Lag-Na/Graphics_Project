#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "PixelShader.h"
#include "VertexShader.h"
#include "islandmodel.h"
#include"cubeobj.h"
//Using this to get access to SimpleMesh, and keep things clean/simple
#include "Structs.cpp"

using namespace DirectX;

class Model
{
public:



	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext* ,const OBJ_VERT*, const unsigned int*, unsigned int, unsigned int, float);
	void Shutdown();
	void Render(ID3D11DeviceContext*, ID3D11VertexShader*, ID3D11PixelShader*, ID3D11InputLayout*, ID3D11RenderTargetView*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*, ID3D11DeviceContext* ,const OBJ_VERT*, const unsigned int* );
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*, ID3D11VertexShader*, ID3D11PixelShader*, ID3D11InputLayout*, ID3D11RenderTargetView*);
	

private:
	SimpleMesh simpleMesh;
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer = nullptr;
	unsigned int m_vertexCount, m_indexCount;
	float m_scale;
};

