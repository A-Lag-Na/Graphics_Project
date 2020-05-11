#pragma once
#include "Model.h"

Model::Model()
{
	simpleMesh;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_vertexCount = 0;
	m_indexCount = 0;
};

Model::Model(const Model& other)
{
	simpleMesh = other.simpleMesh;
	m_vertexBuffer = other.m_vertexBuffer;
	m_indexBuffer = other.m_indexBuffer;
	m_vertexCount = other.m_vertexCount;
	m_indexCount = other.m_indexCount;
}


Model::~Model()
{
	simpleMesh.vertexList.~vector();
	simpleMesh.indicesList.~vector();
}

bool Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,const OBJ_VERT* modelData, const unsigned int* indicesData)
{
	bool result;


	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device, deviceContext, modelData, indicesData);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::Shutdown()
{
	// Release the vertex and index buffers.
	ShutdownBuffers();

	return;
}

void Model::Render(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* view)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext, vertexShader, pixelShader, inputLayout, view);

	return;
}

int Model::GetIndexCount()
{
	return m_indexCount;
}

bool Model::InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const OBJ_VERT *modelData, const unsigned int *indicesData)
{
	//This is me changing the code to use SimpleMesh (A struct with 2 vectors, vertexList and indicesList) to make the code more readable, IMO, and help debug.
	
	//These two lines are replaced with simpleMesh.vertexList and simpleMesh.indices respectively.
	//VertexType* vertices;
	//unsigned int* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(SimpleVertex);
	offset = 0;


	// Set the size of simpleMesh.vertexList.
	// Variables here for index count and vertex count are replaced with the size of the vectors in vertex/indices lists.
	simpleMesh.vertexList.resize(sizeof(*modelData) / sizeof(modelData[0]));
	//simpleMesh.vertexList.resize(8);

	// Set the size of simpleMesh.indicesList.
	//simpleMesh.indicesList.resize(sizeof(indicesData));
	simpleMesh.indicesList.resize(36);

	//The following 2 blocks of code (creating arrays) are rendered superfluous by using a simpleMesh.
	// Create the vertex array.
	//vertices = new VertexType[m_vertexCount];
	//if (!vertices)
	//{
	//	return false;
	//}

	// Create the index array.
	//indices = new unsigned int[m_indexCount];
	//if (!indices)
	//{
	//	return false;
	//}

	// Load the vertex array with data.
	for (int i = 0; i < simpleMesh.vertexList.size(); i++)
	{
		// I'm dividing these positions to make the mesh smaller, otherwise it'd be clipped and not show up onscreen.
		simpleMesh.vertexList[i].Pos.x = modelData[i].pos[0] ;
		simpleMesh.vertexList[i].Pos.y = modelData[i].pos[1] ;
		simpleMesh.vertexList[i].Pos.z = modelData[i].pos[2] ;

		simpleMesh.vertexList[i].Tex.x = modelData[i].uvw[0];
		simpleMesh.vertexList[i].Tex.y = modelData[i].uvw[1];

		simpleMesh.vertexList[i].Normal.x = modelData[i].nrm[0];
		simpleMesh.vertexList[i].Normal.y = modelData[i].nrm[1];
		simpleMesh.vertexList[i].Normal.z = modelData[i].nrm[2];

	};

	// Load the index array with data.
	for (int i = 0; i < simpleMesh.indicesList.size(); i++)
	{
		simpleMesh.indicesList[i] = indicesData[i];

	};

	
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * simpleMesh.vertexList.size();
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
	indexBufferDesc.ByteWidth = sizeof(int) * simpleMesh.indicesList.size();
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

	//Edited following to use new vertex/index buffer
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

void Model::ShutdownBuffers()
{
	if (m_vertexBuffer) m_vertexBuffer->Release();
	if (m_indexBuffer)  m_indexBuffer->Release();
	// Delete simpleMesh

	return;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* view)
{
	// setup the pipeline
	ID3D11RenderTargetView* const views[] = { view };
	deviceContext->OMSetRenderTargets(ARRAYSIZE(views), views, nullptr);
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	
	deviceContext->DrawIndexed(simpleMesh.indicesList.size(), 0, 0);
}
