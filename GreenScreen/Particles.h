#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Structs.cpp"

class Particles
{
private:
	struct ParticleType
	{
		float positionX, positionY, positionZ;
		float red, green, blue;
		float velocity;
		bool active;
	};


public:
	Particles();
	Particles(const Particles&);
	~Particles();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*, ID3D11VertexShader*, ID3D11PixelShader*, ID3D11InputLayout*, ID3D11RenderTargetView*, ID3D11ShaderResourceView*, ID3D11SamplerState*);

	int GetIndexCount();

private:


	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext*);

	void RenderBuffers(ID3D11DeviceContext*, ID3D11VertexShader*, ID3D11PixelShader*, ID3D11InputLayout*, ID3D11RenderTargetView*, ID3D11ShaderResourceView*, ID3D11SamplerState*);

private:
	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_maxParticles;

	int m_currentParticleCount;
	float m_accumulatedTime;

	ParticleType* m_particleList;
	int m_vertexCount, m_indexCount;
	SimpleVertex* m_vertices;
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
};

