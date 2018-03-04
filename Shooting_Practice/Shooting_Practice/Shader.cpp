#include "Shader.h"

Shader::Shader()
{
	m_VS = 0;
	m_PS = 0;
	m_layout = 0;
	m_matrixBuf = 0;
	m_sampleState = 0;
}

Shader::~Shader() {}

bool Shader::Init(ID3D11Device *device, HWND hWnd)
{
	bool rs;

	rs = InitShader(device, hWnd, L"..\\Shooting_Practice\\VS.hlsl", L"..\\Shooting_Practice\\PS.hlsl");
	if (!rs)
		return false;


	return true;
}

void Shader::Release()
{
	ReleaseShader();

	return;
}

bool Shader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix,
	ID3D11ShaderResourceView* texture)
{
	bool rs;

	rs = SetShaderParam(deviceContext, worldMatrix, viewMatrix, orthoMatrix, texture);
	if (!rs)
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

bool Shader::InitShader(ID3D11Device* device, HWND hWnd, const WCHAR* pathVS, const WCHAR* pathPS)
{
	HRESULT hr;
	ID3D10Blob* errorMsg = 0;
	ID3D10Blob* vsBuf = 0, *psBuf = 0;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElem;
	D3D11_BUFFER_DESC matrixBufDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	/////////////////////////////
	// Compile the Shader Files

	//
	// VertexShader
	//
	hr = D3DX11CompileFromFile(pathVS, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		NULL, &vsBuf, &errorMsg, NULL);

	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Compile Vertex Shader.", L"Error", MB_OK);
		return false;
	}

	hr = device->CreateVertexShader(vsBuf->GetBufferPointer(), vsBuf->GetBufferSize(), NULL, &m_VS);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Create Vertex Shader.", L"Error", MB_OK);
		return false;
	}

	//
	// PixelShader
	//
	hr = D3DX11CompileFromFile(pathPS, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		NULL, &psBuf, &errorMsg, NULL);

	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Compile Pixel Shader.", L"Error", MB_OK);
		return false;
	}

	hr = device->CreatePixelShader(psBuf->GetBufferPointer(), psBuf->GetBufferSize(), NULL, &m_PS);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Create Pixel Shader.", L"Error", MB_OK);
		return false;
	}

	/////////////////////////////
	// Input Layout Description
	// 
	// It must be same with the VertexType which in the Sprite Class
	//

	//
	// POSITION
	//
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].SemanticName = "POSITION";

	//
	// Texture
	//
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].SemanticName = "TEXCOORD";

	numElem = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	hr = device->CreateInputLayout(polygonLayout, numElem,
		vsBuf->GetBufferPointer(), vsBuf->GetBufferSize(), &m_layout);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Create Input Layout.", L"Error", MB_OK);
		return false;
	}

	vsBuf->Release();
	psBuf->Release();

	////////////////////////////////////////
	// Constant Matrix Buffer Description.

	matrixBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufDesc.ByteWidth = sizeof(MatrixBufType);
	matrixBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufDesc.MiscFlags = 0;
	matrixBufDesc.StructureByteStride = 0;
	matrixBufDesc.Usage = D3D11_USAGE_DYNAMIC;

	hr = device->CreateBuffer(&matrixBufDesc, NULL, &m_matrixBuf);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Create Matrix Buffer.", L"Error", MB_OK);
		return false;
	}

	////////////////////////////
	// SamplerState Descriptipn

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap : keeping Coord to 0.0f ~ 1.0f
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;

	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD = 0;
	samplerDesc.MipLODBias = 0.0f;

	hr = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Create Sampler State", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Shader::ReleaseShader()
{
	if (m_matrixBuf) m_matrixBuf->Release();
	if (m_layout) m_layout->Release();
	if (m_PS) m_PS->Release();
	if (m_VS) m_VS->Release();
	if (m_sampleState) m_sampleState->Release();

	return;
}

void Shader::OutputShaderErrorMsg(ID3D10Blob *errorMsg, HWND hWnd, WCHAR* shaderFile)
{
	char* compileError;
	unsigned long bufSize;
	ofstream fout;

	compileError = (char*)(errorMsg->GetBufferPointer());

	bufSize = errorMsg->GetBufferSize();

	fout.open("shaderError.txt");
	for (int i = 0; i < bufSize; i++)
		fout << compileError[i];

	fout.close();

	errorMsg->Release();

	MessageBox(hWnd, L"Check your Error File.", shaderFile, MB_OK);

	return;
}

bool Shader::SetShaderParam(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix,
	ID3D11ShaderResourceView *texture)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufType *dataPtr;
	unsigned int bufNum;

	///////////////////////////////
	// Tranpose the Matrix to Use
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	orthoMatrix = XMMatrixTranspose(orthoMatrix);

	/////////////////////////
	// Constant Buffer
	hr = deviceContext->Map(m_matrixBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Mapping Matrix Resource.", L"Error", MB_OK);
		return false;
	}

	////////////////////////////////
	// Subresource : Matrix Buffer

	//////////
	// Lock
	dataPtr = (MatrixBufType*)mappedResource.pData;

	// Copy
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->proj = orthoMatrix;

	///////////
	// UnLock 
	deviceContext->Unmap(m_matrixBuf, 0);

	// Set the Constant Buffer's Position which in the Vertex Shader 
	bufNum = 0;

	deviceContext->VSSetConstantBuffers(bufNum, 1, &m_matrixBuf);

	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void Shader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set Shaders & Input Layout
	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_VS, NULL, 0);
	deviceContext->PSSetShader(m_PS, NULL, 0);

	// Set Sample State in the PS.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Draw
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}