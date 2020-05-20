#include "ModelLoading.h"



bool ModelLoading::LoadOBJ(char* filename, vector <XMFLOAT3>& out_vertices, vector <XMFLOAT2>& out_uvs, vector <XMFLOAT3>& out_normals)
{
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<XMFLOAT3> temp_vertices, temp_normals;
	vector<XMFLOAT2> temp_uvs;

	FILE* file = fopen(filename, "r");

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.


	while (1)
	{
		char lineHeader[128];

		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.


			// Read in the vertices.
		if (strcmp(lineHeader, "v") == 0)
		{
			XMFLOAT3 temp_vert;

			fscanf(file, "%f %f %f\n", &temp_vert.x, &temp_vert.y, &temp_vert.z);

			// Invert the Z vertex to change to left hand system.
			//temp_vert.z = temp_vert.z * -1.0f;

			temp_vertices.push_back(temp_vert);

		}

		// Read in the texture uv coordinates.
		else if (strcmp(lineHeader, "vt") == 0)
		{
			XMFLOAT2 temp_uv;

			fscanf(file, "%f %f\n", &temp_uv.x, &temp_uv.y);

			// Invert the V texture coordinates to left hand system.
			//temp_uv.y = 1.0f - temp_uv.y;

			temp_uvs.push_back(temp_uv);
		}

		// Read in the normals.
		else if (strcmp(lineHeader, "vn") == 0)
		{
			XMFLOAT3 temp_nrm;

			fscanf(file, "%f %f %f\n", &temp_nrm.x, &temp_nrm.y, &temp_nrm.z);

			// Invert the Z normal to change to left hand system.
			//temp_nrm.z = temp_nrm.z * -1.0f;

			temp_normals.push_back(temp_nrm);
		}



		// Read in the faces.
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		XMFLOAT3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++)
	{
		unsigned int uvIndex = uvIndices[i];
		XMFLOAT2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int normalIndex = normalIndices[i];
		XMFLOAT3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}

	return true;
}


bool ModelLoading::LoadModelBuffers(char* filename,ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Read our .obj file
	std::vector< XMFLOAT3 > vertices;
	std::vector< XMFLOAT2 > uvs;
	std::vector< XMFLOAT3 > normals; // Won't be used at the moment.
	bool res = LoadOBJ(filename, vertices, uvs, normals);

	//This is me changing the code to use SimpleMesh (A struct with 2 vectors, vertexList and indicesList) to make the code more readable, IMO, and help debug.
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the size of simpleMesh.vertexList.
	// Variables here for index count and vertex count are replaced with the size of the vectors in vertex/indices lists.
	m_vertexCount = vertices.size();

	simpleMesh.vertexList.resize(m_vertexCount);


	// Load the vertex array with data.
	for (int i = 0; i < m_vertexCount; i++)
	{
		// I'm dividing these positions to make the mesh smaller (and further back from the camera on the z axis), otherwise it'd be clipped and not show up onscreen.
		simpleMesh.vertexList[i].Pos.x = vertices[i].x/ m_scale;
		simpleMesh.vertexList[i].Pos.y = vertices[i].y / m_scale;
		simpleMesh.vertexList[i].Pos.z = vertices[i].z / m_scale;// +1;

		simpleMesh.vertexList[i].Tex.x = uvs[i].x;
		simpleMesh.vertexList[i].Tex.y = uvs[i].y;

		simpleMesh.vertexList[i].Normal.x = normals[i].x;
		simpleMesh.vertexList[i].Normal.y = normals[i].y;
		simpleMesh.vertexList[i].Normal.z = normals[i].z;

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


	return true;
}

void ModelLoading::Render(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* view, ID3D11ShaderResourceView* SRV = nullptr, ID3D11SamplerState* sampler = nullptr)
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
	deviceContext->Draw(m_vertexCount, 0);
}

