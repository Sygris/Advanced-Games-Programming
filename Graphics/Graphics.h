#pragma once
#include <d3d11.h>
#include "Shaders.h"
#include "Vertex.h"
#include "ConstantBufferTypes.h"

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

	// Shaders
	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	// Buffers
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indicesBuffer;
	ID3D11Buffer* m_constantBuffer;

	// Depth and Stencil
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	
	// Rasterizer
	ID3D11RasterizerState* m_rasterizerState;

	// Sampler State
	ID3D11SamplerState* m_samplerState;
	ID3D11ShaderResourceView* m_texture;
};