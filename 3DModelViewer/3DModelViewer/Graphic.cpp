#include "Graphic.h"

Graphic::Graphic()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Shader = 0;
	m_Light = 0;
}
Graphic::~Graphic() {}

bool Graphic::Init(int width, int height, HWND hWnd, char* modelPath)
{
	bool rs;

	m_D3D = new D3DClass;
	if (!m_D3D)
		return false;

	rs = m_D3D->InitD3D(hWnd, width, height, VSYNC, WINDOW, SCR_NEAR, SCR_FAR);
	if (!rs) {
		MessageBox(NULL, L"Failed to Initialize D3D Class.", L"Error", MB_OK);
		return false;
	}

	m_Camera = new Camera;
	if (!m_Camera)
		return false;

	m_Camera->SetPos(0.0f, 0.0f, -10.0f);

	m_Model = new Model;
	if (!m_Model)
		return false;

	rs = m_Model->Init(m_D3D->GetDevice(), L"..\\3DModelViewer\\seafloor.dds", modelPath);
	if (!rs) {
		MessageBox(NULL, L"Failed to Initialize Model Class.", L"Error", MB_OK);
		return false;
	}

	m_Shader = new Shader;
	if (!m_Shader)
		return false;

	rs = m_Shader->Init(m_D3D->GetDevice(), hWnd);
	if (!rs) {
		MessageBox(NULL, L"Failed to Initialize Shader Class.", L"Error", MB_OK);
		return false;
	}

	m_Light = new Light;
	if (!m_Light)
		return false;

	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDir(0.0f, 0.0f, 1.0f);

	return true;
}
void Graphic::Release()
{
	if (m_D3D) {
		m_D3D->Release();
		delete m_D3D;
	}

	if (m_Camera) {
		delete m_Camera;
	}

	if (m_Model) {
		m_Model->Release();
		delete m_Model;
	}

	if (m_Shader) {
		m_Shader->Release();
		delete m_Shader;
	}

	if (m_Light) delete m_Light;

	return;
}

bool Graphic::Frame()
{
	static float rot = 0.0f;
	bool rs;

	rot += (float)XM_PI * 0.01f;
	if (rot > 360.0f)
		rot -= 360.0f;

	rs = Render(rot);

	if (!rs)
		return false;

	return true;
}
bool Graphic::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projMatrix;
	bool rs;

	m_D3D->Start();

	m_Camera->Render();

	viewMatrix = m_Camera->GetViewMatrix();
	worldMatrix = m_D3D->GetWorld();
	projMatrix = m_D3D->GetProj();

	worldMatrix = XMMatrixRotationY(rotation);

	m_Model->Render(m_D3D->GetDeviceContext());

	rs = m_Shader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projMatrix,
		m_Model->GetTexture(), m_Light->GetDir(), m_Light->GetDiffuseColor());
	if (!rs)
		return false;

	m_D3D->End();

	return true;
}