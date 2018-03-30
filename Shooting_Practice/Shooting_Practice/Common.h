#pragma once
#include <string>

using namespace std;

// constant value
const float PLANE_ADDITION = 3.0f;
const float SPEED = 12.0f;
const int MAX_ENEMY = 30;
const int MAX_BACKGROUND = 2;
const int MAX_PROJECTION = 30;

// constant tag
const string Player = "PLAYER";
const string Projectile = "PROJECTILE";
const string Enemy = "ENEMY";
const string BackGround = "BACKGROUND";

typedef struct {
	string animation;
	float cut_W, cut_H;
	float width, height;
}Animation_Info;

struct Collider_Info {
	float x, y, r;
	bool active;
	string tag;
};	

struct Collision_Info {
	int to_index;
	string to;
	int from_index;
	string from;
};