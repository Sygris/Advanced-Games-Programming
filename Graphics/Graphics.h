#pragma once
#include <d3d11.h>
#include "Shaders.h"
#include "Vertex.h"

class Graphics
{
public:
	bool Initialise(HWND hWnd, int width, int height);
	void RenderFrame();
private:
	bool InitialiseDirectX(HWND hWnd, int width, int height);
	bool InitialiseShaders();
	bool InitialiseScene();

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_renderTargetView;

	D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	ID3D11Buffer* m_vertexBuffer;
};