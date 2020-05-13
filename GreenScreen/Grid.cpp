#include "Grid.h"

bool Grid::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float width, float depth,  int m ,  int n)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	 m_vertexCount = m * n;
	 m_indexCount = (m - 1) * (n - 1) * 2;

	//Create The Vertices

	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;
	float dx = width / (n - 1);
	float dz = depth / (m - 1);
	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	meshData.vertexList.resize(m_vertexCount);

	for (int i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;

		for (int j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;
			//Position
			meshData.vertexList[(i * n) + j].Pos = XMFLOAT3(x, 0.0f, z);
			//Normal
			meshData.vertexList[(i * n) + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//Texture
			meshData.vertexList[(i * n) + j].Tex.x = j * du;
			meshData.vertexList[(i * n) + j].Tex.y = i * dv;
		}
	}

	meshData.indicesList.resize(m_indexCount * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	int k = 0;

	for (int i = 0; i < m - 1; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
		{
			meshData.indicesList[k] = (i * n) + j;
			meshData.indicesList[k + 1] = ((i * n) + j) + 1;
			meshData.indicesList[k + 2] = ((i + 1) * n) + j;
			meshData.indicesList[k + 3] = ((i + 1) * n) + j;
			meshData.indicesList[k + 4] = ((i * n) + j) + 1;
			meshData.indicesList[k + 5] = (((i + 1) * n) + j) + 1;
			k += 6; // next quad

		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = meshData.vertexList.data();
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
	indexData.pSysMem = meshData.indicesList.data();
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

void Grid::Render(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* view)
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
	//deviceContext->PSSetSamplers(0, 1, &myLinearSampler);

	deviceContext->DrawIndexed(m_indexCount * 3, 0, 0);
}