#include "Graphics.h"

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
	float bgcolor[] = { 0.0f, 0.0f, 1.0, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, bgcolor);

	m_deviceContext->IASetInputLayout(m_vertexShader.GetInputLayout());
	m_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_deviceContext->VSSetShader(m_vertexShader.GetShader(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	m_deviceContext->Draw(1, 0);

	m_swapChain->Present(0, NULL);
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

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
#pragma endregion

#pragma region Create and Set Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	// Set viewport
	m_deviceContext->RSSetViewports(1, &viewport);
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
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_INSTANCE_DATA, 0},
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
	Vertex v[] =
	{
		Vertex(0.0f, 0.0f),
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
		return false;
	}

	return true;
}
