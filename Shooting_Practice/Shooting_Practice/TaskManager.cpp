#include "TaskManager.h"

TaskManager::TaskManager()
{
	index_Enemy = 0;
	index_Player = 0;
}

TaskManager::~TaskManager()
{

}

bool TaskManager::Init()
{

	return true;
}

void TaskManager::Release()
{


	return;
}

vector<Collision_Info> TaskManager::Frame()
{
	vector<Collision_Info> temp;

	for (int i = 0; i < MAX_ENEMY; ++i) { // Enemy
		for (int j = 0; j < MAX_PROJECTION + 1; ++j){ // Player

			if (task_Enemy[i].active && task_Player[j].active) {
				int a = (task_Enemy[i].x - task_Player[j].x) * (task_Enemy[i].x - task_Player[j].x);
				int b = (task_Enemy[i].y - task_Player[j].y) * (task_Enemy[i].y - task_Player[j].y);
				int c = (task_Enemy[i].r + task_Player[j].r) * (task_Enemy[i].r + task_Player[j].r);

				if (a + b <= c) {
					if(task_Player[j].tag == Projectile)
  						temp.push_back(Collision_Info { i + 1, Enemy, j - 1, Projectile });
					else if(task_Player[j].tag == Player) 
						temp.push_back(Collision_Info{ 0, Player, 0, Enemy });
				}
			}

		}
	}

	return temp;
}

Collider_Info& TaskManager::GetTask_Enemy(){
	return *task_Enemy;
}

void TaskManager::InsertTask_Enemy(Collider_Info &info)
{
	if (index_Enemy == MAX_ENEMY)
		index_Enemy = 0;
	task_Enemy[index_Enemy++] = info;
	return;
}

Collider_Info& TaskManager::GetTask_Player() {
	return *task_Player;
}

void TaskManager::InsertTask_Player(Collider_Info &info)
{
	if (index_Player == MAX_PROJECTION + 1)
		index_Player = 0;
	task_Player[index_Player++] = info;
	return;
}