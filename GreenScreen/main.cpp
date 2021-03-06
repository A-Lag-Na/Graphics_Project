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

using namespace std;

#include <time.h>  
#include <vector>
#include <algorithm>
#include <utility>

//Added includes
#include "Camera.h"
#include "Model.h"
#include "DDSTextureLoader.h"
#include "FBXLoader.h"
#include "ModelLoading.h"
#include "Structs.cpp"

//Models & Rendering includes
#include "Grid.h"
#include "SkySphere.h"
#include "corvetteobj.h"
#include "planeobj.h"
#include "cubeobj.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

Camera* m_Camera = 0;
Camera* topCamera = 0;
Camera* bottomCamera = 0;

Model* m_Model = 0;
Model* m_Cube = 0;
Model* m_Planet01 = 0;
Model* m_Planet02 = 0;
Model* m_Planet03 = 0;
Model* m_Moon = 0;
Model* m_Satellite = 0;
Model* islandModel = 0;
Model* pointCube = 0;
Model* spotCube = 0;
SkySphere* reflectCube = 0;

Grid* m_Grid = 0;
SkySphere* m_SkySphere = 0;
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
Microsoft::WRL::ComPtr<ID3D11VertexShader>	skyVertexShader;
Microsoft::WRL::ComPtr<ID3D11VertexShader>  transparentVertexShader;
Microsoft::WRL::ComPtr<ID3D11VertexShader>  particleVertexShader;
Microsoft::WRL::ComPtr<ID3D11VertexShader>  waveVertexShader;

Microsoft::WRL::ComPtr<ID3D11VertexShader>   variableVertexShader;

Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader>	skyPixelShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader>   transparentPixelShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader>   particlePixelShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader>   reflectivePixelShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader>   wavePixelShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader>   tesselatePixelShader;

Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	indexBuffer;

Microsoft::WRL::ComPtr<ID3D11Buffer>	WVPconstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	cameraConstantBuffer;

Microsoft::WRL::ComPtr<ID3D11Buffer>	dirLightConstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	pointLightConstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	ambLightConstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	spotLightConstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	transparentConstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	timeConstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	colorConstantBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>	renderToTextureConstantBuffer;

Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> corvetteSRV;
Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> placeholderSRV;
Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> sunsetSRV;
Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> Planet01SRV;
Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> Planet02SRV;
Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> Planet03SRV;
Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> Planet04SRV;
Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> MoonSRV;
Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> SatelliteSRV;

Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> renderToTextureSRV;
Microsoft::WRL::ComPtr < ID3D11RenderTargetView> renderToTextureRTV;
Microsoft::WRL::ComPtr < ID3D11Texture2D> renderToTexture2D;

Microsoft::WRL::ComPtr < ID3D11SamplerState> myLinearSampler;

D3D11_VIEWPORT fullView;
D3D11_VIEWPORT topView;
D3D11_VIEWPORT bottomView;

ID3D11BlendState* m_alphaEnableBlendingState = 0;
ID3D11BlendState* m_alphaDisableBlendingState = 0;
ID3D11RasterizerState* m_BackFaceCulling = 0;
ID3D11RasterizerState* m_FrontFaceCulling = 0;

WVP constantBufferData;
XMFLOAT4 camerapos;

Light dirLight;
PointLight pointLight;
Light ambLight;
SpotLight spotLight;
SimpleTransparent m_transparency;

XMFLOAT4 timePassed;

XMVECTOR cube00, cube01, cube02;
vector<pair<UINT, FLOAT>> draw_order;
vector<pair<UINT, XMVECTOR>> draw_positions;

XMFLOAT4 PScolor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

bool lightSwitch = false;
bool splitscreenSwitch = false;

unsigned int widthTemp, heightTemp;

//---------------------------------------------

float clr[] = { 57 / 255.0f, 1.0f, 20 / 255.0f, 1 }; // start with a neon green

bool Initialize(int, int);
void Shutdown();
bool Frame();
bool Render();

FLOAT distance(XMVECTOR first, XMVECTOR second);

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

template <typename T>
HRESULT createConstBuffer(T data, ID3D11Buffer** bufferAddress)
{
	CD3D11_BUFFER_DESC desc = CD3D11_BUFFER_DESC(sizeof(T), D3D11_BIND_CONSTANT_BUFFER);
	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = &data;
	return myDevice->CreateBuffer(&desc, &srd, bufferAddress);
}

void TurnOnAlphaBlending( float x, float y, float z, float w)
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = x;
	blendFactor[1] = y;
	blendFactor[2] = z;
	blendFactor[3] = w;

	// Turn on the alpha blending.
	myContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

void TurnOffAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	myContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void TurnOnFrontFaceCulling()
{
	myContext->RSSetState(m_FrontFaceCulling);
}

void TurnOnBackFaceCulling()
{
	myContext->RSSetState(m_BackFaceCulling);
}

void SetupViewports(D3D11_VIEWPORT& fullView, D3D11_VIEWPORT& topView, D3D11_VIEWPORT& bottomView)
{
	unsigned int widthTemp, heightTemp;
	unsigned int nTopLeftX = 0;
	unsigned int nTopLeftY = 0;
	win.GetClientWidth(widthTemp);
	win.GetClientHeight(heightTemp);
	win.GetClientTopLeft(nTopLeftX, nTopLeftY);

	//Setup fullView
	fullView.Width = static_cast<float>(widthTemp);
	fullView.Height = static_cast<float>(heightTemp);
	fullView.MinDepth = 0.0f;
	fullView.MaxDepth = 1.0f;
	fullView.TopLeftX = static_cast<float>(nTopLeftX);
	fullView.TopLeftY = static_cast<float>(nTopLeftY);

	//Setup topView
	topView.Width = static_cast<float>(widthTemp);
	topView.Height = static_cast<float>(heightTemp) / 2.0f;
	topView.MinDepth = 0.0f;
	topView.MaxDepth = 1.0f;
	topView.TopLeftX = static_cast<float>(nTopLeftX);
	topView.TopLeftY = static_cast<float>(nTopLeftY);

	//Setup bottomView
	bottomView.Width = static_cast<float>(widthTemp);
	bottomView.Height = static_cast<float>(heightTemp) / 2.0f;
	bottomView.MinDepth = 0.0f;
	bottomView.MaxDepth = 1.0f;
	bottomView.TopLeftX = static_cast<float>(nTopLeftX);
	bottomView.TopLeftY = static_cast<float>(nTopLeftY) + (static_cast<float>(heightTemp) / 2.0f);
	return;
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
	topCamera = new Camera;
	bottomCamera = new Camera;
	//-----------

	// Constant Buffer Creation
	//-----------
	//World/View/Projection matrix constant buffer
	HRESULT hr;
	hr = createConstBuffer<WVP>(constantBufferData, WVPconstantBuffer.GetAddressOf());

	//Directional light constant buffer
	hr = createConstBuffer<Light>(dirLight, dirLightConstantBuffer.GetAddressOf());

	//Point light constant buffer
	hr = createConstBuffer<PointLight>(pointLight, pointLightConstantBuffer.GetAddressOf());

	//Ambient light constant buffer
	hr = createConstBuffer<Light>(ambLight, ambLightConstantBuffer.GetAddressOf());

	//Spotlight constant buffer
	hr = createConstBuffer<SpotLight>(spotLight, spotLightConstantBuffer.GetAddressOf());

	//Cameraposition constant buffer
	hr = createConstBuffer<XMFLOAT4>(camerapos, cameraConstantBuffer.GetAddressOf());	

	//Transparent  Constant Buffer
	hr = createConstBuffer<SimpleTransparent>(m_transparency, transparentConstantBuffer.GetAddressOf());

	//Time Constant Buffer
	hr = createConstBuffer<XMFLOAT4>(timePassed, timeConstantBuffer.GetAddressOf());

	//Color Constant Buffer
	hr = createConstBuffer<XMFLOAT4>(PScolor, colorConstantBuffer.GetAddressOf());

	// End of constant buffers
	// Create an alpha enabled blend state description.
	D3D11_BLEND_DESC blendStateDescription;

	blendStateDescription.AlphaToCoverageEnable = FALSE;
	blendStateDescription.IndependentBlendEnable = FALSE;

	for (size_t i = 0; i < 8; i++)
	{
		
		blendStateDescription.RenderTarget[i].BlendEnable = TRUE;
		blendStateDescription.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDescription.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[i].RenderTargetWriteMask = 0x0f;
	}

	hr = myDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	
	for (size_t i = 0; i < 8; i++)
	{
		blendStateDescription.RenderTarget[i].BlendEnable = FALSE;
	}

	hr = myDevice->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);

	D3D11_RASTERIZER_DESC rasterizerDescription;

	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.CullMode = D3D11_CULL_FRONT;
	rasterizerDescription.FrontCounterClockwise = FALSE;
	rasterizerDescription.DepthBias = 0;
	rasterizerDescription.DepthBiasClamp = 0.0f;
	rasterizerDescription.SlopeScaledDepthBias = 0.0f;
	rasterizerDescription.DepthClipEnable = TRUE;
	rasterizerDescription.ScissorEnable = FALSE;
	rasterizerDescription.MultisampleEnable = FALSE;
	rasterizerDescription.AntialiasedLineEnable = FALSE;

	hr = myDevice->CreateRasterizerState(&rasterizerDescription, &m_FrontFaceCulling);

	rasterizerDescription.CullMode = D3D11_CULL_BACK;

	hr = myDevice->CreateRasterizerState(&rasterizerDescription, &m_BackFaceCulling);
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

	m_Cube = new Model;
	if (!m_Cube)
	{
		return false;
	}

	m_Planet01 = new Model;
	if (!m_Planet01)
	{
		return false;
	}
	m_Planet02 = new Model;
	if (!m_Planet01)
	{
		return false;
	}
	m_Planet03 = new Model;
	if (!m_Planet03)
	{
		return false;
	}

	m_Moon = new Model;
	if (!m_Moon)
	{
		return false;
	}

	m_Satellite = new Model;
	if (!m_Satellite)
	{
		return false;
	}
	// Initialize the model object.
	//For now, gotta pass in vertex and index count for each model rendered (.h or hardcoded)
	//result = m_Model->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), corvetteobj_data, corvetteobj_indicies, 3453, 8112, 40.f);

	m_ModelLoader = new ModelLoading;
	if (!m_ModelLoader)
	{
		return false;
	}


	//Skysphere stuff
	//only run ModelLoader if you need new one
	//result = m_ModelLoader->LoadModel("../corvetteobj.obj");

	m_SkySphere = new SkySphere;
	if (!m_SkySphere)
	{
		return false;
	}

	result = m_SkySphere->Initialize(*myDevice.GetAddressOf(), "../skyCubeModel.txt");
	//result = m_Model->Initialize( *myDevice.GetAddressOf(), *myContext.GetAddressOf(), corvetteobj_data , corvetteobj_indicies, 3453, 8112, 40.f);

	// Initialize the model object.
	//For now, gotta pass in vertex and index count for each model rendered (.h or hardcoded)
	result = m_Model->Initialize( *myDevice.GetAddressOf(),"../corvetteModel.txt", 40.0f);
	
	result = m_Cube->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), cubeobj_data, cubeobj_indicies, 788, 1692, 100.f);

	result = m_Planet01->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), Planet_data, Planet_indicies, 1681, 9360, 2500.f);
	result = m_Planet02->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), Planet_data, Planet_indicies, 1681, 9360, 4500.f);
	result = m_Planet03->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), Planet_data, Planet_indicies, 1681, 9360, 6500.f);
	result = m_Moon->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), Planet_data, Planet_indicies, 1681, 9360, 9000.f);

	result = m_Satellite->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), satellite_data, satellite_indicies, 1640, 2544, 10.f);

	//Create and initialize island model
	islandModel = new Model;
	result = islandModel->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), islandmodel_data, islandmodel_indicies, 29546, 100860, 5000.f);

	//Cube for showing the position of pointlight.
	pointCube = new Model;
	result = pointCube->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), cubeobj_data, cubeobj_indicies, 788, 1692, 1000.f);

	//Cube for showing the position of pointlight.
	spotCube = new Model;
	result = spotCube->Initialize(*myDevice.GetAddressOf(), *myContext.GetAddressOf(), cubeobj_data, cubeobj_indicies, 788, 1692, 1000.f);

	//Sphere to reflect skybox
	reflectCube = new SkySphere;
	result = reflectCube->Initialize(*myDevice.GetAddressOf(), "../skyModel.txt");

	//End geometry renderers.
	cube00 = XMVectorSet(-.5f, 0.2f, 1.0f, 1.0f);
	cube01 = XMVectorSet(-1.0f, 0.2f, 1.0f, 1.0f);
	cube02 = XMVectorSet(0.4f, 0.2f, 1.0f, 1.0f);

	draw_positions.push_back(make_pair(0, cube00));
	draw_positions.push_back(make_pair(1, cube01));
	draw_positions.push_back(make_pair(2, cube02));

	//Initialize viewport data	
	SetupViewports(fullView, topView, bottomView);

	// Set the initial position of the camera.
	unsigned int widthTemp, heightTemp, halfWidth, halfHeight;
	win.GetClientWidth(widthTemp);
	win.GetClientHeight(heightTemp);
	halfWidth = static_cast<float>(widthTemp) / 2.f;
	halfHeight = static_cast<float>(heightTemp) / 2.f;

	m_Camera->Initialize(static_cast<float>(widthTemp), static_cast<float>(heightTemp), SCREEN_NEAR, SCREEN_DEPTH);
	topCamera->Initialize(widthTemp, halfHeight, SCREEN_NEAR, SCREEN_DEPTH);
	bottomCamera->Initialize(widthTemp, halfHeight, SCREEN_NEAR, SCREEN_DEPTH);
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
	// Release the camera object.
	if (topCamera)
	{
		delete topCamera;
		topCamera = 0;
	}
	// Release the camera object.
	if (bottomCamera)
	{
		delete bottomCamera;
		bottomCamera = 0;
	}
	//Release Grid object.
	if (m_Grid)
	{
		delete m_Grid;
		m_Grid = nullptr;
	}
	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Cube)
	{
		delete m_Cube;
		m_Cube = 0;
	}
	if (m_Planet01)
	{
		delete m_Planet01;
		m_Planet01 = 0;
	}
	if (m_Planet02)
	{
		delete m_Planet02;
		m_Planet02 = 0;
	}
	if (m_Planet03)
	{
		delete m_Planet03;
		m_Planet03 = 0;
	}
	if (m_Moon)
	{
		delete m_Moon;
		m_Moon = 0;
	}
	if (m_Satellite)
	{
		delete m_Satellite;
		m_Satellite = 0;
	}
	if (islandModel)
	{
		delete islandModel;
		islandModel = 0;
	}
	if (pointCube)
	{
		delete  pointCube;
		pointCube = 0;
	}
	if (spotCube)
	{
		delete spotCube;
		spotCube = 0;
	}
	if (reflectCube)
	{
		delete reflectCube;
		reflectCube = 0;
	}

	if (m_ModelLoader)
	{
		delete m_ModelLoader;
		m_ModelLoader = nullptr;
	}

	if (m_SkySphere)
	{
		m_SkySphere->Shutdown();
		delete m_SkySphere;
		m_SkySphere = nullptr;
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
	hr = myDevice->CreatePixelShader(PixelShader, sizeof(PixelShader), nullptr, pixelShader.GetAddressOf());
	
	// Create the pixel shader
	hr = myDevice->CreateVertexShader(SkyboxVertexShader, sizeof(SkyboxVertexShader), nullptr, skyVertexShader.GetAddressOf());
	hr = myDevice->CreatePixelShader(SkyboxPixelShader, sizeof(SkyboxPixelShader), nullptr, skyPixelShader.GetAddressOf());

	hr = myDevice->CreateVertexShader(TransparencyVertexShader, sizeof(TransparencyVertexShader), nullptr, transparentVertexShader.GetAddressOf());
	hr = myDevice->CreatePixelShader(TransparencyPixelShader, sizeof(TransparencyPixelShader), nullptr, transparentPixelShader.GetAddressOf());

	hr = myDevice->CreateVertexShader(ParticleVertexShader, sizeof(ParticleVertexShader), nullptr, particleVertexShader.GetAddressOf());
	hr = myDevice->CreatePixelShader(ParticlePixelShader, sizeof(ParticlePixelShader), nullptr, particlePixelShader.GetAddressOf());

	hr = myDevice->CreatePixelShader(ReflectiveShader, sizeof(ReflectiveShader), nullptr, reflectivePixelShader.GetAddressOf());

	hr = myDevice->CreatePixelShader(WavePixelShader, sizeof(WavePixelShader), nullptr, wavePixelShader.GetAddressOf());

	hr = myDevice->CreatePixelShader(TesselatePixelShader, sizeof(TesselatePixelShader), nullptr, tesselatePixelShader.GetAddressOf());

	hr = myDevice->CreateVertexShader(WaveVertexShader, sizeof(WaveVertexShader), nullptr, waveVertexShader.GetAddressOf());

	variableVertexShader = vertexShader;

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
	hr = CreateDDSTextureFromFile(myDevice.Get(), L"../vette_color.dds", nullptr, &corvetteSRV);
	hr = CreateDDSTextureFromFile(myDevice.Get(), L"../placeholderTexture.dds", nullptr, &placeholderSRV);
	hr = CreateDDSTextureFromFile(myDevice.Get(), L"../Planet1.dds", nullptr, &Planet01SRV);
	hr = CreateDDSTextureFromFile(myDevice.Get(), L"../Planet2.dds", nullptr, &Planet02SRV);
	hr = CreateDDSTextureFromFile(myDevice.Get(), L"../Planet3.dds", nullptr, &Planet03SRV);
	hr = CreateDDSTextureFromFile(myDevice.Get(), L"../Planet04.dds", nullptr, &Planet04SRV);

	hr = CreateDDSTextureFromFile(myDevice.Get(), L"../Moon.dds", nullptr, &MoonSRV);

	hr = CreateDDSTextureFromFile(myDevice.Get(), L"../satellite.dds", nullptr, &SatelliteSRV);

	//sunsetSRV will show up black with default pixel shader.
	hr = CreateDDSTextureFromFile(myDevice.Get(), L"../NightSky.dds", nullptr, &sunsetSRV);


	//CD3D11_TEXTURE2D_DESC renderToTexture2Ddesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, widthTemp, heightTemp, 1, 0, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
	//	, D3D11_USAGE_DEFAULT, D3D11_CPU_ACCESS_WRITE);
	//renderToTexture2Ddesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	//myDevice.Get()->CreateTexture2D(&renderToTexture2Ddesc, nullptr, renderToTexture2D.GetAddressOf());
	//Setup SRV for renderToTexture
	//CD3D11_SHADER_RESOURCE_VIEW_DESC renderToTextureSRVdesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(renderToTextureConstantBuffer.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

	//Setup RTV for renderToTexture
	//CD3D11_RENDER_TARGET_VIEW_DESC renderToTextureRTVdesc = {};

	//void CD3D11_RENDER_TARGET_VIEW_DESC(
	//	ID3D11Texture2D * pTex2D,
	//	D3D11_RTV_DIMENSION viewDimension, = D3D11_RTV_DIMENSION_TEXTURE2D,
	//	DXGI_FORMAT         format, = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB 
	//	UINT                mipSlice,
	//	UINT                firstArraySlice,
	//	UINT                arraySize = 1
	//);
	////renderToTextureRTVdesc.

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
	dirLight.vLightDir = XMFLOAT4(0.3f, -1.f, 0.f, 0.f);
	dirLight.vLightColor = XMFLOAT4(0.f, 0.5f, 0.0f, 0.2f);

	//Not actually a direction here, but instead a position of the point light.
	pointLight.light.vLightDir = XMFLOAT4(0.4f, -0.23f, 0.f, 0.f);
	pointLight.light.vLightColor = XMFLOAT4(0.f, 0.f, 1.f, 1.f);
	pointLight.radius = XMFLOAT4(1.2f, 0.f, 0.f, 0.f);

	//AmbLight has no direction or position
	ambLight.vLightColor = XMFLOAT4(0.f, 0.f, 0.f, 0.00f);

	//Spotlight initialization
	spotLight.light.vLightColor = XMFLOAT4(1.f, 0.2f, 0.f, 0.1f);
	spotLight.light.vLightDir = XMFLOAT4(-0.2f, -0.2f, 0.f, 0.0f);
	spotLight.coneDir = XMFLOAT4(1.f, -1.f, 0.f, 0.f);
	//Inner > outer, narrows as ratio approaches 1.
	spotLight.coneRatio = XMFLOAT4(0.8f, 0.95f, 0.f, 0.f);

	//Transparent initialization
	m_transparency.blendAmount.x = 0.1f;
	//--------------------------------------------------------------------------	
	
	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

void clearWVP(ID3D11DeviceContext* con, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, float xslide = 0, float yslide = 0, float zslide = 0, float xscale = 1, float yscale = 1, float zscale = 1)
{
	ZeroMemory(&constantBufferData, sizeof(WVP));
	constantBufferData.w = XMMatrixSet
	(
		xscale, 0, 0, 0,
		0, yscale, 0, 0,
		0, 0, zscale, 0,
		xslide, yslide, zslide, 1
	);

	// added by clark
	constantBufferData.v = XMMatrixInverse(NULL, viewMatrix);
	constantBufferData.p = projectionMatrix;

	// change the constant buffer data here per draw / model
	con->UpdateSubresource(WVPconstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
	con->VSSetConstantBuffers(0, 1, WVPconstantBuffer.GetAddressOf());
	return;
}

void clearWVP(ID3D11DeviceContext* con, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, XMMATRIX& worldMatrix)
{
	ZeroMemory(&constantBufferData, sizeof(WVP));
	constantBufferData.w = worldMatrix;

	// added by clark
	constantBufferData.v = XMMatrixInverse(NULL, viewMatrix);
	constantBufferData.p = projectionMatrix;

	// change the constant buffer data here per draw / model
	con->UpdateSubresource(WVPconstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
	con->VSSetConstantBuffers(0, 1, WVPconstantBuffer.GetAddressOf());
	return;
}

FLOAT distance(XMVECTOR first, XMVECTOR second)
{
	XMVECTOR result = second - first;
	return sqrtf(
		(XMVectorGetX(result) * XMVectorGetX(result))
		+
		(XMVectorGetY(result) * XMVectorGetY(result))
		+
		(XMVectorGetZ(result) * XMVectorGetZ(result))
	);
}

void DrawEverything(ID3D11DeviceContext* con, ID3D11RenderTargetView* view, ID3D11DepthStencilView* dsview, float angle, float moonAngle, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	for (UINT i = 0; i < draw_positions.size(); i++)
	{
		draw_order.push_back(make_pair(i, distance(viewMatrix.r[3], draw_positions[i].second)));
	}

	auto compare = [](pair<UINT, FLOAT> a, pair<UINT, FLOAT> b)
	{return a.second > b.second; };

	sort(draw_order.begin(), draw_order.end(), compare);

	//for (UINT i = 0; i < draw_order.size(); i++)
	//{
	//	cout << "Index at: " << draw_order[i].first << '\t' << "Distance: " << draw_order[i].second << '\n';
	//}

	//Replace the pixel shader here in this render call with the skysphere shader.
	m_SkySphere->Render(con, *skyVertexShader.GetAddressOf(), *skyPixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, sunsetSRV.Get(), myLinearSampler.Get());

	con->ClearDepthStencilView(dsview, D3D11_CLEAR_DEPTH, 1, 0);

	//Update and set constant buffers (This could be done more efficently by setting multiple PSconstantbuffers at once).
	//----------------------------------
	clearWVP(con, viewMatrix, projectionMatrix);

	//Update dirLight buffer
	con->UpdateSubresource(dirLightConstantBuffer.Get(), 0, nullptr, &dirLight, 0, 0);
	con->PSSetConstantBuffers(0, 1, dirLightConstantBuffer.GetAddressOf());

	//Update pointLight buffer
	con->UpdateSubresource(pointLightConstantBuffer.Get(), 0, nullptr, &pointLight, 0, 0);
	con->PSSetConstantBuffers(1, 1, pointLightConstantBuffer.GetAddressOf());

	//Update ambLight buffer
	con->UpdateSubresource(ambLightConstantBuffer.Get(), 0, nullptr, &ambLight, 0, 0);
	con->PSSetConstantBuffers(2, 1, ambLightConstantBuffer.GetAddressOf());

	//Update Spotlight buffer
	con->UpdateSubresource(spotLightConstantBuffer.Get(), 0, nullptr, &spotLight, 0, 0);
	con->PSSetConstantBuffers(3, 1, spotLightConstantBuffer.GetAddressOf());

	//Update CameraPos buffer
	con->UpdateSubresource(cameraConstantBuffer.Get(), 0, nullptr, &camerapos, 0, 0);
	con->VSSetConstantBuffers(1, 1, cameraConstantBuffer.GetAddressOf());

	con->UpdateSubresource(transparentConstantBuffer.Get(), 0, nullptr, &m_transparency, 0, 0);
	con->PSSetConstantBuffers(6, 1, transparentConstantBuffer.GetAddressOf());

	//Update time buffer for wavePixelShader
	con->UpdateSubresource(timeConstantBuffer.Get(), 0, nullptr, &timePassed, 0, 0);
	con->PSSetConstantBuffers(5, 1, timeConstantBuffer.GetAddressOf());

	//Update time buffer for waveVertexShader
	con->UpdateSubresource(timeConstantBuffer.Get(), 0, nullptr, &timePassed, 0, 0);
	con->VSSetConstantBuffers(2, 1, timeConstantBuffer.GetAddressOf());

	//Update color buffer for flatColorPixelShader
	PScolor = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
	con->UpdateSubresource(colorConstantBuffer.Get(), 0, nullptr, &PScolor, 0, 0);
	con->PSSetConstantBuffers(7, 1, colorConstantBuffer.GetAddressOf());

	//Update time buffer for waveVertexShader
	con->UpdateSubresource(timeConstantBuffer.Get(), 0, nullptr, &timePassed, 0, 0);
	con->VSSetConstantBuffers(2, 1, timeConstantBuffer.GetAddressOf());

	//clearWVP clears and updates WVP buffers. Render renders models.
	//--------------------------------------------------
	XMFLOAT4 temp = pointLight.light.vLightDir;
	clearWVP(con, viewMatrix, projectionMatrix, temp.x, temp.y, temp.z);
	pointCube->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, corvetteSRV.Get(), myLinearSampler.Get());

	temp = spotLight.light.vLightDir;
	clearWVP(con, viewMatrix, projectionMatrix, temp.x, temp.y, temp.z);
	spotCube->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, corvetteSRV.Get(), myLinearSampler.Get());

	clearWVP(con, viewMatrix, projectionMatrix);

	m_Model->Render(con, *vertexShader.GetAddressOf(), *wavePixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, corvetteSRV.Get(), myLinearSampler.Get());

	clearWVP(con, viewMatrix, projectionMatrix, -0.5, 0.2, 0, 0.1f, 0.1f, 0.1f);
	reflectCube->Render(con, *variableVertexShader.GetAddressOf(), *reflectivePixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, sunsetSRV.Get(), myLinearSampler.Get());

	//Rotation stuff
	float orbitRadius = 1.0f;
	XMMATRIX matRot, matTrans, matFinal;

	// START PLANETS
	
	matRot = XMMatrixRotationY(angle + 1.09);
	XMMATRIX tilt = XMMatrixRotationX(45.0f);
	matTrans = XMMatrixTranslation(0.f, .60f, .5f);
	matTrans = matTrans * tilt;
	matFinal = matTrans * matRot;

	clearWVP(con, viewMatrix, projectionMatrix, matFinal);
	m_Satellite->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, SatelliteSRV.Get(), myLinearSampler.Get());

	matRot = XMMatrixRotationY(angle);
	matTrans = XMMatrixTranslation(0.f, .20f, orbitRadius);
	matFinal = matTrans * matRot;


	clearWVP(con, viewMatrix, projectionMatrix, matFinal);
	m_Planet01->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, Planet01SRV.Get(), myLinearSampler.Get());

	matRot = XMMatrixRotationY(angle + 2.094f);
	matTrans = XMMatrixTranslation(0.f, .20f, orbitRadius + 1.5f);
	matFinal = matTrans * matRot;

	clearWVP(con, viewMatrix, projectionMatrix, matFinal);
	m_Planet02->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, Planet02SRV.Get(), myLinearSampler.Get());

	matRot = XMMatrixRotationY(angle + 4.188f);
	matTrans = XMMatrixTranslation(0.f, .20f, orbitRadius + .5f);
	matFinal = matTrans * matRot;
	XMMATRIX earth = matFinal;

	clearWVP(con, viewMatrix, projectionMatrix, matFinal);
	m_Planet03->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, Planet03SRV.Get(), myLinearSampler.Get());

	
	matRot = XMMatrixRotationY(moonAngle);
	matTrans = XMMatrixTranslation(0.f, .15f, .4f);
	matFinal = matTrans * matRot;
	matFinal =  matFinal * earth;

	clearWVP(con, viewMatrix, projectionMatrix, matFinal);
	m_Moon->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, MoonSRV.Get(), myLinearSampler.Get());
	

	matRot = XMMatrixRotationY(angle + 2.617f);
	matTrans = XMMatrixTranslation(0.f, .20f, orbitRadius + 1.0f);
	matFinal = matTrans * matRot;

	clearWVP(con, viewMatrix, projectionMatrix, matFinal);
	m_Planet02->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, Planet04SRV.Get(), myLinearSampler.Get());

	//END PLANETS

	clearWVP(con, viewMatrix, projectionMatrix);
	m_Grid->Render(con, *vertexShader.GetAddressOf(), *pixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, placeholderSRV.Get(), myLinearSampler.Get());

	clearWVP(con, viewMatrix, projectionMatrix, 0.f, -0.3);
	islandModel->Render(con, *vertexShader.GetAddressOf(), *tesselatePixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, placeholderSRV.Get(), myLinearSampler.Get());

	//TRANSPARENCY
	TurnOnAlphaBlending(0.0f, 0.0f, 0.0f, 1.0f);

	for (UINT i = 0; i < draw_order.size(); i++)
	{

		clearWVP(con, viewMatrix, projectionMatrix,
			XMVectorGetX(draw_positions[draw_order[i].first].second),
			XMVectorGetY(draw_positions[draw_order[i].first].second),
			XMVectorGetZ(draw_positions[draw_order[i].first].second));

		TurnOnFrontFaceCulling();
		m_Cube->Render(con, *vertexShader.GetAddressOf(), *transparentPixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, placeholderSRV.Get(), myLinearSampler.Get());


		clearWVP(con, viewMatrix, projectionMatrix,
			XMVectorGetX(draw_positions[draw_order[i].first].second),
			XMVectorGetY(draw_positions[draw_order[i].first].second),
			XMVectorGetZ(draw_positions[draw_order[i].first].second));

		TurnOnBackFaceCulling();
		m_Cube->Render(con, *vertexShader.GetAddressOf(), *transparentPixelShader.GetAddressOf(), *vertexFormat.GetAddressOf(), view, placeholderSRV.Get(), myLinearSampler.Get());

	}
	draw_order.clear();
	TurnOffAlphaBlending();
}
void DrawWithCamera(ID3D11DeviceContext* con, ID3D11RenderTargetView* view, ID3D11DepthStencilView* dsview, float angle, float moonAngle, Camera* camera)
{
	// Generate the view matrix based on the camera's position.
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix, tempView;

	camera->Render(viewMatrix, lightSwitch, dirLight, pointLight, spotLight);

	//Get the view matrix from the camera
	camera->GetViewMatrix(viewMatrix);
	camera->GetWorldMatrix(worldMatrix);
	camera->GetProjectionMatrix(projectionMatrix);

	//Get camera position here
	float cameraX, cameraY, cameraZ, cameraW;
	cameraX = XMVectorGetX(viewMatrix.r[3]);
	cameraY = XMVectorGetY(viewMatrix.r[3]);
	cameraZ = XMVectorGetZ(viewMatrix.r[3]);
	cameraW = XMVectorGetW(viewMatrix.r[3]);
	camerapos = XMFLOAT4(cameraX, cameraY, cameraZ, cameraW);
	//Set SkySphere's world matrix to use camera xyz
	clearWVP(con, viewMatrix, projectionMatrix, cameraX, cameraY, cameraZ);
	DrawEverything(con, view, dsview, angle, moonAngle,viewMatrix, projectionMatrix);
}
void DrawFixed(ID3D11DeviceContext* con, ID3D11RenderTargetView* view, ID3D11DepthStencilView* dsview, float angle, float moonAngle, Camera* camera, XMMATRIX& viewMatrix)
{
	// Generate the view matrix based on the camera's position.
	XMMATRIX projectionMatrix, worldMatrix, tempView;
	camera->Render(viewMatrix, lightSwitch, dirLight, pointLight, spotLight);

	//Get the view matrix from the camera
	camera->GetWorldMatrix(worldMatrix);
	camera->GetProjectionMatrix(projectionMatrix);

	//Get camera position here
	float cameraX, cameraY, cameraZ, cameraW;
	cameraX = XMVectorGetX(viewMatrix.r[3]);
	cameraY = XMVectorGetY(viewMatrix.r[3]);
	cameraZ = XMVectorGetZ(viewMatrix.r[3]);
	cameraW = XMVectorGetW(viewMatrix.r[3]);
	camerapos = XMFLOAT4(cameraX, cameraY, cameraZ, cameraW);
	//Set SkySphere's world matrix to use camera xyz
	clearWVP(con, viewMatrix, projectionMatrix, cameraX, cameraY, cameraZ);
	DrawEverything(con, view, dsview, angle, moonAngle, viewMatrix, projectionMatrix);
}
bool Render()
{
	win.GetClientWidth(widthTemp);
	win.GetClientHeight(heightTemp);
	float angle = 0;
	float moonAngle = 0;
	static float lastTime = (float)clock();

	// Render Loop here
	while (+win.ProcessWindowEvents())
	{
		float currTime = (float)clock();
		float timeDelta = (currTime - lastTime) * 0.0001f;
		timePassed.x = currTime;

		angle += (timeDelta / 2) * XM_PI;

		moonAngle += (timeDelta / .5f) * XM_PI;

		lastTime = currTime;

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

			//Controls and static calls amongst all renders here
			if (GetAsyncKeyState(VK_HOME) & 0x1)
			{
				m_transparency.blendAmount.x += 0.1;

				if (m_transparency.blendAmount.x >= 1)
				{
					m_transparency.blendAmount.x = 1;
				}
			}

			if (GetAsyncKeyState(VK_END) & 0x1)
			{
				m_transparency.blendAmount.x -= 0.1;

				if (m_transparency.blendAmount.x <= 0)
				{
					m_transparency.blendAmount.x = 0;
				}
			}

			if (GetAsyncKeyState(0x4D) & 0x1)
			{
				variableVertexShader = waveVertexShader;
			}
			if (GetAsyncKeyState(0x4E) & 0x1)
			{
				variableVertexShader = vertexShader;
			}

			float cameraX, cameraY, cameraZ, cameraW;
			XMMATRIX viewMatrixTemp;
			m_Camera->GetViewMatrix(viewMatrixTemp);
			cameraX = XMVectorGetX(viewMatrixTemp.r[3]);
			cameraY = XMVectorGetY(viewMatrixTemp.r[3]);
			cameraZ = XMVectorGetZ(viewMatrixTemp.r[3]);
			cameraW = XMVectorGetW(viewMatrixTemp.r[3]);

			// draw entire scene
			if (GetAsyncKeyState(VK_LSHIFT) &0x1)
			{
				splitscreenSwitch = true;
			}
			// draw entire scene
			if (GetAsyncKeyState(VK_LCONTROL) & 0x1)
			{
				splitscreenSwitch = false;
			}
			if (splitscreenSwitch)
			{
				con->RSSetViewports(1, &topView);
				DrawFixed(con, view, dsview, angle, moonAngle,topCamera, viewMatrixTemp);				
				//topCamera->SetPosition(cameraX, cameraY, cameraZ);

				con->RSSetViewports(1, &bottomView);

				// Set the initial position of the camera.
				//bottomCamera->SetPosition(cameraX, cameraY, cameraZ);
				DrawWithCamera(con, view, dsview, angle, moonAngle, bottomCamera);
			}
			else
			{
				con->RSSetViewports(1, &fullView);
				//m_Camera->SetPosition(cameraX, cameraY, cameraZ);
				DrawWithCamera(con, view, dsview, angle, moonAngle, m_Camera);
			}			
			
			//-----------------------------

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

	Shutdown();

	return true;
}