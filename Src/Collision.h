#pragma once
#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Collision
{
	/*
	球
	*/
	struct Sphere
	{
		glm::vec3 center = glm::vec3(0);
		float r = 0;
	};
	/*
	直方体
	*/
	struct Rect
	{
		glm::vec3 origin;
		glm::vec3 size;
	};
	/*
	線分
	*/
	struct Segment
	{
		glm::vec3 a = glm::vec3(0); //線分の始点
		glm::vec3 b = glm::vec3(0); //線分の終点
	};
	/*
	カプセル
	*/
	struct Capsule
	{
		Segment seg; //円柱部の中心の線分
		float r = 0; //カプセルの半径
	};

	bool TestSphereSphere(const Sphere&, const Sphere&);
	bool TestSphereCapsule(const Sphere&, const Capsule&);
}

#endif // COLLISION_H_INCLUDED