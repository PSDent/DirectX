//--------------------------------------------------------------------------------------
// File: Tutorial02.cpp
//
// This application displays a triangle using Direct3D 11
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "resource.h"
//
//---------------------------------------------------------------------------------
//define const 
//-------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
    XMFLOAT3 Pos;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;					//������ �ν��Ͻ� 
HWND                    g_hWnd = NULL;					//������ �ڵ� 
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL; //����̹� Ÿ�� ��ü
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0; //��ó����
ID3D11Device*           g_pd3dDevice = NULL; //����̽� ��ü
ID3D11DeviceContext*    g_pImmediateContext = NULL; //����̽� ���ؽ�Ʈ
IDXGISwapChain*         g_pSwapChain = NULL; //����ü�� ��ü
ID3D11RenderTargetView* g_pRenderTargetView = NULL; //����Ÿ�� �� ��ü
ID3D11VertexShader*     g_pVertexShader = NULL; //���� ���̴� ��ü
ID3D11PixelShader*      g_pPixelShader = NULL; //�ȼ� ���̴� ��ü
ID3D11InputLayout*      g_pVertexLayout = NULL; //
ID3D11Buffer*           g_pVertexBuffer = NULL;//���� ��ü
//ID3D11DepthStencilView* g_pDepthStencilView = NULL;

int VertexCount;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow ); //������ �ʱ�ȭ
HRESULT InitDevice(); //����̽� �ʱ�ȭ
void CleanupDevice(); //����̽� Ŭ�� 
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM ); //�����ν���
void Render(); //������ �Լ�


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass",
		L"Direct3D 11 Tutorial 2: Rendering a Triangle",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		rc.right - rc.left, 
		rc.bottom - rc.top, 
		NULL, NULL, 
		hInstance,
        NULL );

    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; //���̴� �÷��� 
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob; //�������̽� ��ü 
    hr = D3DX11CompileFromFile( 
		szFileName, 
		NULL, 
		NULL, 
		szEntryPoint, 
		szShaderModel, 
        dwShaderFlags,
		0, 
		NULL, 
		ppBlobOut, 
		&pErrorBlob, 
		NULL );

    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain ����ü�ΰ� ����̽� ���� 
//--------------------------------------------------------------------------------------
HRESULT InitDevice() //����̽� �ʱ�ȭ 
{
    HRESULT hr = S_OK; //HRESULT: ��� �� 

    RECT rc; //���� ����ü 
    GetClientRect( g_hWnd, &rc ); //�������� ����(���μ���) ������ rc ����ü�� ����.
    UINT width = rc.right - rc.left; //UINT = Unsigned INT 
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] = //����̹� Ÿ�� �迭�� �ʱ�ȭ 
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes ); //�迭�� ����� ���ؼ� ������ ���� 

    D3D_FEATURE_LEVEL featureLevels[] = //��ó ���� �迭 �ʱ�ȭ 
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels ); //�迭�� ����� ���ؼ� ������ ���� 

    DXGI_SWAP_CHAIN_DESC sd; //����ü���� �����ϱ� ���� ������ ���� ����ü 
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;									// ���� ����
    sd.BufferDesc.Width = width;						// ���� ����
    sd.BufferDesc.Height = height;						// ���� ����
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // ���� ����(����)
    sd.BufferDesc.RefreshRate.Numerator = 60;			// �������� ����
    sd.BufferDesc.RefreshRate.Denominator = 1;			//
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // ���� ���ӻ� 
    sd.OutputWindow = g_hWnd;							// ���۰� ��� �� ������ �ڵ�
    sd.SampleDesc.Count = 1;							// AA(Anti-Aliacing)
    sd.SampleDesc.Quality = 0;							// AA(Anti-Aliacing)
    sd.Windowed = TRUE;									// ������ â��� 

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    { //����̺� �迭�� �ִ� ������ ������ Ȯ���Ͽ� ������ �����ϸ� �ݺ� ����.
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

	// ���� Ÿ�� �� ����
	// �ؽ��Ĵ� �������������κ��� ��(View)�� ���� ������ ������. 
	// ����Ÿ�ٵ� �ؽ����� �����̹Ƿ� �並 ���� ���� �ؾ� ��.
	// �̶� ����Ÿ�ٿ� ������ �ϱ� ���� ����Ÿ�� �並 ����Ѵ�.
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

	//������� ����Ÿ�� �並 �����Ѵ�. 
    hr = g_pd3dDevice->CreateRenderTargetView( 
		pBackBuffer,				// �信�� ������ �ϴ� ���ҽ�
		NULL,						// ����Ÿ�� ���� ����
		&g_pRenderTargetView );		// ����Ÿ�� �並 �޾ƿ� ����
    pBackBuffer->Release();			// ����Ÿ�� �䰡 ������ ���Ŀ��� 
    if( FAILED( hr ) )				// ����Ÿ�� ��� ����
        return hr;					// ���� ����Ÿ�� ���� �������̽��� ���� �� ����� ������ ����

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

	// �� ��Ʈ ����.
	// �� ��Ʈ ��? ����Ÿ���� ������ ������ ���� ����.
	// ���� Ÿ���� ���̿� ����, ���̰����� ������ ���� ����. 
	// ������ �� ��Ʈ�� �� ����Ÿ�ٺ��� ������ �ؾ���. ex) 8���� ����Ÿ���� ������ ����Ʈ�� 8��..
    D3D11_VIEWPORT vp; // �� ��Ʈ ��ü ���� 
    vp.Width = (FLOAT)width;		// ����
    vp.Height = (FLOAT)height;		// ����
    vp.MinDepth = 0.0f;				// �ּ� ����
    vp.MaxDepth = 1.0f;				// �ִ� ����
    vp.TopLeftX = 0;				// �»�� X��ǥ
    vp.TopLeftY = 0;				// �»�� Y��ǥ 
    g_pImmediateContext->RSSetViewports( 1, &vp ); //�� ��Ʈ�� RS ���� 
												   //RS: �����Ͷ����� �������� 

    // ���� ���̴� ������ 
    ID3DBlob* pVSBlob = NULL; // ID3DBlob �������̽�, ���� ���̴� ��ü 
    hr = CompileShaderFromFile( L"Tutorial02.fx", "VS", "vs_4_0", &pVSBlob ); //�ش� ���̴��� �������Ͽ� �����Ϳ� ����
    if( FAILED( hr ) )
    {//���� ó�� 
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// ���� ���̴� ���� 
	//���� ���̴��� ����̽��� ���� 
	hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader );
	if( FAILED( hr ) )
	{	//���н� ���̴� ��ü ���� 
		pVSBlob->Release();
        return hr;
	}

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
	UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
	hr = g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &g_pVertexLayout );
	pVSBlob->Release();
	if( FAILED( hr ) )
        return hr;

    // Set the input layout
    g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

	// �ȼ� ���̴� ������ 
	ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( L"Tutorial02.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// �ȼ� ���̴� ���� 
	hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

	UINT indices[15] = { //�ε���(����) ���� 
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4 ,5,
		0, 5, 1
	};

	/*
		0.0f, 0.0f, 0.0f	//0 ����
       -0.5f, 0.3f, 0.0f	//1
        0.0f, 0.5f, 0.0f	//2
		0.5f, 0.3f, 0.0f	//3
	   -0.25f, -0.3f, 0.0f	//4
		0.25f, -0.3f, 0.0f	//5
	*/
	//�ε��� ���۸� �����ϴ� ����ü�� ä��. ���(BIND) �÷��׷�
	//D3D11_BIND_INDEX_BUFFER�� ������. 
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE; 
	ibd.ByteWidth = sizeof(UINT) * (sizeof(indices) / sizeof(indices[0]));
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	//�ε��� ���۸� �ʱ�ȭ�� �ڷḦ ���� 
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	//�ε��� ���۸� ����
	ID3D11Buffer* mIB;
	hr = g_pd3dDevice->CreateBuffer(&ibd, &iinitData, &mIB);
	if (FAILED(hr))
		return hr;

	//�ε��� ���� ���� 
	g_pImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	//���� ���� ����
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // ���� ���� �ʱ�ȭ
    SimpleVertex vertices[] =
    {
        XMFLOAT3( 0.0f, 0.0f, 0.0f), //0 ����
        XMFLOAT3(-0.4f, 0.1, 0.0f), //1
        XMFLOAT3( 0.0f, 0.5f, 0.0f), //2
		XMFLOAT3( 0.4f, 0.1f, 0.0f), //3
		XMFLOAT3(0.2f, -0.4f, 0.0f), //4
		XMFLOAT3(-0.2f, -0.4f, 0.0f), //5
		//XMFLOAT3( 0.9f, 0.1f, 0.0f), //6
    };
	VertexCount = sizeof(vertices) / sizeof(vertices[0]);
    D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * VertexCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer ); //����̽��� ���� ���� 
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //ǥ���� ���
		//D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	//D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    if( g_pImmediateContext ) g_pImmediateContext->ClearState();

    if( g_pVertexBuffer ) g_pVertexBuffer->Release();
    if( g_pVertexLayout ) g_pVertexLayout->Release();
    if( g_pVertexShader ) g_pVertexShader->Release();
    if( g_pPixelShader ) g_pPixelShader->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pImmediateContext ) g_pImmediateContext->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
    // Clear the back buffer 
    float ClearColor[4] = { 0.4f, 0.425f, 0.3f, 1.0f }; // red,green,blue,alpha
    g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );
	
    // Render a triangle
	g_pImmediateContext->VSSetShader( g_pVertexShader, NULL, 0 );
	g_pImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
	g_pImmediateContext->DrawIndexed(15,0,0); //���� ���� | ���� �ε��� | ������ 
   // g_pImmediateContext->Draw(VertexCount, 0 );


    // Present the information rendered to the back buffer to the front buffer (the screen)
    g_pSwapChain->Present( 0, 0 );
}
