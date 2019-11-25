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
敵(ゾンビ)のアクター
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
	float Speed = 1.0f; //ゾンビの移動速度
	float PoppingInterval = 15.0f; //ゾンビの出現時間
	float PoppingTimer = 0.0f; //ゾンビの出現タイマー
	int Num;
	int Killed;  //殺した敵の数。この数値がenemyTotalと等しくなかったらステージクリア

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
