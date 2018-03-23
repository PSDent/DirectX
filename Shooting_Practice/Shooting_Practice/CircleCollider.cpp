#include "CircleCollider.h"

CircleCollider::CircleCollider()
{

}

CircleCollider::CircleCollider(float posX, float posY, float width, float height) 
	: Collider(posX, posY, width, height)
{

}

CircleCollider::~CircleCollider()
{

}

bool CircleCollider::Check()
{


	return false;
}
