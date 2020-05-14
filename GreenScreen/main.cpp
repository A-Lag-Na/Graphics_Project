#pragma once
// Simple basecode showing how to create a window and attatch a d3d11surface
#define GATEWARE_ENABLE_CORE // All libraries need this
#define GATEWARE_ENABLE_SYSTEM // Graphics libs require system level libraries
#define GATEWARE_ENABLE_GRAPHICS // Enables all Graphics Libraries
// Ignore some GRAPHICS libraries we aren't going to use
#define GATEWARE_DISABLE_GDIRECTX12SURFACE // we have another template for this
#define GATEWARE_DISABLE_GRASTERSURFACE // we have another template for this
#define GATEWARE_DISABLE_GOPENGLSURFACE // we have another template for this
#define GATEWARE_DISABLE_GVULKANSURFACE // we have another template for this
// With what we want & what we don't defined we can include the API
#include "../Gateware/Gateware.h"
#include "triangle.h" // example rendering code (not Gateware code!)
// open some namespaces to compact the code a bit
using namespace GW;
using namespace CORE;
using namespace SYSTEM;
using namespace GRAPHICS;

//Added includes
#include "Camera.h"
#include "Model.h"
#include "DDSTextureLoader.h"
#include "FBXLoader.h"
#include "Structs.cpp"

#include "../GreenScreen/LightManager.h"

//Models & Rendering includes
#include "Grid.h"
#include "corvetteobj.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

Camera* m_Camera = 0;
Model* m_Model = 0;
Grid* m_Grid = 0;
GWindow win;
GEventReceiver msgs;
GDirectX11Surface d3d11;

//Added components for rendering in this .cpp
//---------------------------------------------
Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapChain;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> myContext;

/*Generic names like vertexBuffer are placeholders, 
when we are rendering multiple objects we will have multiple vertex
buffers, and will need more specific names for them.*/
Microsoft::WRL::ComPtr<ID3D11InputLayout>	vertexFormat;
Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;

Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	indexBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	WVPconstantBuffer;

WVP constantBufferData;
ID3D11ShaderResourceView* mySRV = nullptr;
//ID3D11ShaderResourceView* skyboxSRV = nullptr;
ID3D11SamplerState* myLinearSampler = nullptr;

//---------------------------------------------

float clr[] = { 57 / 255.0f, 1.0f, 20 / 255.0f, 1 }; // start with a neon green

bool Initialize(int, int);
void Shutdown();
bool Frame();
bool Render();

// lets pop a window and use D3D11 to clear to a green screen
int main()
{
	if (+win.Create(0, 0, 800, 600, GWindowStyle::WINDOWEDBORDERED))
	{
		msgs.Create(win, [&]() 
		{
			if (+msgs.Find(GWindow::Events::RESIZE, true))
				clr[2] += 0.01f; // move towards a cyan as they resize
			
		});

		if (+d3d11.Create(win, GW::GRAPHICS::DEPTH_STENCIL_SUPPORT | GW::GRAPHICS::DEPTH_BUFFER_SUPPORT))
		{
			
			Frame();
		}
	}
	return 0; // that's all folks
}

bool Initialize(int screenWidth, int screenHeight)
{
	bool result;

	 d3d11.GetDevice((void**)&myDevice);
	 d3d11.GetImmediateContext((void**)&myContext);

	//Camera Code
	//-----------
	// Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->Initialize(screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	//-----------

	// Constant Buffer Creation
	//-----------
	CD3D11_BUFFER_DESC desc = CD3D11_BUFFER_DESC(sizeof(WVP), D3D11_BIND_CONSTANT_BUFFER);
	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = &constantBufferData;

	HRESULT hr = myDevice->CreateBuffer(&desc, &srd, WVPconstantBuffer.GetAddressOf());

	m_Grid = new Grid;
	if (!m_Grid)
	{
		return false;
	}
	//Create Grid
	result = m_Grid->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), 1 , 1 , 10, 10);
	// Create the model object.
	m_Model = new Model;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	//For now, gotta pass in vertex and index count for each model rendered (.h or hardcoded)
	result = m_Model->Initialize( *myDevice.GetAddressOf(), *myContext.GetAddressOf(), corvetteobj_data , corvetteobj_indicies, 3453, 8112, 40.f);

	return true;
}

void Shutdown()
{
	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
}

bool Frame()
{
	bool result;

	Initialize(800, 800);

	//I'm unsure if this code is supposed to go here or at the top of Render().

	//I have to initialize it after d3d11.Create() is called.
	//-------------------------------------------------------------------------
	d3d11.GetSwapchain((void**)&mySwapChain);

	// Create the vertex shader
	HRESULT hr = myDevice->CreateVertexShader(VertexShader, sizeof(VertexShader), nullptr, vertexShader.GetAddressOf());

	// Create the pixel shader
	hr = myDevice->CreatePixelShader(PixelShader, sizeof(PixelShader), nullptr, pixelShader.GetAddressOf());

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	myDevice->CreateInputLayout(layout, numElements, VertexShader, sizeof(VertexShader), &vertexFormat);

	// Set primitive topology
	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//Texturing
	//---------
	// Load corvette Texture

	hr = CreateDDSTextureFromFile(myDevice.Get(), L"../vette_color.dds", nullptr, &mySRV);

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	myDevice->CreateSamplerState(&sampDesc, &myLinearSampler);
	//--------------------------------------------------------------------------

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Render()
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix, tempView;
	bool result;

	// Initialize stuff here

	// Render Loop here
	while (+win.ProcessWindowEvents())
	{
		// Generate the view matrix based on the camera's position.
		
		m_Camera->Render(viewMatrix);

		//Get the view matrix from the camera
		m_Camera->GetViewMatrix(viewMatrix);
		m_Camera->GetWorldMatrix(worldMatrix);
		m_Camera->GetProjectionMatrix(projectionMatrix);

		IDXGISwapChain* swap;
		ID3D11DeviceContext* con;
		ID3D11RenderTargetView* view;
		ID3D11DepthStencilView* dsview;

	
		if (+d3d11.GetImmediateContext((void**)&con) &&
			+d3d11.GetRenderTargetView((void**)&view) &&
			+d3d11.GetSwapchain((void**)&swap))
		{
			
			+d3d11.GetDepthStencilView((void**)&dsview);
			ID3D11RenderTargetView* const views[] = { view };
			con->OMSetRenderTargets(ARRAYSIZE(views), views, dsview);

		
			con->ClearRenderTargetView(view, clr);
			con->ClearDepthStencilView(dsview, D3D11_CLEAR_DEPTH, 1, 0);

			ZeroMemory(&constantBufferData, sizeof(WVP));
			constantBufferData.w = XMMatrixIdentity();
			constantBufferData.v = viewMatrix;
			
			// added by clark
			constantBufferData.v = XMMatrixInverse(NULL, viewMatrix);
			
			constantBufferData.p = projectionMatrix;

		
			// change the constant buffer data here per draw / model
			con->UpdateSubresource(WVPconstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
			con->VSSetConstantBuffers(0, 1, WVPconstantBuffer.GetAddressOf());
			
			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			// To disable texturing, call Model->Render without the SRV or sampler parameters (untested).
			m_Model->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, mySRV, myLinearSampler);
			
			ZeroMemory(&constantBufferData, sizeof(WVP));
			constantBufferData.w = XMMatrixIdentity();
			constantBufferData.v = viewMatrix;

			// added by clark
			constantBufferData.v = XMMatrixInverse(NULL, viewMatrix);

			constantBufferData.p = projectionMatrix;


			// change the constant buffer data here per draw / model
			con->UpdateSubresource(WVPconstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
			con->VSSetConstantBuffers(0, 1, WVPconstantBuffer.GetAddressOf());

			m_Grid->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view);

			swap->Present(1, 0);
			// release incremented COM reference counts
			if(swap)
			swap->Release();
			if(view)
			view->Release();
			if (con) 
				con->Release();			
		}
	}

	// Shutdown stuff here
	Shutdown();

	return true;
}
