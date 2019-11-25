#pragma once
/*
@file Enemy.h
*/
#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Player.h"
#include "Object.h"
#include <vector>
#include <random>
#include <glm/gtc/matrix_transform.hpp>


/*
�G(�]���r)�̃A�N�^�[
*/
class ZombieActor : public Actor
{
public:
	virtual ~ZombieActor() = default;
	virtual void Update(float deltaTime) override;
	PlayerActor player;
	PlayerActor* target;
	std::vector<Actor*> players;
	bool isAttaking = false;
	float attakingTimer = 5.0f;
	float baseSpeed = 1.0f;
	float Speed = 1.0f; //�]���r�̈ړ����x
	float PoppingInterval = 15.0f; //�]���r�̏o������
	float PoppingTimer = 0.0f; //�]���r�̏o���^�C�}�[
	int Num;
	int Killed;  //�E�����G�̐��B���̐��l��enemyTotal�Ɠ������Ȃ�������X�e�[�W�N���A

	enum class Length
	{
		enemyFar,
		enemyNear,
		enemyMiddle,
	};
	Length length = Length::enemyFar;
};

class EnemyBulletActor : public Actor
{
public:
	virtual ~EnemyBulletActor() = default;
	virtual void Update(float deltaTime) override;
	PlayerActor* target = nullptr;
	float baseSpeed = 1.0f;
	const float Speed = 20.0f;
	float Timer = 0;
};





#endif //ENEMY_H_INCLUDED
