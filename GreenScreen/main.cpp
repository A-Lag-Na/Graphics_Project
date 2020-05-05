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
#include "PixelShader.h"
#include "VertexShader.h"


Camera* m_Camera = 0;
GWindow win;
GEventReceiver msgs;
GDirectX11Surface d3d11;

//Added components for rendering in this .cpp

Microsoft::WRL::ComPtr <ID3D11Device> myDevice;
Microsoft::WRL::ComPtr <IDXGISwapChain> mySwapChain;
Microsoft::WRL::ComPtr <ID3D11DeviceContext> myContext;

Microsoft::WRL::ComPtr<ID3D11Buffer>		vertexBuffer;
Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
Microsoft::WRL::ComPtr<ID3D11InputLayout>	vertexFormat;

float clr[] = { 57 / 255.0f, 1.0f, 20 / 255.0f, 1 }; // start with a neon green

bool Initialize(int, int);
void Shutdown();
bool Frame();
bool Render();

// lets pop a window and use D3D11 to clear to a green screen
int main()
{
	//Initialize(800, 800);

	if (+win.Create(0, 0, 800, 600, GWindowStyle::WINDOWEDBORDERED))
	{
		
		msgs.Create(win, [&]() {
			if (+msgs.Find(GWindow::Events::RESIZE, true))
				clr[2] += 0.01f; // move towards a cyan as they resize
			});
		if (+d3d11.Create(win, 0))
		{
			Frame();
		}
	}
	return 0; // that's all folks
}

bool Initialize(int screenWidth, int screenHeight)
{
	bool result;

	//Camera Code
	//-----------
	// Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->Initialize(screenWidth, screenHeight);
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	//-----------

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
}

bool Frame()
{
	bool result;

	d3d11.GetDevice((void**)&myDevice);

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
	XMMATRIX viewMatrix , projectionMatrix, worldMatrix;
	bool result;

	

	// Initialize stuff here
	Triangle tri(win, d3d11);

	
	// Render Loop here
	while (+win.ProcessWindowEvents())
	{


		/* Commented these out cause they wouldn't build due to viewMatrix, projectionMatrix, and worldMatrix being null
		// Generate the view matrix based on the camera's position.
		m_Camera->Render();

		//Get the view matrix from the camera
		m_Camera->GetViewMatrix(viewMatrix);
		m_Camera->GetProjectionMatrix(projectionMatrix);
		m_Camera->GetWorldMatrix(worldMatrix);

		*/

		IDXGISwapChain* swap;
		ID3D11DeviceContext* con;
		ID3D11RenderTargetView* view;
		if (+d3d11.GetImmediateContext((void**)&con) &&
			+d3d11.GetRenderTargetView((void**)&view) &&
			+d3d11.GetSwapchain((void**)&swap))
		{
		
			con->ClearRenderTargetView(view, clr);
			tri.Render();
			swap->Present(1, 0);
			// release incremented COM reference counts
			swap->Release();
			view->Release();
			con->Release();
		}
	}

	// Shutdown stuff here
	Shutdown();

	return true;
}
