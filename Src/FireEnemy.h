#pragma once
#ifndef FIREENEMY_H_INCLUDED
#define FIREENEMY_H_INCLUDED
#include "Enemy.h"

class FireEnemy : public ZombieActor
{
public:
	virtual ~FireEnemy() = default;
	//virtual void Update(float deltaTime) override;
};

#endif // !FIREENEMY_H_INCLUDED
