#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include<string> 
#include "Structs.cpp"

using namespace std;

class ModelLoading
{
public:
	bool LoadOBJ(char*, vector <XMFLOAT3>&, vector <XMFLOAT2>&, vector <XMFLOAT3>&);
	bool LoadModelBuffers(char*, ID3D11Device*, ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*, ID3D11VertexShader*, ID3D11PixelShader*, ID3D11InputLayout*, ID3D11RenderTargetView*, ID3D11ShaderResourceView*, ID3D11SamplerState*);
private:
	bool result;
	SimpleMesh simpleMesh;
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer = nullptr;
	unsigned int m_vertexCount, m_indexCount;
	float m_scale;
	
};

