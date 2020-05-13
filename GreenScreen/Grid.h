#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Structs.cpp"

class Grid
{
public:
	SimpleMesh meshData;
	bool Initialize(ID3D11Device* , ID3D11DeviceContext* , float, float, int, int);
	void Render(ID3D11DeviceContext*, ID3D11VertexShader*, ID3D11PixelShader*, ID3D11InputLayout*, ID3D11RenderTargetView*);
private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer = nullptr;
	int m_vertexCount, m_faceCount;

};
