#include <DirectXMath.h>

using namespace DirectX;

class Camera {
public:
	Camera();
	~Camera();

	void SetPos(float, float, float);
	void SetRot(float, float, float);

	XMFLOAT3 GetPos();
	XMFLOAT3 GetRot();

	void Render();
	XMMATRIX GetViewMatrix();

private:
	float m_posX, m_posY, m_posZ;
	float m_rotX, m_rotY, m_rotZ;
	XMMATRIX m_viewMatrix;

};