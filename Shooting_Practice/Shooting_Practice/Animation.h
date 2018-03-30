#pragma once
#include "Sprite.h"
#include <vector>

class Animation
{
public:
	Animation();
	~Animation();

	bool Init();
	void RunAnimation();

private:
	bool isAnimeOn;

};

	