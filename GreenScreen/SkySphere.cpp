#include "SkySphere.h"

SkySphere::SkySphere()
{

	m_model = 0;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

SkySphere::SkySphere(const SkySphere&)
{
}

SkySphere::~SkySphere()
{
}

bool SkySphere::Initialize(ID3D11Device* device, char filename[256])
{
	bool result;


	// Load in the sky dome model.
	result = LoadSkyDomeModel(filename);
	if (!result)
	{
		return false;
	}

	// Set the color at the top of the sky dome.
	gradient.apexColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the color at the center of the sky dome.
	gradient.centerColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	// Load the sky dome into a vertex and index buffer for rendering.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}





	return true;
}

void SkySphere::Shutdown()
{

	// Release the vertex and index buffer that were used for rendering the sky dome.
	ReleaseBuffers();

	// Release the sky dome model.
	ReleaseSkyDomeModel();

}

void SkySphere::Render(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* view, ID3D11ShaderResourceView* SRV = nullptr, ID3D11SamplerState* sampler = nullptr)
{
	// Render the sky dome.
	RenderBuffers(deviceContext, vertexShader, pixelShader, inputLayout,view,SRV,sampler);

}

int SkySphere::GetIndexCount()
{
	return m_indexCount;
}

bool SkySphere::LoadSkyDomeModel(char* filename)
{

	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new SimpleVertex[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].Pos.x >> m_model[i].Pos.y >> m_model[i].Pos.z;
		fin >> m_model[i].Tex.x >> m_model[i].Tex.y;
		fin >> m_model[i].Normal.x >> m_model[i].Normal.y >> m_model[i].Normal.z;
	}

	// Close the model file.
	fin.close();

	return true;
}

void SkySphere::ReleaseSkyDomeModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

bool SkySphere::InitializeBuffers(ID3D11Device* device)
{
	SimpleVertex* vertices;
	unsigned int* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new SimpleVertex[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned int[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].Pos = XMFLOAT3(m_model[i].Pos.x, m_model[i].Pos.y, m_model[i].Pos.z);
		indices[i] = i;
	}

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Gradient constant buffer
	CD3D11_BUFFER_DESC desc = CD3D11_BUFFER_DESC(sizeof(WVP), D3D11_BIND_CONSTANT_BUFFER);
	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&desc, sizeof(desc));
	desc = CD3D11_BUFFER_DESC(sizeof(Gradient), D3D11_BIND_CONSTANT_BUFFER);
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = &gradient;
	HRESULT hr = device->CreateBuffer(&desc, &srd, &gradientConstantBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void SkySphere::ReleaseBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void SkySphere::RenderBuffers(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* view, ID3D11ShaderResourceView* SRV = nullptr, ID3D11SamplerState* sampler = nullptr)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(SimpleVertex);
	unsigned int offset = 0;

	// setup the pipeline
	deviceContext->IASetInputLayout(inputLayout);
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	deviceContext->PSSetConstantBuffers(2, 1, &gradientConstantBuffer);

	//This contains the texture being loaded in.
	deviceContext->PSSetShaderResources(0, 1, &SRV);

	if (sampler)
	{
		//The sampler reads from the SRV to get the texture data.
		deviceContext->PSSetSamplers(0, 1, &sampler);
	}

	deviceContext->DrawIndexed(m_indexCount, 0, 0);
}

