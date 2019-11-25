#include "Bullet.h"


void Red::Initialize(int mesh, GLuint tex, int hp, int state,
	const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
{
	reds.resize(128);
	for (auto& bullet : reds)
	{
		bullet = new Red;
	}
	const int meshId = 7; //弾のメッシュID
	const glm::mat4 matRotY =
		glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
	ObjActor::Initialize(mesh, tex, hp, state, pos, rot, scale);
}

void Blue::Initialize(int mesh, GLuint tex, int hp, int state,
	const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
{
	const int meshId = 7; //弾のメッシュID
	const glm::mat4 matRotY =
		glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
	ObjActor::Initialize(mesh, tex, hp, state, pos, rot, scale);
}

void Green::Initialize(int mesh, GLuint tex, int hp, int state,
	const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
{
	const int meshId = 7; //弾のメッシュID
	const glm::mat4 matRotY =
		glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
	ObjActor::Initialize(mesh, tex, hp, state, pos, rot, scale);
}

void Red::Update(float deltaTime)
{
	rotation.y += glm::radians(30.0f) * deltaTime;
	ObjActor::Update(deltaTime);
}

void Blue::Update(float deltaTime)
{
	rotation.y += glm::radians(30.0f) * deltaTime;
	ObjActor::Update(deltaTime);
}

void Green::Update(float deltaTime)
{
	rotation.y += glm::radians(30.0f) * deltaTime;
	ObjActor::Update(deltaTime);
}