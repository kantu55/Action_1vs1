#pragma once
#ifndef WATERENEMY_H_INCLUDED
#define WATERENEMY_H_INCLUDED
#include "Enemy.h"
#include "Texture.h"

class WaterEnemy : public ZombieActor
{
public:
	virtual ~WaterEnemy() = default;
	virtual void LocalInitialize();
	virtual void Update(float deltaTime);
	std::mt19937 random;
	bool Hit;
	bool knockBack;
	glm::vec3 v;
	glm::vec3 playerPosition;
	glm::vec3 playerRotation;
	glm::vec3 rushPos;
	glm::vec3 objectVec;
	glm::vec3 ObjectToPlayerVec;
	float attackTimer;
	float lookTimer;
	
	bool down = false;
	bool rush = false;
	float rushTimer = 100;
	int Select;

	enum class Type
	{
		fire,
		water,
		wind
	};
	Type type = Type::fire;

	enum class State
	{
		wait,
		nearAttack,
		farAttack,
		midlleAttack,
		disturbAttack
	};
	State state = State::wait;
	
private:
	WaterEnemy* waterEnemis[20];
	Texture::Image2D texWater;
};

#endif // !WATERENEMY_H_INCLUDED
