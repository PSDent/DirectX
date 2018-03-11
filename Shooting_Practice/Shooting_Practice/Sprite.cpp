#include "Sprite.h"

Sprite::Sprite()
{
	m_vertexBuf = 0;
	m_indexBuf = 0;
	m_Texture = 0;
}

Sprite::~Sprite() {}

bool Sprite::Init(ID3D11Device *device, int scrX, int scrW, int spriteW, int spriteH, WCHAR *texPath)
{
	bool r;

	m_scrX = scrX;
	m_scrY = scrW;

	m_spriteW = spriteW;
	m_spriteH = spriteH;

	m_prevX = m_prevY = -1;

	r = InitBuf(device);
	if (!r) {
		MessageBox(NULL, L"Failed to Initialize Vertex Buffer.", L"Error", MB_OK);
		return false;
	}

	r = LoadTexture(device, texPath);
	if (!r) {
		MessageBox(NULL, L"Failed to Load Texture.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool Sprite::InitBuf(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_indexCount = m_vertexCount = 6;
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	for (int i = 0; i < m_indexCount; i++)
		indices[i] = i;

	////////////////////////////////
	// Vertex Buffer Description
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // dynamic buffer
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	////////////////////////////////
	// SubResource : vertexData
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	///////////////////////////////
	// Create Vertex Buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuf);
	if (FAILED(result))
		return false;

	////////////////////////////////
	// Index Buffer Description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	////////////////////////////////
	// SubResource : Index
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	///////////////////////////////
	// Create Index Buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuf);
	if (FAILED(result))
		return false;

	////////////////////
	// Release Pointer

	delete[] vertices;
	delete[] indices;

	return true;
}

void Sprite::Release()
{
	ReleaseTexture();
	ReleaseBuf();
	return;
}


bool Sprite::Render(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	bool rs;

	rs = UpdateBuf(deviceContext, posX, posY);
	if (!rs)
		return false;

	RenderBuf(deviceContext);

	return true;
}

int Sprite::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Sprite::GetTexture()
{
	return m_Texture->GetTexPtr();
}

void Sprite::ReleaseBuf()
{
	if (m_indexBuf) m_indexBuf->Release();
	if (m_vertexBuf)m_vertexBuf->Release();

	return;
}

void Sprite::RenderBuf(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	// Available VertexBuffer to IA
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuf, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuf, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Sprite::LoadTexture(ID3D11Device *device, WCHAR *texPath)
{
	bool rs;

	m_Texture = new Texture;
	if (!m_Texture)
		return false;

	rs = m_Texture->Init(device, texPath);
	if (!rs) {
		MessageBox(NULL, L"Failed to Initialize Texture.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Sprite::ReleaseTexture()
{
	if (m_Texture) {
		m_Texture->Release();
		delete m_Texture;
	}

	return;
}

bool Sprite::UpdateBuf(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	float left, right, top, bot;
	VertexType *vertices, *verticesPtr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr;

	// there is no change position.
	if ((posX == m_prevX) && (posY == m_prevY))
		return true;

	m_prevX = posX;
	m_prevY = posY;

	left = (float)((m_scrX / 2) * -1) + (float)posX;
	right = left + (float)m_spriteW;
	top = (float)(m_scrY / 2) - (float)posY;
	bot = top - (float)m_spriteH;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	// Triangle (1)
	vertices[0].position = XMFLOAT3(left, top, 0.0f);
	vertices[0].tex = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bot, 0.0f);
	vertices[1].tex = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bot, 0.0f);
	vertices[2].tex = XMFLOAT2(0.0f, 1.0f);

	// Triangle (2)
	vertices[3].position = XMFLOAT3(left, top, 0.0f);
	vertices[3].tex = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);
	vertices[4].tex = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bot, 0.0f);
	vertices[5].tex = XMFLOAT2(1.0f, 1.0f);

	/////////
	// Lock
	hr = deviceContext->Map(m_vertexBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Copy the pointer 
	verticesPtr = (VertexType*)mappedResource.pData;

	/////////
	// Copy
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	///////////
	// UnLock
	deviceContext->Unmap(m_vertexBuf, 0);

	delete[] vertices;

	return true;
}