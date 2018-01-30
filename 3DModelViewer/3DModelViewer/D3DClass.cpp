#include "D3DClass.h"

D3DClass::D3DClass()
{
	m_driverType			= D3D_DRIVER_TYPE_NULL;
	m_swapChain				= 0;
	m_device				= 0;
	m_deviceContext			= 0;
	m_renderTargetView		= 0;
	m_featureLevel			= D3D_FEATURE_LEVEL_11_0;
}

D3DClass::~D3DClass()
{

}

bool D3DClass::InitD3D(HWND hWnd, int width, int height, bool vsync, bool windowMode, float Near, float Far)
{
	HRESULT hr;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;
	unsigned int numModes, numerator, denominator;
	DXGI_MODE_DESC* displayModeList;

	m_vsync = vsync;

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
	swapChainDesc.SampleDesc.Count = 1;
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


	////////////////////////////
	// Depth Buffer 

	D3D11_TEXTURE2D_DESC depthBufDesc;

	ZeroMemory(&depthBufDesc, sizeof(depthBufDesc));

	depthBufDesc.Width = width;
	depthBufDesc.Height = height;
	depthBufDesc.MipLevels = 1;
	depthBufDesc.ArraySize = 1;
	depthBufDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufDesc.SampleDesc.Count = 1;
	depthBufDesc.SampleDesc.Quality = 0;
	depthBufDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufDesc.CPUAccessFlags = 0;
	depthBufDesc.MiscFlags = 0;

	hr = m_device->CreateTexture2D(&depthBufDesc, NULL, &m_depthBuf);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"Failed to Create Depth-Stencil Buffer.", L"Error", MB_OK);
		return false;
	}

	///////////////////////////
	// Depth - Stencil State
	
	D3D11_DEPTH_STENCIL_DESC depthDesc;

	ZeroMemory(&depthDesc, sizeof(depthDesc));

	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthDesc.StencilEnable = true;
	depthDesc.StencilReadMask = 0xFF;
	depthDesc.StencilWriteMask = 0xFF;

	// Front Facing
	depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
	depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	// Back Facing
	depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	hr = m_device->CreateDepthStencilState(&depthDesc, &m_depthStencilState);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"Failed to Create Depth-Stencil State.", L"Error", MB_OK);
		return false;
	}

	// Set State 
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	///////////////////////////
	// Depth - Stencil View

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;

	ZeroMemory(&depthViewDesc, sizeof(depthViewDesc));

	depthViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0;

	hr = m_device->CreateDepthStencilView(m_depthBuf, &depthViewDesc, &m_depthStencilView);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"Failed to Create Depth-Stencil View.", L"Error", MB_OK);
		return false;
	}

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	///////////////////////////
	// RasterRizer State
	D3D11_RASTERIZER_DESC rsDesc;

	rsDesc.AntialiasedLineEnable = false;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.DepthBias = 0;
	rsDesc.DepthBiasClamp = 0.0f;
	rsDesc.DepthClipEnable = true;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.MultisampleEnable = false;
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

	///////////////////////
	// Matrix 

	float fov = (float)XM_PI / 4.0f;
	float Aspect = (float)width / (float)height;

	// Crate Projection Matrix
	m_projMatrix = XMMatrixPerspectiveFovLH(fov, Aspect, Near, Far);

	// Identity Matrix
	m_worldMatrix = XMMatrixIdentity();

	// Create Ortho Matrix to use at 2D Rendering.
	m_orthoMatrix = XMMatrixOrthographicLH(width, height, Near, Far);


	return true;
}	

void D3DClass::Start()
{
	float cleanColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, cleanColor);

	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::End()
{
	if (m_vsync) // Fixed Refresh rate.
		m_swapChain->Present(1, 0);
	else         // non - fixed Refresh rate.
		m_swapChain->Present(0, 0);

	return;
}

void D3DClass::Release()
{
	if (m_swapChain) m_swapChain->Release();
	if (m_device) m_device->Release();
	if (m_deviceContext) m_deviceContext->Release();
	if (m_renderTargetView) m_renderTargetView->Release();
	if (m_rasterState) m_rasterState->Release();
	if (m_depthBuf) m_depthBuf->Release();
	if (m_depthStencilState) m_depthStencilState->Release();
	if (m_depthStencilView) m_depthStencilView->Release();

	return;
}

ID3D11Device* D3DClass::GetDevice(){
	return m_device;
}


ID3D11DeviceContext* D3DClass::GetDeviceContext(){
	return m_deviceContext;
}


XMMATRIX D3DClass::GetProj(){
	return m_projMatrix;
}


XMMATRIX D3DClass::GetWorld(){
	return m_worldMatrix;
}


XMMATRIX D3DClass::GetOrtho(){
	return m_orthoMatrix;
}