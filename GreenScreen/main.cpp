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
#include "ModelLoading.h"
#include "Structs.cpp"

//Models & Rendering includes
#include "Grid.h"
#include "corvetteobj.h"
#include "planeobj.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

Camera* m_Camera = 0;
Model* m_Model = 0;
Model* planeModel = 0;

Grid* m_Grid = 0;
ModelLoading* m_ModelLoader = 0;
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

Microsoft::WRL::ComPtr<ID3D11Buffer>	dirLightConstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	pointLightConstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	ambLightConstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	spotLightConstantBuffer;


Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> mySRV;
Microsoft::WRL::ComPtr < ID3D11SamplerState> myLinearSampler;

WVP constantBufferData;

Light dirLight;
Light pointLight;
Light ambLight;
Spotlight spotLight;

bool lightSwitch = false;

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
	m_Camera->SetPosition(0.0f, 0.0f, -20.0f);
	//-----------

	// Constant Buffer Creation
	//-----------
	//World/View/Projection matrix constant buffer
	CD3D11_BUFFER_DESC desc = CD3D11_BUFFER_DESC(sizeof(WVP), D3D11_BIND_CONSTANT_BUFFER);
	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = &constantBufferData;

	HRESULT hr = myDevice->CreateBuffer(&desc, &srd, WVPconstantBuffer.GetAddressOf());

	//Directional light constant buffer
	ZeroMemory(&desc, sizeof(desc));
	desc = CD3D11_BUFFER_DESC(sizeof(Light), D3D11_BIND_CONSTANT_BUFFER);
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = &dirLight;
	hr = myDevice->CreateBuffer(&desc, &srd, dirLightConstantBuffer.GetAddressOf());

	//Point light constant buffer
	ZeroMemory(&desc, sizeof(desc));
	desc = CD3D11_BUFFER_DESC(sizeof(Light), D3D11_BIND_CONSTANT_BUFFER);
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = &pointLight;
	hr = myDevice->CreateBuffer(&desc, &srd, pointLightConstantBuffer.GetAddressOf());

	//Ambient light constant buffer
	ZeroMemory(&desc, sizeof(desc));
	desc = CD3D11_BUFFER_DESC(sizeof(Light), D3D11_BIND_CONSTANT_BUFFER);
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = &ambLight;
	hr = myDevice->CreateBuffer(&desc, &srd, ambLightConstantBuffer.GetAddressOf());

	//Spotlight constant buffer
	ZeroMemory(&desc, sizeof(desc));
	desc = CD3D11_BUFFER_DESC(sizeof(Spotlight), D3D11_BIND_CONSTANT_BUFFER);
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = &spotLight;
	hr = myDevice->CreateBuffer(&desc, &srd, spotLightConstantBuffer.GetAddressOf());

	// End of constant buffers

	//Initalize Geometry Renderers here

	//Create Grid
	m_Grid = new Grid;
	if (!m_Grid)
	{
		return false;
	}
	//Initalize Grid
	result = m_Grid->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), 1 , 1 , 10, 10);

	// Create the model object.
	m_Model = new Model;
	if (!m_Model)
	{
		return false;
	}
	m_ModelLoader = new ModelLoading;
	if (!m_ModelLoader)
	{
		return false;
	}

	// Initialize the model object.
	//For now, gotta pass in vertex and index count for each model rendered (.h or hardcoded)
	result = m_Model->Initialize( *myDevice.GetAddressOf(), *myContext.GetAddressOf(), corvetteobj_data , corvetteobj_indicies, 3453, 8112, 40.f);
	result = m_ModelLoader->LoadModelBuffers("../cubeobj.obj", *myDevice.GetAddressOf(), *myContext.GetAddressOf());
	//Create and initialize plane model
	planeModel = new Model;
	result = planeModel->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), planeObj_data, planeObj_indicies, 873, 2256, 1.f);

	//End geometry renderers.

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


	if (m_ModelLoader)
	{
		delete m_ModelLoader;
		m_ModelLoader = nullptr;
	}

	//Release Grid object.
	if (m_Grid)
	{
		delete m_Grid;
		m_Grid = nullptr;
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
	//----------

	//Lighting
	//Direction light setting (direction is fixed, color is set through camera render function because of the R button functionality)
	dirLight.vLightDir = XMFLOAT4(0.3f, -1.f, 0.f, 0.f);

	//Not actually a direction here, but instead a position of the point light.
	pointLight.vLightDir = XMFLOAT4(0.f, -99.f, 0.f, 0.f);
	pointLight.vLightColor = XMFLOAT4(0.f, 0.f, 1.f, 0.2f);

	//AmbLight has no direction or position
	ambLight.vLightColor = XMFLOAT4(1.f, 1.f, 1.f, 0.1f);

	//Spotlight initialization
	Light temp = spotLight.light;
	temp.vLightColor = XMFLOAT4(1.f, 1.f, 0.f, 0.4f);
	temp.vLightDir = XMFLOAT4(0.f, 0.f, 1.f, 0.4f);
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

	// Render Loop here
	while (+win.ProcessWindowEvents())
	{
		// Generate the view matrix based on the camera's position.
		
		m_Camera->Render(viewMatrix, lightSwitch, dirLight, pointLight);

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

			//Update and set constant buffers (This could be done more efficently by setting multiple PSconstantbuffers at once).
			//----------------------------------
			ZeroMemory(&constantBufferData, sizeof(WVP));
			constantBufferData.w = XMMatrixIdentity();
			constantBufferData.v = viewMatrix;
			
			// added by clark
			constantBufferData.v = XMMatrixInverse(NULL, viewMatrix);
			constantBufferData.p = projectionMatrix;			
		
			// Update the constant buffer data here per draw / model
			con->UpdateSubresource(WVPconstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
			con->VSSetConstantBuffers(0, 1, WVPconstantBuffer.GetAddressOf());

			//Light constant buffer
			if (lightSwitch)
			{
				//This will get caught in pixel shader and ignored
				dirLight.vLightColor = XMFLOAT4(0.f, 0.f, 0.f, 0.0f);
			}
			else
			{
				dirLight.vLightColor = XMFLOAT4(1.f, 1.f, 1.f, 0.2f);
			}
			//Update dirLight buffer to use updated light color.
			con->UpdateSubresource(dirLightConstantBuffer.Get(), 0, nullptr, &dirLight, 0, 0);
			con->PSSetConstantBuffers(0, 1, dirLightConstantBuffer.GetAddressOf());

			//Update pointLight buffer light color. Currently unused, as pointlight does not change.
			con->UpdateSubresource(pointLightConstantBuffer.Get(), 0, nullptr, &pointLight, 0, 0);
			con->PSSetConstantBuffers(1, 1, pointLightConstantBuffer.GetAddressOf());

			//Update ambLight buffer light color. Currently unused, as amblight does not change.
			con->UpdateSubresource(ambLightConstantBuffer.Get(), 0, nullptr, &ambLight, 0, 0);
			con->PSSetConstantBuffers(2, 1, pointLightConstantBuffer.GetAddressOf());

			//
			//con->UpdateSubresource(spotLightConstantBuffer.Get(), 0, nullptr, &spotLight, 0, 0);
			//con->PSSetConstantBuffers(2, 1, spotLightConstantBuffer.GetAddressOf());
			//End constant buffers for model.

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			// To disable texturing, call Model->Render without the SRV or sampler parameters (untested).
			m_Model->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, mySRV.Get(), myLinearSampler.Get());

			//TODO: Update WVP and/or constant buffers for plane object as appropriate.
			planeModel->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, nullptr, myLinearSampler.Get());
			

			//Update and set constant buffers for grid
			//----------------------------------------
			ZeroMemory(&constantBufferData, sizeof(WVP));
			constantBufferData.w = XMMatrixIdentity();
			constantBufferData.v = viewMatrix;

			// added by clark
			constantBufferData.v = XMMatrixInverse(NULL, viewMatrix);
			constantBufferData.p = projectionMatrix;

			// change the constant buffer data here per draw / model
			con->UpdateSubresource(WVPconstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
			con->VSSetConstantBuffers(0, 1, WVPconstantBuffer.GetAddressOf());
			
			//End grid constant buffers

			m_Grid->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, mySRV.Get(), myLinearSampler.Get());

			//Update and set constant buffers for grid
			//----------------------------------------
			ZeroMemory(&constantBufferData, sizeof(WVP));
			constantBufferData.w = XMMatrixIdentity();
			constantBufferData.v = viewMatrix;

			// added by clark
			constantBufferData.v = XMMatrixInverse(NULL, viewMatrix);
			constantBufferData.p = projectionMatrix;

			// change the constant buffer data here per draw / model
			con->UpdateSubresource(WVPconstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
			con->VSSetConstantBuffers(0, 1, WVPconstantBuffer.GetAddressOf());

			//m_ModelLoader->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, mySRV.Get(), myLinearSampler.Get());

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
