#include "WaterEnemy.h"

extern int Left;
extern int Total;
extern glm::vec3 PlayerPos;
glm::vec3 EnemyPos;


void WaterEnemy::LocalInitialize()
{
	ZombieActor enemy;
	WaterEnemy waterEnemy;
	texWater.Reset(Texture::LoadImage2D("Res/tga/mizu.tga"));
	colLocal =
	{
		glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(2.0f, 3.4f, 2.0f)
	};
	waterEnemy.Num = 1;
	waterEnemy.Speed = 1.0f;
	Hit = false;
	knockBack = false;
	attackTimer = 3.0f;
	Actor::Initialize(4, texWater.Get(), 100, 1,
		glm::vec3(0, 0.2f, 0), glm::vec3(0), glm::vec3(2));
	target = &player;
	target->position = glm::vec3(-5.0f, 0.2f, -5.0f);
	EnemyPos = position;
}

void WaterEnemy::Update(float deltaTime)
{
	target = &player;
	target->Update(deltaTime);
	ZombieActor enemy;
	if (!target)
	{
		return;
	}
	
	if (helth <= 0)
	{
		rotation.x -= glm::radians(45.0f) * deltaTime;
		if (rotation.x < glm::radians(-90.0f))
		{
			rotation.x = glm::radians(-90.0f);
		}
		return;
	}

	//のけぞる
	if (Hit == true)
	{
		rotation.x += glm::radians(30.0f) * deltaTime;
		if (rotation.x < glm::radians(10.0f))
		{
			knockBack = true;
			Hit = false;
			rotation.x = glm::radians(10.0f);
		}		
	}
	if (knockBack == true)
	{
		rotation.x -= glm::radians(30.0f) * deltaTime;
		if (rotation.x < glm::radians(0.0f))
		{
			knockBack = false;
			rotation.x = glm::radians(0.0f);
		}
	}

	float playerRadian;
	float vPlayerRadian;
	//ターゲット(プレイヤー)の方向を調べる
	const glm::vec3 vTarget = glm::normalize(ObjectToPlayerVec);
	playerRadian = std::atan2(-vTarget.z, vTarget.x);
	const glm::vec3 vPlayerFront =
		glm::rotate(
			glm::mat4(1), playerRotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, -1);
	const glm::vec3 front = glm::normalize(vPlayerFront);
	vPlayerRadian = std::atan2(-front.z, front.x);
	const float playerFront = playerRadian - vPlayerRadian;
	const float PlayerToObjectLength = glm::length(ObjectToPlayerVec);
	if (playerFront <= 0.5f && playerFront >= -0.5f &&
		PlayerToObjectLength <= 10.0f && state != State::disturbAttack &&
		helth <= 100)
	{
		lookTimer += deltaTime;
		if (lookTimer >= 1.0f)
		{
			state = State::disturbAttack;
		}
	}
	else
	{
		lookTimer = 0.0f;
	}

	if (state == State::disturbAttack)
	{
		float objectRadian;
		float vObjectRadian;
		//ターゲット(プレイヤー)の方向を調べる
		const glm::vec3 vTarget = glm::normalize(objectVec);
		objectRadian = std::atan2(-vTarget.z, vTarget.x);
		const glm::vec3 vEnemyFront =
			glm::rotate(
				glm::mat4(1), rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, -1);
		const glm::vec3 front = glm::normalize(vEnemyFront);
		vObjectRadian = std::atan2(-front.z, front.x);
		const float Front = objectRadian - vObjectRadian;

		if (Front >= -0.1f)
		{
			rotation.y += glm::radians(180.0f) * deltaTime;
		}
		else if (Front <= 0.1f)
		{
			rotation.y -= glm::radians(180.0f) * deltaTime;
		}
		/*
		const float objectLength = glm::length(objectVec);
		if (objectLength <= 1.0f)
		{
			velocity = glm::vec3(0);
			state = State::wait;
		}
		else if (objectLength > 1.0f)
		{
			const glm::mat4x4 matRotY =
				glm::rotate(
					glm::mat4(1), rotation.y + Front, glm::vec3(0, 1, 0));
			velocity = matRotY * glm::vec4(0, 0, -20, 1);
		}
		*/
	}

	if (attackTimer <= 0)
	{
		velocity = glm::vec3(0);
		if (state == State::wait)
		{
			if (glm::length(v) > 10.0f)
			{
				state = State::farAttack;
			}
			else if (glm::length(v) < 6.0f)
			{
				state = State::nearAttack;
			}
			else
			{
				state = State::midlleAttack;
			}
		}
		if (state == State::nearAttack && down == false)
		{
			rotation.x -= glm::radians(90.0f) * (deltaTime * 5);
			if (rotation.x <= glm::radians(-45.0f))
			{
				colLocal =
				{
					glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(4.0f, 3.4f, 4.0f)
				};
			}
			if (rotation.x <= glm::radians(-90.0f))
			{
				rotation.x = glm::radians(-90.0f);
				down = true;
			}
		}
		if (down == true)
		{
			rotation.x += glm::radians(90.0f) * (deltaTime * 5);
			if (rotation.x >= glm::radians(0.0f))
			{
				down = false;
				rotation.x = glm::radians(0.0f);
				attackTimer = 5.0f;
				colLocal =
				{
					glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(2.0f, 3.4f, 2.0f)
				};
				state = State::wait;
			}
		}
		if (state == State::farAttack)
		{
			const glm::vec3 lengthVec = rushPos - position;
			const glm::vec3 vlength = glm::normalize(lengthVec);
			const float length = glm::length(lengthVec);
			if (length <= 0.5f)
			{
				attackTimer = 5.0f;
				velocity = glm::vec3(0);
				state = State::wait;
				rush = false;
			}
			else
			{
				const glm::mat4x4 matRotY =
					glm::rotate(
						glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
				velocity = matRotY * glm::vec4(0, 0, -20, 1);
			}
			
		}
	}
	if (state == State::wait)
	{
		attackTimer -= deltaTime;
		if (attackTimer > 2)
		{
			std::mt19937 random;
			random.seed(std::random_device()());
			std::uniform_int_distribution<int> Base(1, 3);
			Select = Base(random);
		}
		if (attackTimer <= 2 && attackTimer > 0)
		{
			if (Select == 1)
			{
				type = Type::fire;
			}
			else if (Select == 2)
			{
				type = Type::water;
			}
			else if (Select == 3)
			{
				type = Type::wind;
			}
		}
		
		float radian;
		float vRadian;
		//ターゲット(プレイヤー)の方向を調べる
		const glm::vec3 vTarget = glm::normalize(v);
		radian = std::atan2(-vTarget.z, vTarget.x);
		const glm::vec3 vZombieFront =
			glm::rotate(
				glm::mat4(1), rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, -1);
		const glm::vec3 enemyFront = vZombieFront - position;
		const glm::vec3 front = glm::normalize(vZombieFront);
		vRadian = std::atan2(-front.z, front.x);
		const float Front = radian - vRadian;

		if (Front > 0.0f)
		{
			rotation.y += glm::radians(90.0f) * deltaTime;
		}
		else if (Front < 0.0f)
		{
			rotation.y -= glm::radians(90.0f) * deltaTime;
		}

	}

	EnemyPos = position;
	ZombieActor::Update(deltaTime);
}