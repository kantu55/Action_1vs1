#include "Enemy.h"

int Total; //�G�̑���
int Left;  //���o��̓G�̐��B�G���o�������邽�тɌ������Ă���
glm::vec3 Pos;

/*
�]���r�̏�Ԃ��X�V����
*/
void ZombieActor::Update(float deltaTime)
{
	Pos = position;
	Actor::Update(deltaTime);
}

//�]���r�̒e�̍X�V
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

