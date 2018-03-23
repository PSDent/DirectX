#include <DirectXMath.h>
#include "Object.h"
//#include 

class Collider
{
public:
	Collider();
	Collider(float, float, float, float);
	
	void Release();
	virtual ~Collider();
	bool CheckCollision(vector<Object> &);

private:
	virtual bool Check() = 0;
	void SelectionBullet();

private:
	float posX, posY;
	float collision_W, collision_H;
	vector<Object> task;
};