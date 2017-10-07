//---------------------------------------------------------------------------------
// File: D3DClass.cpp
//
// This Source is for Initialize DirectX Interface(COM object) or Class
//-------------------------------------------------------------------------------------
#include "D3DClass.h"
#include "WinMain.h"

D3DClass::D3DClass()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
	m_depthStencilBuffer = 0;
}

D3DClass::~D3DClass()
{
	Release();
}

bool D3DClass::InitD3D(HWND hwnd, int width, int height, float screenNear, float screenFar, bool vsync, bool windowMode)
{
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, numerator, denominator;
	DXGI_MODE_DESC *displayModeList;
	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* pBackBuffer = NULL;
	D3D11_VIEWPORT m_viewPort;
	D3D11_RASTERIZER_DESC rasterizeDesc;
	float screenView, screenAspect;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	//=================================================================
	// Get Graphic Device Information.
	//=================================================================

	// Create GI(Graphic Interface) Factory.
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr))
		return false;

	// Create GI adapter.
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
		return false;

	// Enumerate First Adapter about Output.
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
		return false;

	// Get Mode number in correct on Display Format.
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
											&numModes, NULL);
	if (FAILED(hr))
		return false;

	// Create array for save modelist.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;

	// Insert list Structure.
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList);
	if (FAILED(hr))
		return false;

	// find display mode in correct screen.
	for(unsigned int i = 0; i < numModes; i++)
		if (displayModeList[i].Width == (unsigned int)GetSystemMetrics(SM_CXSCREEN) &&
			displayModeList[i].Height == (unsigned int)GetSystemMetrics(SM_CYSCREEN))
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}

	// Release Interface.
	delete[] displayModeList;
	adapterOutput->Release();
	adapter->Release();
	factory->Release();

	//=================================================================
	// Create SwapChain Description.
	//=================================================================
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = windowMode;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	//===================================================================
	// Create Device & SwapChain.
	// loop before find correct drivertype.
	//=================================================================
	for (UINT i = 0; i < numDriverTypes; i++)
	{
		D3D_DRIVER_TYPE t_driverType = driverTypes[i];

		hr = D3D11CreateDeviceAndSwapChain(
			NULL,			   // Adapter (Graphic Card)
			t_driverType,      // Driver Type
			NULL,			   // Driver Type Software. Usually set with NULL.
			0,				   // Device Flag 
			featureLevels,     // FeatureLevel Array
			numFeatureLevels,  // FeatureLevel Count
			D3D11_SDK_VERSION, // SDK Version
			&swapChainDesc,    // Swapchain Description Struct
			&m_swapChain,      // Created Swapchain Object
			&m_device,         // Created Device Object
			&featureLevel,     // Created FeatureLevel Information
			&m_deviceContext   // Create DeviceContext Object
		);

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed Create Device and Swapchain.", L"Opps!", MB_OK);
		return false;
	}

	// Get BackBuffer Pointer
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed Get Swapchain Buffer.", L"Opps!", MB_OK);
		return false;
	}

	hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTargetView);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed Create Render Target View.", L"Opps!", MB_OK);
		return false;
	}

	pBackBuffer->Release();
	pBackBuffer = 0;

	//=================================================================
	// Set Depth-Stencil.
	//=================================================================

	// Create Depth-Stencil Buffer(2D Texture) Description.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create DepthBuffer.
	hr = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed Create DepthBuffer.", L"Opps!", MB_OK);
		return false;
	}

	// Create Depth-Stencil Description.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF; // 1Byte. 0xFF == 11111111
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Front Facing.
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	// Back Facing.
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	// Create Depth-Stencil State.
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed Create Depth-Stencil State.", L"Opps!", MB_OK);
		return false;
	}


	// set Depth-Stencil State.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Crerate Depth-Stencil Buffer View.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create Depth-Stencil View.
	hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed Create DepthStencilView.", L"Opps!", MB_OK);
		return false;
	}

	// Binding RenderTargetView & Depth-Stencil View on the Output Pipe-Line.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	//=================================================================
	// Create Rasterize Description.
	//=================================================================
	rasterizeDesc.AntialiasedLineEnable = false;
	rasterizeDesc.CullMode = D3D11_CULL_BACK;
	rasterizeDesc.DepthBias = 0;
	rasterizeDesc.DepthBiasClamp = 0.0f;
	rasterizeDesc.DepthClipEnable = true;
	rasterizeDesc.FillMode = D3D11_FILL_SOLID;
	rasterizeDesc.FrontCounterClockwise = false;
	rasterizeDesc.ScissorEnable = false;
	rasterizeDesc.SlopeScaledDepthBias = 0.0f;

	// Create Rasterize State.
	hr = m_device->CreateRasterizerState(&rasterizeDesc, &m_rasterState);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed Create RasterizeState.", L"Opps!", MB_OK);
		return false;
	}

	m_deviceContext->RSSetState(m_rasterState);
	
	//=================================================================
	// Create ViewPort ( Set ViewPort be equal to Window Size. )
	//=================================================================
	m_viewPort.Width = (FLOAT)width;
	m_viewPort.Height = (FLOAT)height;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;

	m_deviceContext->RSSetViewports(1, &m_viewPort);	

	//=================================================================
	// Create Projection Matrix.
	// It is Transform 3D screen to 2D.
	//=================================================================

	screenView = (float)XM_PI / 4.0f;
	screenAspect = (float)width / (float)height;

	// Create Projection Matrix. It is used in 3D Rendering.
	m_projMatrix = XMMatrixPerspectiveFovLH(screenView, screenAspect, screenNear, screenFar);

	// It is used in transform rotation, move, size.
	m_worldMatrix = XMMatrixIdentity();

	// It is used in 2D Rendering. (UI, Font, Image)
	m_orthoMatrix = XMMatrixOrthographicLH((float)width, (float)height, screenNear, screenFar);

	return true;
}

void D3DClass::Release()
{
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
		m_swapChain->Release();
	}
	if (m_device) m_device->Release();
	if (m_deviceContext) m_deviceContext->Release();
	if (m_renderTargetView) m_renderTargetView->Release();
	if (m_depthStencilState) m_depthStencilState->Release();
	if (m_depthStencilView) m_depthStencilView->Release();
	if (m_rasterState) m_rasterState->Release();
	if (m_depthStencilBuffer) m_depthStencilBuffer->Release();
	
	return;
}

ID3D11Device* D3DClass::GetDevice()
{
	return this->m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return this->m_deviceContext;
}

void D3DClass::ClearScene()
{
	float color[4] = { 0.125f, 0.1f, 0.1f, 0.0f };

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	m_swapChain->Present(0, 0);
	return;
}

XMMATRIX D3DClass::GetProjMatrix()
{
	return this->m_projMatrix;
}

XMMATRIX D3DClass::GetWorldMatrix()
{
	return this->m_worldMatrix;
}

XMMATRIX D3DClass::GetOrthoMatrix()
{
	return this->m_orthoMatrix;
}