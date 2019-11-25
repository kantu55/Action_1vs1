#pragma once
#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED
#include "Player.h"
#include "Enemy.h"

class ObjActor : public Actor
{
public:
	virtual ~ObjActor() = default;
	virtual void Initialize(int mesh, GLuint tex, int hp, int state,
		const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) override;
	virtual void Update(float deltaTime) override;

	Texture::Image2D texRedAmmo;
	Texture::Image2D texBlueAmmo;
	Texture::Image2D texGreenAmmo;
};



#endif // !OBJECT_H_INCLUDED
