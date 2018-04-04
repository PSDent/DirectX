#pragma once
#include <vector>
#include <string>
#include "Sprite.h"
#include "Animation.h"
#include "Audio.h"
#include "Input.h"
#include "CircleCollider.h"
#include "Common.h"
#include "ResourcePath.h"

const float MAX_HORIZONTAL = 1.0f;
const float MAX_VERTICAL = 1.0f;
const float INCREASE_HORIZONTAL = 1.0f;
const float INCREASE_VERTICAL = 1.0f;
const float SHIFT_SPEED = 1.0f;
const int PLAYSCR_W = 700;
const int PLAYSCR_H = 768;
const int COOLTIME = 100;
const int GAP = 40;

class Object
{
public:
	Object();
	~Object();

	void ReceiveInput(Input &input);
	bool InitObject(ID3D11Device*, bool, float, float, float, float, float, int, int, const string, const WCHAR*);
	void ResetPosition();
	void CheckCollision();
	void PlayAnimation();
	void EndAnimation();
	bool Do_Animation();

	// Setter
	void SetActiveState(bool);
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
	bool GetPlayingAnime();
	vector<Object>& GetProjectile();
	bool GetActiveState();
	int GetPrevPosX();
	int GetPrevPosY();
	int GetWidth();
	int GetHeight();
	float GetPosX();
	float GetPosY();
	Collider_Info GetColl_Info();

	Sprite& GetSprite();
	Animation& GetAnime();
	Audio& GetAudio();

private:
	void SetPlayingAnime(bool);
	void MoveVertical();
	void MoveHorizon();
	void Shoot();
	bool IsCooltime();

private:
	string tag;
	float pivotX, pivotY;
	// to decide object's direction. 
	float vertical;   // + down, - up
	float horizontal; // + right, - left
	int projectileCnt;
	bool isTimerOn;

	// time
	int startTime;
	int nowTime;

	// Object basic Value 
	bool isPlayingAnime;
	float hp, mp;
	float speed;
	float fireRapidity;
	float damage;
	float range;
	float posX, posY;
	float score;
	float prevSpeed;
	bool active;

	LARGE_INTEGER largeInt, freq;

	ID3D11Device *device;
	ID3D11DeviceContext *deviceContext;

	// Components
	Sprite *sprite;
	Sprite *plane;
	Sprite *explosion;
	vector<Object> projectile;
	Animation *animation;
	Audio *audio;
	CircleCollider *cirCollider;
};

