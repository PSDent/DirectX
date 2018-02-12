#include "Shader.h"

Shader::Shader()
{
	m_VS = 0;
	m_PS = 0;
	m_layout = 0;
	m_matrixBuf = 0;
	m_sampleState = 0;
	m_lightBuf = 0;
	m_cameraBuf = 0;
}

Shader::~Shader() {}

bool Shader::Init(ID3D11Device *device, HWND hWnd)
{
	bool rs;

	rs = InitShader(device, hWnd, L"..\\3DModelViewer\\VS.hlsl", L"..\\3DModelViewer\\PS.hlsl");
	if (!rs)
		return false;


	return true;
}

void Shader::Release()
{
	ReleaseShader();

	return;
}

bool Shader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projMatrix,
	ID3D11ShaderResourceView* texture, XMFLOAT3 lightDir, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor,
	XMFLOAT3 cameraPos, XMFLOAT4 specularColor, float specularPower)
{
	bool rs;

	rs = SetShaderParam(deviceContext, worldMatrix, viewMatrix, projMatrix, texture, 
		lightDir, ambientColor, diffuseColor, cameraPos, specularColor, specularPower);
	if (!rs)
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

bool Shader::InitShader(ID3D11Device* device, HWND hWnd, WCHAR* pathVS, WCHAR* pathPS)
{
	HRESULT hr;
	ID3D10Blob* errorMsg = 0;
	ID3D10Blob* vsBuf = 0, *psBuf = 0;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElem;
	D3D11_BUFFER_DESC matrixBufDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufDesc;
	D3D11_BUFFER_DESC cameraBufDesc;

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
	// It must be same with the VertexType which in the Model Class.
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

	//
	// Normal
	//
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].SemanticName = "NORMAL";

	numElem = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	hr = device->CreateInputLayout(polygonLayout, numElem, vsBuf->GetBufferPointer(), vsBuf->GetBufferSize(), &m_layout);
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

	/////////////////////////////////////////////
	// Normal Vector Constant Buffer Description
	lightBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufDesc.ByteWidth = sizeof(LightBufType);
	lightBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufDesc.MiscFlags = 0;
	lightBufDesc.StructureByteStride = 0;
	lightBufDesc.Usage = D3D11_USAGE_DYNAMIC;

	hr = device->CreateBuffer(&lightBufDesc, NULL, &m_lightBuf);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Create Light Buffer.", L"Error", MB_OK);
		return false;
	}

	/////////////////////////////////////////////
	// Camera Position Buffer Description

	cameraBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufDesc.ByteWidth = sizeof(CameraBufType);
	cameraBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufDesc.MiscFlags = 0;
	cameraBufDesc.StructureByteStride = 0;
	cameraBufDesc.Usage = D3D11_USAGE_DYNAMIC;

	hr = device->CreateBuffer(&cameraBufDesc, NULL, &m_cameraBuf);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Create Camera Buffer.", L"Error", MB_OK);
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
	if (m_lightBuf) m_lightBuf->Release();
	if (m_cameraBuf) m_cameraBuf->Release();

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

bool Shader::SetShaderParam(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projMatrix, 
	ID3D11ShaderResourceView *texture, XMFLOAT3 lightDir, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor,
	XMFLOAT3 cameraPos, XMFLOAT4 specularColor, float specularPower)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufType *dataPtr;
	LightBufType *dataPtr2;
	CameraBufType *dataPtr3;
	unsigned int bufNum;

	///////////////////////////////
	// Tranpose the Matrix to Use
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projMatrix = XMMatrixTranspose(projMatrix);

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
	dataPtr->proj = projMatrix;

	///////////
	// UnLock 
	deviceContext->Unmap(m_matrixBuf, 0);
	
	// Set the Constant Buffer's Position which in the Vertex Shader 
	bufNum = 0;

	deviceContext->VSSetConstantBuffers(bufNum, 1, &m_matrixBuf);

	/////////////////////////////////
	// Subresource : Light Buffer

	///////////
	// Lock
	hr = deviceContext->Map(m_lightBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Failed to Mapping Lighting Resource.", L"Error", MB_OK);
		return false;
	}

	dataPtr2 = (LightBufType*)mappedResource.pData;

	// Copy
	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDir;
	dataPtr2->specularColor = specularColor;
	dataPtr2->specularPower = specularPower;

	////////////
	// Unlock
	deviceContext->Unmap(m_lightBuf, 0);

	bufNum = 0;

	deviceContext->PSSetConstantBuffers(bufNum, 1, &m_lightBuf);

	deviceContext->PSSetShaderResources(0, 1, &texture);

	///////////////////////////////////////////
	// Subresource : Camera Buffer (position)

	////////
	// Lock
	hr = deviceContext->Map(m_cameraBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
		return false;

	dataPtr3 = (CameraBufType*)mappedResource.pData;

	// copy
	dataPtr3->CameraPos = cameraPos;
	dataPtr3->padding = 0.0f;

	///////////
	// Unlock
	deviceContext->Unmap(m_cameraBuf, 0);

	bufNum = 1;

	deviceContext->VSSetConstantBuffers(bufNum, 1, &m_cameraBuf);

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