#pragma once
#include <vector>
#include <string>
#include "Sprite.h"
#include "Animation.h"
#include "Audio.h"

class Object
{
public:
	Object();
	~Object();

	//void Init();
	bool InitObject(ID3D11Device*, float, float, float, float, float, int, int, const string, const WCHAR*);
	void ResetPosition();

	// Setter
	void SetSpeed(float);
	void SetVertical(float);
	void SetHorizontal(float);
	void SetPivotX(float);
	void SetPivotY(float);
	void SetPosX(float);
	void SetPosY(float);
	void SetTag(string tag);
	void Movement();
	void Render(ID3D11DeviceContext*);

	// Getter
	string GetTag();
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
	void MoveVertical();
	void MoveHorizon();

private:
	string tag;
	float pivotX, pivotY;
	// to decide object's direction. 
	float vertical;   // + down, - up
	float horizontal; // + right, - left

	// Object basic Value 
	float hp, mp;
	float speed;
	float fireRapidity;
	float damage;
	float range;
	float posX, posY;
	float score;

	// Components
	Sprite *sprite;
	vector<Object> projectile;
	Animation *animation;
	Audio *audio;

};

