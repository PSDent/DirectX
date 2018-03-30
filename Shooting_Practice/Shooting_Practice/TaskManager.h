#pragma once
#include <vector>
#include "Object.h"
#include "Common.h"

using namespace std;

class TaskManager
{
public:
	TaskManager();
	~TaskManager();

	void Release();
	vector<Collision_Info> Frame();
	bool Init();

	Collider_Info& GetTask_Enemy();
	void InsertTask_Enemy(Collider_Info&);

	Collider_Info& GetTask_Player();
	void InsertTask_Player(Collider_Info&);

public:

private:


private:
	// 적에 관련된 모든 좌표. (투사체, 적측 비행기 좌표)
	Collider_Info task_Enemy[30]; 
	int index_Enemy;

    // 아군에 관련된 모든 좌표. (투사체)
	Collider_Info task_Player[40];
	int index_Player;


};