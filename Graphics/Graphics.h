#pragma once
#include <d3d11.h>
//#include <wrl/client.h> // ComPtr
#include "Shaders.h"
#include "Camera.h"
#include "../Entities/GameObject.h"
#include "../UI/text2D.h"
#include "../Map.h"
#include "..\Light\Light.h"
#include "..\Light\DirectionalLight.h"
#include "../Light/PointLight.h"

class Graphics
{
public:
	bool Initialise(HWND hWnd, int width, int height);
	void RenderFrame();

	Camera m_camera;
	GameObject m_sphere;
	GameObject m_skybox;
	Map* m_map;

	Light* m_ambientLight;
	DirectionalLight* m_directioanlLight;
	PointLight* m_pointLight;

	Text2D* m_text = nullptr;

private:
	bool InitialiseDirectX(HWND hWnd);
	bool InitialiseScene();

	// Window Dimensions
	int m_windowWidth = 0;
	int m_windowHeight = 0;

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Buffers
	ConstantBuffer<CB_VS_Model> m_cb_vertexShader;
	ConstantBuffer<CB_PixelShader> m_cb_pixelShader;

	// Depth and Stencil
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilStateSkyBox;

	// Rasterizer
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerStateSkyBox;

	// Blend State
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;

	// Sampler State
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
};