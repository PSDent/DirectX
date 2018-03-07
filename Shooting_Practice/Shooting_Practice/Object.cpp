#include "Object.h"



Object::Object()
{
}


Object::~Object()
{
}

Sprite& Object::GetSprite() {
	return *sprite;
}

Animation& Object::GetAnime() {
	return *animation;
}
Audio& Object::GetAudio() {
	return *audio;
}

bool Object::InitObject(ID3D11Device* device)
{
	// Initialize Sprite.
	//sprite->Init(device,);

	return true;
}