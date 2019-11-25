#include "Object.h"

void ObjActor::Initialize(int mesh, GLuint tex, int hp, int state,
	const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
{
	const int meshId = 7; //íeÇÃÉÅÉbÉVÉÖID
	const glm::mat4 matRotY =
		glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
	colLocal =
	{
		glm::vec3(-0.5f,0,-0.5f), glm::vec3(1.0f)
	};
	Actor::Initialize(mesh, tex, hp, state, pos, rot, scale);
}
void ObjActor::Update(float deltaTime)
{
	
	
	Actor::Update(deltaTime);
}