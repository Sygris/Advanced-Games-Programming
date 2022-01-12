#include "Graphics.h"
#include <D3DX11.h>

bool Graphics::Initialise(HWND hWnd, int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	if (!InitialiseDirectX(hWnd))
		return false;

	if (!InitialiseShaders())
		return false;

	if (!InitialiseScene())
		return false;

	return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0f, 0.0f, 0.0, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), bgcolor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_deviceContext->IASetInputLayout(m_vertexShader.GetInputLayout());
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->RSSetState(m_rasterizerState.Get());
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
	m_deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
	m_deviceContext->VSSetShader(m_vertexShader.GetShader(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.GetShader(), NULL, 0);

	UINT offset = 0;

	m_model.Draw(m_camera.GetViewMatrix() * m_camera.GetProjectionMatrix());

	m_swapChain->Present(0, NULL);
}

bool Graphics::InitialiseDirectX(HWND hWnd)
{
	// Enables Devices Debugging Capabilities when in Debug 
	UINT createDeviceFlags = 0;
#ifdef _DEBUG 
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = S_OK;

#pragma region Device and Swap Chain creation
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, // Comment this line out if you need to test D3D 11.0 functionality on hardware that doesn't support it
		D3D_DRIVER_TYPE_WARP,	  // Comment this out also to use referene device
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_windowWidth;
	swapChainDesc.BufferDesc.Height = m_windowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			m_driverType,
			NULL,
			NULL,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			m_swapChain.GetAddressOf(), m_device.GetAddressOf(),
			&m_featureLevel, m_deviceContext.GetAddressOf()
		);

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
	{
		OutputDebugString("Failed to create device and swap chain!");
		return false;
	}
#pragma endregion

#pragma region Creation of Render Target View
	// Get pointer to back buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		OutputDebugString("Failed to get pointer to back buffer!");
		return false;
	}

	hr = m_device->CreateRenderTargetView(pBackBuffer.Get(), NULL, m_renderTargetView.GetAddressOf());

	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Render Target View!");
		return false;
	}

	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
#pragma endregion

#pragma region Describe Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilBuffer;
	depthStencilBuffer.Width = m_windowWidth;
	depthStencilBuffer.Height = m_windowHeight;
	depthStencilBuffer.MipLevels = 1;
	depthStencilBuffer.ArraySize = 1;
	depthStencilBuffer.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBuffer.SampleDesc.Count = 1;
	depthStencilBuffer.SampleDesc.Quality = 0;
	depthStencilBuffer.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBuffer.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBuffer.CPUAccessFlags = 0;
	depthStencilBuffer.MiscFlags = 0;

	hr = m_device->CreateTexture2D(&depthStencilBuffer, NULL, m_depthStencilBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Depth Stencil Buffer!");
		return false;
	}

	hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), NULL, m_depthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Depth Stencil View!");
		return false;
	}
#pragma endregion

#pragma region Create and Set Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(m_windowWidth);
	viewport.Height = static_cast<float>(m_windowHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// Set viewport
	m_deviceContext->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region Create Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Depth Stencil State!");
		return false;
	}
#pragma endregion

#pragma region Create Rasterizer State
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;

	hr = m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Rasterizer State!");
		return false;
	}
#pragma endregion 

#pragma region Create Blend State
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
	ZeroMemory(&renderTargetBlendDesc, sizeof(renderTargetBlendDesc));

	renderTargetBlendDesc.BlendEnable = true;
	renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0] = renderTargetBlendDesc;

	hr = m_device->CreateBlendState(&blendDesc, m_blendState.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
#pragma endregion

#pragma region Create Sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_device->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
#pragma endregion

	return true;
}

bool Graphics::InitialiseShaders()
{
#pragma region Determine Shader Path (Change it to a better solution in the future)
	std::wstring shaderFolder = L"";

	// If running from visual studio
	if (IsDebuggerPresent())
	{
#ifdef _DEBUG // Debug Mode
#ifdef _WIN64 //x64
		shaderFolder = L"x64\\Debug\\";
#else //x86 (Win32)
		shaderFolder = L"Debug\\";
#endif
#else // Release Mode
#ifdef _WIN64 //x64
		shaderFolder = L"x64\\Release\\";
#else //x86 (Win32)
		shaderFolder = L"Release\\";
#endif
#endif // DEBUG
	}
#pragma endregion

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numOfLayout = ARRAYSIZE(layout);
	if (!m_vertexShader.Initialise(m_device, shaderFolder + L"VertexShader.cso", layout, numOfLayout))
		return false;

	if (!m_pixelShader.Initialise(m_device, shaderFolder + L"PixelShader.cso"))
		return false;

	return true;
}

bool Graphics::InitialiseScene()
{
	HRESULT hr;

	// Load Texture(s)
	hr = D3DX11CreateShaderResourceViewFromFile(m_device.Get(), "Assets/Textures/pinksquare.jpg", NULL, NULL, &m_pinkTexture, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	hr = D3DX11CreateShaderResourceViewFromFile(m_device.Get(), "Assets/Textures/seamless_grass.jpg", NULL, NULL, &m_grassTexture, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	// Create Constant Buffer(s)
	hr = m_cb_vertexShader.Initialize(m_device.Get(), m_deviceContext.Get());
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create constant buffer!");
		return false;
	}

	hr = m_cb_pixelShader.Initialize(m_device.Get(), m_deviceContext.Get());
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create constant buffer!");
		return false;
	}

	// Initialise Model
	if (!m_model.Initialise(m_device.Get(), m_deviceContext.Get(), m_grassTexture.Get(), m_cb_vertexShader))
		return false;

	if (!m_model.LoadObjModel((char*)"Assets/Models/Sphere.obj"))
		return false;

	if (!m_model.AddTexture((char*)"Assets/Textures/LewisPaella.png"))
		return false;

	m_camera.SetPosition(0.0f, 0.0f, -2.0f);
	m_camera.SetProjectMatrix(90.0f, static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight), 0.1f, 1000.0f);

	return true;
}
