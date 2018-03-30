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
	// ���� ���õ� ��� ��ǥ. (����ü, ���� ����� ��ǥ)
	Collider_Info task_Enemy[30]; 
	int index_Enemy;

    // �Ʊ��� ���õ� ��� ��ǥ. (����ü)
	Collider_Info task_Player[40];
	int index_Player;


};