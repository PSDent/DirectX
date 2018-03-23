#include "Collider.h"

class CircleCollider : public Collider
{
public:
	CircleCollider();
	CircleCollider(float, float, float, float);
	~CircleCollider();

private:
	bool Check();
	void SelectionBullet();
	

};