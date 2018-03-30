#include "D2D.h"

D2D::D2D()
{
	m_driverType = D3D_DRIVER_TYPE_NULL;
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_featureLevel = D3D_FEATURE_LEVEL_11_0;
}

D2D::~D2D()
{

}

bool D2D::Init(HWND hWnd, int width, int height, bool vsync, bool windowMode, float Near, float Far)
{
	HRESULT hr;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;
	unsigned int numModes, numerator, denominator;
	DXGI_MODE_DESC* displayModeList;

	m_vsync = true;

	///////////////////////////////
	// Set adapter & display mode

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr))
		return false;

	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
		return false;

	hr = adapter->EnumOutputs(0, &output);
	if (FAILED(hr))
		return false;

	hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr))
		return false;

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;

	hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr))
		return false;

	// Find Best Hz.
	for (int i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == (unsigned int)width) {
			if (displayModeList[i].Height == (unsigned int)height) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;

				break;
			}
		}
	}

	factory->Release();
	adapter->Release();
	output->Release();
	delete[] displayModeList;

	////////////////////////////////
	// Driver & FeatureLevel Setup

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	////////////////////
	// Swap Chain

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_vsync) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;

	if (windowMode)
		swapChainDesc.Windowed = TRUE;
	else
		swapChainDesc.Windowed = FALSE;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//////////////////////////////////
	// Find Best Driver Combination.

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			m_driverType,
			NULL,
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&m_swapChain,
			&m_device,
			&m_featureLevel,
			&m_deviceContext
		);

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr)) {
		MessageBox(hWnd, L"Failed to Set up Driver Combination.", L"Error", MB_OK);
		return false;
	}

	////////////////////////////
	// Set RenderTarget View.

	ID3D11Texture2D *backBuffer = NULL;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"Failed to Set Up BackBuffer.", L"Error", MB_OK);
		return false;
	}

	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"Failed to Create Render Target View.", L"Error", MB_OK);
		return false;
	}
	backBuffer->Release();

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);

	///////////////////////////
	// RasterRizer State
	D3D11_RASTERIZER_DESC rsDesc;

	rsDesc.AntialiasedLineEnable = true;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.DepthBias = 0;
	rsDesc.DepthBiasClamp = 0.0f;
	rsDesc.DepthClipEnable = true;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.MultisampleEnable = true;
	rsDesc.ScissorEnable = false;
	rsDesc.SlopeScaledDepthBias = 0.0f;

	hr = m_device->CreateRasterizerState(&rsDesc, &m_rasterState);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"Failed to Create RasterRizer State.", L"Error", MB_OK);
		return false;
	}

	m_deviceContext->RSSetState(m_rasterState);


	//////////////////
	// Set ViewPort
	D3D11_VIEWPORT viewPort;
	viewPort.Height = (FLOAT)height;
	viewPort.Width = (FLOAT)width;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	m_deviceContext->RSSetViewports(1, &viewPort);

	//////////////////
	// Blend State
	D3D11_BLEND_DESC blendStateDesc;

	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;

	hr = m_device->CreateBlendState(&blendStateDesc, &m_blendState);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"Failed to Create Blend State.", L"Error", MB_OK);
		return false;
	}

	m_deviceContext->OMSetBlendState(m_blendState, NULL, 0xffffffff);

	///////////////////////
	// Matrix 

	float fov = (float)XM_PI / 4.0f;
	float Aspect = (float)width / (float)height;

	// Crate Projection Matrix
	m_projMatrix = XMMatrixPerspectiveFovLH(fov, Aspect, Near, Far);

	// Identity Matrix
	m_worldMatrix = XMMatrixIdentity();

	// Orthographic Projection Matrix (2D) 
	m_orthoMatrix = XMMatrixOrthographicLH(width, height, Near, Far);

	return true;
}

void D2D::Start()
{
	float cleanColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, cleanColor);

	return;
}

void D2D::End()
{
	if (m_vsync) // Fixed Refresh rate.
		m_swapChain->Present(1, 0);
       	else         // non - fixed Refresh rate.
		m_swapChain->Present(0, 0);

	return;
}

void D2D::Release()
{
	if (m_swapChain) {
		m_swapChain->SetFullscreenState(false, NULL);
		m_swapChain->Release();
	}
	if (m_device) m_device->Release();
	if (m_deviceContext) m_deviceContext->Release();
	if (m_renderTargetView) m_renderTargetView->Release();
	if (m_rasterState) m_rasterState->Release();
	if (m_depthBuf) m_depthBuf->Release();

	return;
}

ID3D11Device* D2D::GetDevice() {
	return m_device;
}


ID3D11DeviceContext* D2D::GetDeviceContext() {
	return m_deviceContext;
}


XMMATRIX D2D::GetProj() {
	return m_projMatrix;
}


XMMATRIX D2D::GetWorld() {
	return m_worldMatrix;
}


XMMATRIX D2D::GetOrtho() {
	return m_orthoMatrix;
}
