#pragma once
#include "Sprite.h"

typedef struct {
	Sprite animation;
	int cut_W, cut_H;
}Animation_Info;

class Animation
{
public:
	Animation();
	~Animation();

	bool Init();
	void RunAnimation();

private:
	//vector<Animation_Info> sprite_anime;


};

