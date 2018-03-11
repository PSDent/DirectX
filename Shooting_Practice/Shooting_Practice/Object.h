#pragma once
#include <vector>
#include "Sprite.h"
#include "Animation.h"
#include "Audio.h"

class Object
{
public:
	Object();
	~Object();

	//void Init();
	bool InitObject(ID3D11Device*, int, int, int, int, WCHAR*);

	int GetPrevPosX();
	int GetPrevPosY();
	int GetWidth();
	int GetHeight();

	float GetPosX();
	float GetPosY();

	Sprite& GetSprite();
	Animation& GetAnime();
	Audio& GetAudio();

private:
	// Object basic Value 
	float hp, mp;
	float speed;
	float fireRapidity;
	float damage;
	float range;
	float posX, posY;

	// Components
	Sprite *sprite;
	vector<Object> projectile;
	Animation *animation;
	Audio *audio;

};

