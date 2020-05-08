#pragma once
#include <windows.h>
#include "Camera.h"


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

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialize(int, int);
	void Shutdown();
	bool Frame();
	
	float clr[4] = { 57 / 255.0f, 1.0f, 20 / 255.0f, 1 }; // start with a neon green

	Camera* m_Camera = 0;
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

private:
	bool Render();
	
};

