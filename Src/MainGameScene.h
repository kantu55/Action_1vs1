#pragma once
/*
@file MainGameScene.h
*/
#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "GLFWEW.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshList.h"
#include "Scene.h"
#include "FireEnemy.h"
#include "WaterEnemy.h"
#include "WindEnemy.h"
#include "NothingEnemy.h"
#include "Player.h"
#include "Object.h"
#include "Bullet.h"
#include "Audio.h"
#include <vector>
#include <random>


/*
���C���Q�[�����
*/
class MainGameScene : public Scene
{
public:
	MainGameScene() = default;
	virtual ~MainGameScene() = default;

	virtual bool Initialize() override;    //��ʂ̕\���ɕK�v�ȃf�[�^����������
	virtual void ProcessInput() override;  //���͂��󂯂ē�����Ԃ�ݒ肷��
	virtual void Update() override;        //�v���O�����̏�Ԃ��X�V����
	virtual void Render() override;        //���f���̕\���Ȃǂ��s��
	virtual void Finalize() override;      //Initialize�֐��ŏ��������f�[�^��j������
	void RenderLoad();
	float BulletLoad(Actor*, Actor, float);
	float EnemyBulletLoad(Actor*, Actor, float);
	void FixCollision(Actor&, Actor&);
	float BulletTimer = 0.0f;
	int RotCount;
	float Rot = -0.5f;
	int enemyRotCount;
	float enemyRot = glm::radians(-130.0f);
	float enemyGauage = -250.0f;
	float playerGauage = -250.0f;
	float damageTimer;
	float shildLength = 1;

private:
	MeshList meshList;

	Texture::Image2D texId;
	Texture::Image2D texId2;
	Texture::Image2D texHouse;
	Texture::Image2D texRock;
	Texture::Image2D texGround;
	Texture::Image2D texHuman;
	Texture::Image2D texBullet;
	Texture::Image2D texZombie;
	Texture::Image2D texSphere;

	//���\���p�e�N�X�`��
	Texture::Image2D texScore;
	Texture::Image2D texNumber[10];
	Texture::Image2D texHP;
	Texture::Image2D texStageClear;
	Texture::Image2D texGameOver;
	Texture::Image2D texMouse;
	Texture::Image2D texPouse;
	Texture::Image2D texRed;
	Texture::Image2D texBlue;
	Texture::Image2D texGreen;
	Texture::Image2D texAttribute;
	Texture::Image2D texNone;
	Texture::Image2D texFire;
	Texture::Image2D texFireImage;
	Texture::Image2D texWater;
	Texture::Image2D texWaterImage;
	Texture::Image2D texGrass;
	Texture::Image2D texGrassImage;
	Texture::Image2D texGauge;
	Texture::Image2D tex100;
	Texture::Image2D tex50;
	Texture::Image2D tex25;
	Texture::Image2D texZokusei;

	Texture::Image2D texTitle_B;
	Texture::Image2D texTitle_Y;
	Texture::Image2D texRetry_B;
	Texture::Image2D texRetry_Y;


	Shader::Program progSimple;
	Shader::Program progLighting;
	Shader::Program progLightingGuest;
	Shader::LightList lights;

	Audio::EngineRef audio = Audio::Engine::Instance();
	//BGM
	Audio::SoundPtr Mainbgm;
	//���ʉ�
	Audio::SoundPtr Bulletbgm;
	Audio::SoundPtr Stanbgm;
	Audio::SoundPtr Hitbgm;
	Audio::SoundPtr Overbgm;
	Audio::SoundPtr Enterbgm;
	Audio::SoundPtr Menubgm;

	float angleY = 0;
	float mMaxForwardpeed;

	int viewHeight = 1280;
	int viewWidth = 720;
	
	//�v���C���[�̃J�����ʒu�ƒ����_
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
	
	double mousex;
	double mousey;

	
	
	std::mt19937 random;

	//�Q�[�����
	//�񋓌^
	enum class State
	{
		play,        //�ʏ�v���C��
		pouse,
		stageClear,  //�X�e�[�W�N���A
		gameOver,    //�Q�[���I�[�o�[
	};
	State state = State::play;

	enum class GameOver
	{
		title,
		retry
	};
	GameOver over = GameOver::title;

	int stageNo = 1;
	int score = 0;
	
	Actor actor;

	ObjActor object;
	Red redObject;
	Red* red;
	Blue blueObject;
	Green greenObject;

	//�v���C���[
	PlayerActor player;
	std::vector<Actor*> myplayer;

	//�e
	PlayerBullet playerBullet;
	std::vector<Actor*> playerBullets;
	std::vector<Actor*> guestBullets;

	//��
	ObjActor floors;
	ObjActor floor[4];
	glm::vec3 floorPos[4] =
	{
		glm::vec3(0,0,-12.25f),
		glm::vec3(12.25f,0,0),
		glm::vec3(0,0,12.25f),
		glm::vec3(-12.25f,0,0)
	};
	glm::vec3 collisionMin[4] =
	{
		glm::vec3(-20.0f,-20.0f,0.0f),
		glm::vec3(0.0f,-20.0f,-20.0f),
		glm::vec3(-20.0f,-20.0f,0.0f),
		glm::vec3(0.0f,-20.0f,-20.0f)
	};
	glm::vec3 collisionMax[4] =
	{
		glm::vec3(40.0f, 40.0f,  0.1f),
		glm::vec3(0.1f, 40.0f, 40.0f),
		glm::vec3(40.0f, 40.0f,  0.1f),
		glm::vec3(0.1f, 40.0f, 40.0f),
	};
	float floorRot = 0.0f;
	ObjActor cube;
	std::vector<Actor*> ground;

	//�]���r
	WaterEnemy waterEnemy;
	WaterEnemy* waterEnemis[20];
	std::vector<Actor*> w_enemies;
	ObjActor Shild;
	ObjActor redShild;
	ObjActor blueShild;
	ObjActor greenShild;
	glm::vec3 p_dir;
	glm::vec3 p_dirOffset;
	glm::vec3 p_dirVeloctiy;
	glm::vec3 p_dirTarget;

	ZombieActor zombie;
	ZombieActor* zombies2[20];
	std::vector<Actor*> enemies;
	EnemyBulletActor enemyBullet;
	std::vector<Actor*> enemyBullets;
};

#endif //MAINGAMESCENE_H_INCLUDED