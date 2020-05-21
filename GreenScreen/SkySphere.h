#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Structs.cpp"
#include "SkyboxPixelShader.h"
#include "SkyboxVertexShader.h"
#include <fstream>

using namespace std;
using namespace DirectX;

class SkySphere
{
public:
	SkySphere();
	SkySphere(const SkySphere&);
	~SkySphere();

	bool Initialize(ID3D11Device*, char filename[256]);
	void Shutdown();
	void Render(ID3D11DeviceContext*, ID3D11VertexShader*, ID3D11PixelShader*, ID3D11InputLayout*, ID3D11RenderTargetView*, ID3D11ShaderResourceView*, ID3D11SamplerState*);

	int GetIndexCount();

private:
	bool LoadSkyDomeModel(char*);
	void ReleaseSkyDomeModel();

	bool InitializeBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*, ID3D11VertexShader*, ID3D11PixelShader*, ID3D11InputLayout*, ID3D11RenderTargetView*, ID3D11ShaderResourceView*, ID3D11SamplerState*);

private:
	SimpleVertex* m_model;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	ID3D11Buffer* gradientConstantBuffer;
	Gradient gradient;
};

