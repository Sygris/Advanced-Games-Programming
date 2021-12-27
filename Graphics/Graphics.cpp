#include "Graphics.h"
#include <D3DX11.h>

bool Graphics::Initialise(HWND hWnd, int width, int height)
{
	if (!InitialiseDirectX(hWnd, width, height))
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
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, bgcolor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_deviceContext->IASetInputLayout(m_vertexShader.GetInputLayout());
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->RSSetState(m_rasterizerState);
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
	m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	m_deviceContext->VSSetShader(m_vertexShader.GetShader(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Square
	m_deviceContext->PSSetShaderResources(0, 1, &m_texture);
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_indicesBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->DrawIndexed(6, 0, 0);

	m_swapChain->Present(1, NULL);
}

bool Graphics::InitialiseDirectX(HWND hWnd, int width, int height)
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
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
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
			&swapChainDesc, &m_swapChain, &m_device, &m_featureLevel, &m_deviceContext
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
	ID3D11Texture2D* pBackBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hr))
	{
		OutputDebugString("Failed to get pointer to back buffer!");
		return false;
	}

	hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTargetView);

	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Render Target View!");
		return false;
	}

#pragma region Describe Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilBuffer;
	depthStencilBuffer.Width = width;
	depthStencilBuffer.Height = height;
	depthStencilBuffer.MipLevels = 1;
	depthStencilBuffer.ArraySize = 1;
	depthStencilBuffer.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBuffer.SampleDesc.Count = 1;
	depthStencilBuffer.SampleDesc.Quality = 0;
	depthStencilBuffer.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBuffer.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBuffer.CPUAccessFlags = 0;
	depthStencilBuffer.MiscFlags = 0;

	hr = m_device->CreateTexture2D(&depthStencilBuffer, NULL, &m_depthStencilBuffer);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Depth Stencil Buffer!");
		return false;
	}

	hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, NULL, &m_depthStencilView);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Depth Stencil View!");
		return false;
	}
#pragma endregion

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
#pragma endregion

#pragma region Create and Set Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
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

	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
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

	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Rasterizer State!");
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

	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerState);
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
	// Textured Square
	Vertex v[] =
	{
		Vertex(-0.5f, -0.5f, 1.0f, 0.0f, 1.0f), // Bottom Left  - [0]
		Vertex(-0.5f,  0.5f, 1.0f, 0.0f, 0.0f), // Top Left		- [1]
		Vertex(0.5f,   0.5f, 1.0f, 1.0f, 0.0f), // Top Right	- [2]
		Vertex(0.5f,  -0.5f, 1.0f, 1.0f, 1.0f), // Bottom Right	- [3]
	};

	DWORD indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;

	HRESULT hr = m_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create vertex buffer!");
		return hr;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = indices;

	hr = m_device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indicesBuffer);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create indices buffer!");
		return hr;
	}

	// Load Texture
	hr = D3DX11CreateShaderResourceViewFromFile(m_device, "Assets/Textures/LewisPaella.png", NULL, NULL, &m_texture, NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	// Initialise Constant Buffer
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));

	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.ByteWidth = static_cast<UINT>(sizeof(CB_VertexShader) + (16 - sizeof(CB_VertexShader)));
	cbDesc.StructureByteStride = 0;

	hr = m_device->CreateBuffer(&cbDesc, NULL, &m_constantBuffer);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create constant buffer!");
		return hr;
	}

	return true;
}
