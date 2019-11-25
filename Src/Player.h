#pragma once
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "GLFWEW.h"
#include "Actor.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshList.h"
#include <vector>
#include <random>
#include <glm/gtc/matrix_transform.hpp>

class PlayerActor : public Actor
{
public:
	virtual ~PlayerActor() = default;
	virtual void Initialize(int mesh, GLuint tex, int hp, int state,
		const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) override;
	virtual void ProcessInput();
	virtual void Update(float deltaTime) override;
	
	//プレイヤーのカメラ位置と注視点
	glm::vec3 viewPos;
	glm::vec3 targetPos;
	glm::vec3 viewoffset;
	glm::vec3 targetoffset;
	glm::vec3 vCamera;
	glm::vec3 vTraget;
	glm::vec3 vCameraTraget;
	glm::vec3 vTragetoffset;
	glm::vec3 cameraVelocity;
	glm::vec3 TragetVelocity;
	glm::vec3 mousePos = glm::vec3(0);
	glm::vec2 mouseAngles;

	glm::vec3 v;

	float rotationSpeed = 1.0f * glm::radians(60.0f);
	float frontRange = glm::radians(90.0f);
	float knockBackTimer;
	bool back = false;
	bool revers = false;
	bool knockBack = false;

	double mousex;
	double mousey;

private:
	MeshList meshList;
	Texture::Image2D texHuman;
	enum class State
	{
		play,        //通常プレイ中
		pouse,
		stageClear,  //ステージクリア
		gameOver,    //ゲームオーバー
	};
	State gameState = State::play;
};

/*
プレイヤーから発射される弾のアクター
*/
class PlayerBullet : public Actor
{
public:
	virtual ~PlayerBullet() = default;
	virtual void Initialize();
	virtual void ProcessInput();
	virtual void Update(float deltaTime) override;
	float BulletLoad(Actor*, glm::vec3, glm::vec3, float);
	float Timer = 0;
	float time = 0;
	std::vector<Actor*> playerBullets;

	

private:
	Texture::Image2D texBullet;
	Texture::Image2D texRed;
	Texture::Image2D texBlue;
	Texture::Image2D texGreen;
};

#endif // !PLAYER_H_INCLUDED
