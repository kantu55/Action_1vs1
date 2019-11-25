#pragma once
#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Collision
{
	/*
	��
	*/
	struct Sphere
	{
		glm::vec3 center = glm::vec3(0);
		float r = 0;
	};
	/*
	������
	*/
	struct Rect
	{
		glm::vec3 origin;
		glm::vec3 size;
	};
	/*
	����
	*/
	struct Segment
	{
		glm::vec3 a = glm::vec3(0); //�����̎n�_
		glm::vec3 b = glm::vec3(0); //�����̏I�_
	};
	/*
	�J�v�Z��
	*/
	struct Capsule
	{
		Segment seg; //�~�����̒��S�̐���
		float r = 0; //�J�v�Z���̔��a
	};

	bool TestSphereSphere(const Sphere&, const Sphere&);
	bool TestSphereCapsule(const Sphere&, const Capsule&);
}

#endif // COLLISION_H_INCLUDED