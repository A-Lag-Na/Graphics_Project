#pragma once
#include "Model.h"

Model::Model()
{
	simpleMesh;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_vertexCount = 0;
	m_indexCount = 0;
	m_scale = 1.f;
};

Model::Model(const Model& other)
{
	simpleMesh = other.simpleMesh;
	m_vertexBuffer = other.m_vertexBuffer;
	m_indexBuffer = other.m_indexBuffer;
	m_vertexCount = other.m_vertexCount;
	m_indexCount = other.m_indexCount;
	m_scale = other.m_scale;
}


Model::~Model()
{
	simpleMesh.vertexList.~vector();
	simpleMesh.indicesList.~vector();
}

void Model::Shutdown()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	// Release the vertex and index buffers.
	ShutdownBuffers();

	return;
}

void Model::ShutdownBuffers()
{
	if (m_vertexBuffer) m_vertexBuffer->Release();
	if (m_indexBuffer)  m_indexBuffer->Release();

	return;
}

int Model::GetIndexCount()
{
	return m_indexCount;
}

//Higher scaling factor == smaller model
bool Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,const OBJ_VERT* modelData, const unsigned int* indicesData, unsigned int vertCount, unsigned int indexCount, float scalingFactor)
{
	bool result;
	m_vertexCount = vertCount;
	m_indexCount = indexCount;
	m_scale = scalingFactor;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device, deviceContext, modelData, indicesData);
	if (!result)
	{
		return false;
	}
	return true;
}
bool Model::Initialize(ID3D11Device* device, char filename[256], float scalingFactor)
{
	bool result;

	m_scale = scalingFactor;

	result = LoadModel(filename);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Model::LoadModel(char* filename)
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

bool Model::InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const OBJ_VERT *modelData, const unsigned int *indicesData)
{
	//This is me changing the code to use SimpleMesh (A struct with 2 vectors, vertexList and indicesList) to make the code more readable, IMO, and help debug.
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the size of simpleMesh.vertexList.
	// Variables here for index count and vertex count are replaced with the size of the vectors in vertex/indices lists.
	simpleMesh.vertexList.resize(m_vertexCount);

	// Set the size of simpleMesh.indicesList.
	simpleMesh.indicesList.resize(m_indexCount);

	// Load the vertex array with data.
	for (int i = 0; i < m_vertexCount; i++)
	{
		// I'm dividing these positions to make the mesh smaller (and further back from the camera on the z axis), otherwise it'd be clipped and not show up onscreen.
		simpleMesh.vertexList[i].Pos.x = modelData[i].pos[0] / m_scale;
		simpleMesh.vertexList[i].Pos.y = modelData[i].pos[1] / m_scale;
		simpleMesh.vertexList[i].Pos.z = modelData[i].pos[2] / m_scale;

		simpleMesh.vertexList[i].Tex.x = modelData[i].uvw[0];
		simpleMesh.vertexList[i].Tex.y = modelData[i].uvw[1];

		simpleMesh.vertexList[i].Normal.x = modelData[i].nrm[0];
		simpleMesh.vertexList[i].Normal.y = modelData[i].nrm[1];
		simpleMesh.vertexList[i].Normal.z = modelData[i].nrm[2];

	};

	// Load the index array with data.
	for (int i = 0; i < m_indexCount; i++)
	{
		simpleMesh.indicesList[i] = indicesData[i];
	};
	
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = simpleMesh.vertexList.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	//Edited following line to use new vertexbuffer*
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = simpleMesh.indicesList.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	//Edited following line to use new indexbuffer*
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Model::InitializeBuffers(ID3D11Device* device)
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
		vertices[i].Pos.x = m_model[i].Pos.x / m_scale;
		vertices[i].Pos.y = m_model[i].Pos.y / m_scale;
		vertices[i].Pos.z = m_model[i].Pos.z / m_scale;

		vertices[i].Normal = XMFLOAT3(m_model[i].Normal.x, m_model[i].Normal.y, m_model[i].Normal.z);
		vertices[i].Tex = XMFLOAT2(m_model[i].Tex.x, m_model[i].Tex.y);

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

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void Model::Render(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* view, ID3D11ShaderResourceView* SRV = nullptr, ID3D11SamplerState* sampler = nullptr)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	if (pixelShader)
	{
		RenderBuffers(deviceContext, vertexShader, pixelShader, inputLayout, view, SRV, sampler);
	}
	return;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* view, ID3D11ShaderResourceView* SRV, ID3D11SamplerState* sampler)
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

	
	if (SRV != nullptr)
	{
		//This contains the texture being loaded in.
		deviceContext->PSSetShaderResources(0, 1, &SRV);
	}
	if (sampler)
	{
		//The sampler reads from the SRV to get the texture data.
		deviceContext->PSSetSamplers(0, 1, &sampler);
	}
	deviceContext->DrawIndexed(m_indexCount, 0, 0);
}

void Model::RenderMultiple(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* view, ID3D11ShaderResourceView* SRV, ID3D11SamplerState* sampler)
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


	if (SRV != nullptr)
	{
		//This contains the texture being loaded in.
		deviceContext->PSSetShaderResources(0, 1, &SRV);
	}
	if (sampler)
	{
		//The sampler reads from the SRV to get the texture data.
		deviceContext->PSSetSamplers(0, 1, &sampler);
	}
	deviceContext->DrawIndexedInstanced(m_indexCount,3,0,0,0);
}