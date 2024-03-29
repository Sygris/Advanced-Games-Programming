#include "Graphics.h"

bool Graphics::Initialise(HWND hWnd, int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	if (!InitialiseDirectX(hWnd))
		return false;

	if (!InitialiseScene())
		return false;

	return true;
}

void Graphics::RenderFrame()
{
	// Clear Frame 
	float bgcolor[] = { 0.0f, 0.0f, 0.0, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), bgcolor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set the Sampler
	m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

	// Set Rasterizer and DepthStencilState to Skybox
	m_deviceContext->OMSetDepthStencilState(m_depthStencilStateSkyBox.Get(), 0);
	m_deviceContext->RSSetState(m_rasterizerStateSkyBox.Get());
	
	// Draw Skybox
	m_skybox.Draw(m_camera->GetViewMatrix() * m_camera->GetProjectionMatrix(), m_ambientLight, m_directioanlLight, nullptr);
	
	// Set Rasterizer and DepthStencilState to default
	m_deviceContext->RSSetState(m_rasterizerState.Get());
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

	// Draw GameObjects and UI
	m_map->Draw(m_camera->GetViewMatrix() * m_camera->GetProjectionMatrix(), m_ambientLight, m_directioanlLight, m_pointLight);
	m_text->RenderText();

	// Swap buffers
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

#pragma region Create Z Buffer
	ID3D11Texture2D* pZBufferTexture;
	hr = m_device->CreateTexture2D(&depthStencilBuffer, NULL, &pZBufferTexture);

	if (FAILED(hr)) return hr;

	// Create the Z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = depthStencilBuffer.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	m_device->CreateDepthStencilView(pZBufferTexture, &dsvDesc, m_depthStencilView.GetAddressOf());
	pZBufferTexture->Release();

	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
#pragma endregion

#pragma region Create Rasterizer State
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = false;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = true;
	rasterizerDesc.AntialiasedLineEnable = true;

	hr = m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Rasterizer State!");
		return false;
	}

	// Sky Box
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerStateSkyBox.GetAddressOf());
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

#pragma region Create Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Depth Stencil State!");
		return false;
	}

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilStateSkyBox.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Depth Stencil State!");
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

	return true;
}

bool Graphics::InitialiseScene()
{
	HRESULT hr;
	
	// Create Constant Buffer(s)
	hr = m_cb_vertexShader.Initialize(m_device.Get(), m_deviceContext.Get());
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create constant buffer!");
		return false;
	}

	// SkyBox
	if (!m_skybox.Initialise(m_device.Get(), m_deviceContext.Get(), "Assets/Models/Cube.obj", "Assets/Textures/skybox02.dds", "SkyBoxVS.cso", "SkyBoxPS.cso", m_cb_vertexShader))
		return false;
	m_skybox.SetScale(100.0f, 100.0f, 100.0f);

	// Map
	m_map = new Map(m_device.Get(), m_deviceContext.Get(), "Assets/Maps/map.txt", m_cb_vertexShader);

	// Camera - Future Player
	m_camera = new Camera();
	m_camera->SetProjectMatrix(90.0f, static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight), 0.1f, 1000.0f);
	m_player = new Player(m_map, m_camera);


	// Lights
	m_ambientLight = new Light(0.1f, 0.1f, 0.1f, 0.1f);
	m_directioanlLight = new DirectionalLight(0.5f, 25.0f, 0.0f, 0.0f, 0.6f, 0.2f, 1.0f);
	m_directioanlLight->SetColour(0.5f, 0.5f, 0.5f, 1.0f);

	m_pointLight = new PointLight();
	m_pointLight->SetColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_pointLight->SetPosition(4.5f, 5.0f, 3.0f);

	// Text
	m_text = new Text2D("Assets/Fonts/font1.png", m_device.Get(), m_deviceContext.Get());

	return true;
}
