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
HINSTANCE               g_hInst = NULL;					//윈도우 인스턴스 
HWND                    g_hWnd = NULL;					//윈도우 핸들 
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL; //드라이버 타입 객체
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0; //피처레벨
ID3D11Device*           g_pd3dDevice = NULL; //디바이스 객체
ID3D11DeviceContext*    g_pImmediateContext = NULL; //디바이스 컨텍스트
IDXGISwapChain*         g_pSwapChain = NULL; //스왑체인 객체
ID3D11RenderTargetView* g_pRenderTargetView = NULL; //렌더타겟 뷰 객체
ID3D11VertexShader*     g_pVertexShader = NULL; //정점 쉐이더 객체
ID3D11PixelShader*      g_pPixelShader = NULL; //픽셀 쉐이더 객체
ID3D11InputLayout*      g_pVertexLayout = NULL; //
ID3D11Buffer*           g_pVertexBuffer = NULL;//버퍼 객체
//ID3D11DepthStencilView* g_pDepthStencilView = NULL;

int VertexCount;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow ); //윈도우 초기화
HRESULT InitDevice(); //디바이스 초기화
void CleanupDevice(); //디바이스 클린 
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM ); //윈프로시저
void Render(); //렌더링 함수


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

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; //쉐이더 플래그 
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob; //인터페이스 객체 
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
// Create Direct3D device and swap chain 스왑체인과 디바이스 생성 
//--------------------------------------------------------------------------------------
HRESULT InitDevice() //디바이스 초기화 
{
    HRESULT hr = S_OK; //HRESULT: 결과 값 

    RECT rc; //영역 구조체 
    GetClientRect( g_hWnd, &rc ); //윈도우의 영역(가로세로) 정보를 rc 구조체에 담음.
    UINT width = rc.right - rc.left; //UINT = Unsigned INT 
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] = //드라이버 타입 배열에 초기화 
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes ); //배열의 사이즈를 구해서 변수에 대입 

    D3D_FEATURE_LEVEL featureLevels[] = //피처 레벨 배열 초기화 
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels ); //배열의 사이즈를 구해서 변수에 대입 

    DXGI_SWAP_CHAIN_DESC sd; //스왑체인을 생성하기 위한 정보를 담은 구조체 
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;									// 버퍼 개수
    sd.BufferDesc.Width = width;						// 버퍼 가로
    sd.BufferDesc.Height = height;						// 버퍼 높이
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 버퍼 형식(색깔)
    sd.BufferDesc.RefreshRate.Numerator = 60;			// 리프레시 제한
    sd.BufferDesc.RefreshRate.Denominator = 1;			//
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // 버퍼 쓰임새 
    sd.OutputWindow = g_hWnd;							// 버퍼가 출력 될 윈도우 핸들
    sd.SampleDesc.Count = 1;							// AA(Anti-Aliacing)
    sd.SampleDesc.Quality = 0;							// AA(Anti-Aliacing)
    sd.Windowed = TRUE;									// 윈도우 창모드 

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    { //드라이브 배열에 있는 값들을 일일이 확인하여 적용을 성공하면 반복 종료.
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

	// 렌더 타겟 뷰 생성
	// 텍스쳐는 파이프라인으로부터 뷰(View)를 통해 접근이 가능함. 
	// 렌더타겟도 텍스쳐의 일종이므로 뷰를 통해 접근 해야 함.
	// 이때 렌더타겟에 접근을 하기 위해 렌더타겟 뷰를 사용한다.
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

	//백버퍼의 렌더타겟 뷰를 생성한다. 
    hr = g_pd3dDevice->CreateRenderTargetView( 
		pBackBuffer,				// 뷰에서 엑세스 하는 리소스
		NULL,						// 렌더타겟 뷰의 정의
		&g_pRenderTargetView );		// 렌더타겟 뷰를 받아올 변수
    pBackBuffer->Release();			// 렌더타겟 뷰가 생성된 이후에는 
    if( FAILED( hr ) )				// 렌더타겟 뷰로 접근
        return hr;					// 따라서 렌더타겟 뷰의 인터페이스를 얻은 후 백버퍼 포인터 해제

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

	// 뷰 포트 설정.
	// 뷰 포트 란? 렌더타겟의 렌더링 영역에 관한 설정.
	// 렌더 타겟의 넓이와 높이, 깊이값으로 렌더링 영역 설정. 
	// 때문에 뷰 포트는 각 렌더타겟별로 설정을 해야함. ex) 8개의 렌더타겟이 있으면 뷰포트도 8개..
    D3D11_VIEWPORT vp; // 뷰 포트 객체 생성 
    vp.Width = (FLOAT)width;		// 넓이
    vp.Height = (FLOAT)height;		// 높이
    vp.MinDepth = 0.0f;				// 최소 깊이
    vp.MaxDepth = 1.0f;				// 최대 깊이
    vp.TopLeftX = 0;				// 좌상단 X좌표
    vp.TopLeftY = 0;				// 좌상단 Y좌표 
    g_pImmediateContext->RSSetViewports( 1, &vp ); //뷰 포트에 RS 설정 
												   //RS: 레스터라이저 스테이지 

    // 정점 쉐이더 컴파일 
    ID3DBlob* pVSBlob = NULL; // ID3DBlob 인터페이스, 정점 쉐이더 객체 
    hr = CompileShaderFromFile( L"Tutorial02.fx", "VS", "vs_4_0", &pVSBlob ); //해당 쉐이더를 컴파일하여 포인터에 연결
    if( FAILED( hr ) )
    {//예외 처리 
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// 정점 쉐이더 생성 
	//정점 쉐이더를 디바이스에 연결 
	hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader );
	if( FAILED( hr ) )
	{	//실패시 쉐이더 객체 해제 
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

	// 픽셀 쉐이더 컴파일 
	ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( L"Tutorial02.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// 픽셀 쉐이더 생성 
	hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

	UINT indices[15] = { //인덱스(색인) 버퍼 
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4 ,5,
		0, 5, 1
	};

	/*
		0.0f, 0.0f, 0.0f	//0 중점
       -0.5f, 0.3f, 0.0f	//1
        0.0f, 0.5f, 0.0f	//2
		0.5f, 0.3f, 0.0f	//3
	   -0.25f, -0.3f, 0.0f	//4
		0.25f, -0.3f, 0.0f	//5
	*/
	//인덱스 버퍼를 서술하는 구조체를 채움. 결속(BIND) 플래그로
	//D3D11_BIND_INDEX_BUFFER를 지정함. 
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE; 
	ibd.ByteWidth = sizeof(UINT) * (sizeof(indices) / sizeof(indices[0]));
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	//인덱스 버퍼를 초기화할 자료를 지정 
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	//인덱스 버퍼를 생성
	ID3D11Buffer* mIB;
	hr = g_pd3dDevice->CreateBuffer(&ibd, &iinitData, &mIB);
	if (FAILED(hr))
		return hr;

	//인덱스 버퍼 설정 
	g_pImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	//위상 기하 설정
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // 정점 버퍼 초기화
    SimpleVertex vertices[] =
    {
        XMFLOAT3( 0.0f, 0.0f, 0.0f), //0 중점
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
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer ); //디바이스에 버퍼 연결 
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //표현할 방법
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
	g_pImmediateContext->DrawIndexed(15,0,0); //정점 개수 | 시작 인덱스 | 기준점 
   // g_pImmediateContext->Draw(VertexCount, 0 );


    // Present the information rendered to the back buffer to the front buffer (the screen)
    g_pSwapChain->Present( 0, 0 );
}
