#include "Enemy.h"

int Total; //“G‚Ì‘”
int Left;  //–¢“oê‚Ì“G‚Ì”B“G‚ğoŒ»‚³‚¹‚é‚½‚Ñ‚ÉŒ¸­‚µ‚Ä‚¢‚­
glm::vec3 Pos;

/*
ƒ]ƒ“ƒr‚Ìó‘Ô‚ğXV‚·‚é
*/
void ZombieActor::Update(float deltaTime)
{
	Pos = position;
	Actor::Update(deltaTime);
}

//ƒ]ƒ“ƒr‚Ì’e‚ÌXV
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

