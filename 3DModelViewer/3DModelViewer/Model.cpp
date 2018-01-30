#include "Model.h"

Model::Model()
{
	m_vertexBuf = 0;
	m_indexBuf = 0;
	m_Texture = 0;
	m_model = 0;
}

Model::~Model() {}

bool Model::Init(ID3D11Device *device, WCHAR *texPath, char *modelPath)
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

void Model::Release()
{
	ReleaseBuf();
	ReleaseTexture();
	ReleaseModel();

	return;
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuf(deviceContext);

	return;
}

int Model::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return m_Texture->GetTexPtr();
}

bool Model::InitBuf(ID3D11Device* device)
{
	VertexType *vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC vertexBufDesc, indexBufDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT hr;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	///////////////////////////////
	// Load Vertex Data 

	for (int i = 0; i < m_vertexCount; i++) {
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].tex = XMFLOAT2(m_model[i].u, m_model[i].v);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	///////////////////////////////
	// VertexBuffer Description

	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&vertexBufDesc, &vertexData, &m_vertexBuf);
	if (FAILED(hr))
		return false;
	
	/////////////////////////////
	// IndexBuffer Description

	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&indexBufDesc, &indexData, &m_indexBuf);
	if (FAILED(hr))
		return false;

	delete vertices;
	delete indices;

	return true;
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

bool Model::LoadModel(char* modelPath)
{
	ifstream in;
	char Cin;
	int i;

	in.open(modelPath);

	if (in.fail()) {
		MessageBox(NULL, L"Failed to Load Model File.", L"Error", MB_OK);
		return false;
	}

	in.get(Cin);
	while (Cin != ':')
		in.get(Cin);

	in >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
		return false;

	in.get(Cin);
	while (Cin != ':')
		in.get(Cin);

	in.get(Cin);
	in.get(Cin);

	for (int i = 0; i < m_vertexCount; i++) {
		in >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		in >> m_model[i].u >> m_model[i].v;
		in >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	in.close();

	return true;
}

void Model::ReleaseModel()
{
	if (m_model) 
		delete[] m_model;

	return;
}