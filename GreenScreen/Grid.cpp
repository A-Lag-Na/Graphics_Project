#include "Grid.h"

bool Grid::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float width, float depth,  int m ,  int n)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	 m_vertexCount = (m - 1) * (n - 1) * 8;
	 m_indexCount = m_vertexCount;

	//Create The Vertices
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;
	float dx = width / (n - 1);
	float dz = depth / (m - 1);
	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	meshData.vertexList.resize(m_vertexCount);
	meshData.indicesList.resize(m_indexCount); // 3 indices per face
	
	int index = 0;

	
	for (int i = 0; i < m - 1; ++i)
	{
	
		for (int j = 0; j < n - 1; ++j)
		{
	
			 //LINE 1
			float x = (float)-halfWidth + j * dx;
		    float z = (float)halfDepth - (i + 1)* dz;

			//Position UPLEFT
			meshData.vertexList[index].Pos = XMFLOAT3(x, 0.0f, z);
			//Normal
			meshData.vertexList[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//Texture
			meshData.vertexList[index].Tex.x = j * du;
			meshData.vertexList[index].Tex.y = i * dv;

	
			meshData.indicesList[index] = index;

	
			index++;

	
			x = (float)-halfWidth + (j + 1) * dx;
			z = (float) halfDepth - (i + 1) * dz;

			//Position UPRIGHT
			meshData.vertexList[index].Pos = XMFLOAT3(x, 0.0f, z);
			//Normal
			meshData.vertexList[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//Texture
			meshData.vertexList[index].Tex.x = j * du;
			meshData.vertexList[index].Tex.y = i * dv;

			meshData.indicesList[index] = index;

			index++;

			//LINE 2
			x = (float)-halfWidth + (j + 1) * dx;
			z = (float)halfDepth - (i + 1) * dz;

			//Position UPRIGHT
			meshData.vertexList[index].Pos = XMFLOAT3(x, 0.0f, z);
			//Normal
			meshData.vertexList[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//Texture
			meshData.vertexList[index].Tex.x = j * du;
			meshData.vertexList[index].Tex.y = i * dv;

			meshData.indicesList[index] = index;

			index++;

			x = (float)-halfWidth + (j + 1) * dx;
			z = (float)halfDepth - i * dz;

			//Position BOTRIGHT
			meshData.vertexList[index].Pos = XMFLOAT3(x, 0.0f, z);
			//Normal
			meshData.vertexList[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//Texture
			meshData.vertexList[index].Tex.x = j * du;
			meshData.vertexList[index].Tex.y = i * dv;

			meshData.indicesList[index] = index;

			index++;

			//LINE 3
			x = (float)-halfWidth + (j + 1) * dx;
			z = (float)halfDepth - i * dz;

			//Position BOTRIGHT
			meshData.vertexList[index].Pos = XMFLOAT3(x, 0.0f, z);
			//Normal
			meshData.vertexList[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//Texture
			meshData.vertexList[index].Tex.x = j * du;
			meshData.vertexList[index].Tex.y = i * dv;

			meshData.indicesList[index] = index;

			index++;

			x = (float)-halfWidth + j * dx;
			z = (float)halfDepth - i * dz;

			//Position BOTLEFT
			meshData.vertexList[index].Pos = XMFLOAT3(x, 0.0f, z);
			//Normal
			meshData.vertexList[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//Texture
			meshData.vertexList[index].Tex.x = j * du;
			meshData.vertexList[index].Tex.y = i * dv;

			meshData.indicesList[index] = index;

			index++;

			//LINE 4
			x = (float)-halfWidth + j * dx;;
			z = (float)halfDepth - i * dz;

			//Position BOTLEFT
			meshData.vertexList[index].Pos = XMFLOAT3(x, 0.0f, z);
			//Normal
			meshData.vertexList[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//Texture
			meshData.vertexList[index].Tex.x = j * du;
			meshData.vertexList[index].Tex.y = i * dv;

			meshData.indicesList[index] = index;

			index++;

			x = (float)-halfWidth + j * dx;
			z = (float)halfDepth - (i + 1) * dz;

			//Position UPLEFT
			meshData.vertexList[index].Pos = XMFLOAT3(x, 0.0f, z);
			//Normal
			meshData.vertexList[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//Texture
			meshData.vertexList[index].Tex.x = j * du;
			meshData.vertexList[index].Tex.y = i * dv;

			meshData.indicesList[index] = index;

			index++;
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

void Grid::Render(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* view, ID3D11ShaderResourceView* SRV = nullptr, ID3D11SamplerState* sampler = nullptr)
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
	// Set the type of primitive that should be rendered from this vertex buffer, in this case lines
	
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	deviceContext->PSSetShaderResources(0, 0, nullptr);
	//deviceContext->PSSetSamplers(0, 1, &myLinearSampler);

	if (SRV)
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