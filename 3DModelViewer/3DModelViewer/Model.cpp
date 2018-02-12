#include "Model.h"

Model::Model()
{
	m_vertexBuf = 0;
	m_indexBuf = 0;
	m_Texture = 0;
}

Model::~Model() {}

bool Model::Init(ID3D11Device *device, char* modelPath, WCHAR *texPath)
{
	bool r;

	r = LoadModel(modelPath);
		if (!r) {
			MessageBox(NULL, L"Failed to Load Model.", L"Error", MB_OK);
			return false;
		}

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

bool Model::InitBuf(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	////////////////////
	// Set Model Data
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].tex = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	////////////////////////////////
	// Vertex Buffer Description
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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

void Model::Release()
{
	ReleaseTexture();
	ReleaseBuf();
	ReleaseModel();

	return;
}


bool Model::Render(ID3D11DeviceContext* deviceContext)
{
	bool rs;

	RenderBuf(deviceContext);

	return true;
}

int Model::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return m_Texture->GetTexPtr();
}

void Model::ReleaseBuf()
{
	if (m_indexBuf) m_indexBuf->Release();
	if (m_vertexBuf)m_vertexBuf->Release();

	return;
}

void Model::RenderBuf(ID3D11DeviceContext* deviceContext)
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

bool Model::LoadTexture(ID3D11Device *device, WCHAR *texPath)
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

void Model::ReleaseTexture()
{
	if (m_Texture) {
		m_Texture->Release();
		delete m_Texture;
	}

	return;
}

bool Model::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	fin.open(filename);

	if (fin.fail())
		return false;

	fin.get(input);
	while (input != ':')
		fin.get(input);

	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
		return false;

	fin.get(input);
	while (input != ':')
		fin.get(input);

	fin.get(input);
	fin.get(input);

	// Read Vertex Data
	for (i = 0; i < m_vertexCount; i++) {
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();

	return true;
}


void Model::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}