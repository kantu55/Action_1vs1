#pragma once
#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED
#include "Object.h"

class Red : public ObjActor
{
public:
	virtual ~Red() = default;
	virtual void Initialize(int mesh, GLuint tex, int hp, int state,
		const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) override;
	virtual void Update(float deltaTime) override;

	std::vector<Actor*> reds;
};

class Blue : public ObjActor
{
public:
	virtual ~Blue() = default;
	virtual void Initialize(int mesh, GLuint tex, int hp, int state,
		const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) override;
	virtual void Update(float deltaTime) override;
};

class Green : public ObjActor
{
public:
	virtual ~Green() = default;
	virtual void Initialize(int mesh, GLuint tex, int hp, int state,
		const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) override;
	virtual void Update(float deltaTime) override;
};

#endif // !BULLET_H_INCLUDED
