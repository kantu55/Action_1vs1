#include "Enemy.h"

int Total; //敵の総数
int Left;  //未登場の敵の数。敵を出現させるたびに減少していく
glm::vec3 Pos;

/*
ゾンビの状態を更新する
*/
void ZombieActor::Update(float deltaTime)
{
	Pos = position;
	Actor::Update(deltaTime);
}

//ゾンビの弾の更新
void EnemyBulletActor::Update(float deltaTime)
{
	const float mapSize = 20;
	for (size_t i = 0; i < 3; ++i)
	{
		if (position[i] < -mapSize || position[i] > mapSize)
		{
			helth = 0;
			break;
		}
	}
	Actor::Update(deltaTime);
}

