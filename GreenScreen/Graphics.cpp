#include "Graphics.h"


Graphics::Graphics()
{
}


Graphics::Graphics(const Graphics& other)
{
}


Graphics::~Graphics()
{
}


bool Graphics::Initialize(int screenWidth, int screenHeight)
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
	//<<<<<< < HEAD
		m_Camera->SetPosition(0.0f, 0.0f, -20.0f);

	//====== =
		m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	//-----------

	return true;
}


void Graphics::Shutdown()
{
	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
}


bool Graphics::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}


bool Graphics::Render()
{

		XMMATRIX viewMatrix, projectionMatrix, worldMatrix;
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
