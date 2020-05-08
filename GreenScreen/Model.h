#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "PixelShader.h"
#include "VertexShader.h"
#include "islandmodel.h"

using namespace DirectX;

class Model
{
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT3 uv;
		XMFLOAT3 normal;

	};

public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext* ,const OBJ_VERT*, const unsigned int*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*, ID3D11DeviceContext* ,const OBJ_VERT*, const unsigned int* );
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

