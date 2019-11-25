/*
file MainGameScene.cpp
*/

#include "MainGameScene.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include<math.h>

extern int Total;
extern int Left;

float MainGameScene::BulletLoad(Actor* bullet, Actor charctor,float speed)
{
	const int meshId = 7; //弾のメッシュID
	glm::mat4 matRotY =
		glm::rotate(glm::mat4(1), charctor.rotation.y, glm::vec3(0, 1, 0));
	if (player.state == 1)
	{
		RotCount += 1;
		bullet->Initialize(meshId, texRed.Get(), 1, 0,
			charctor.position, charctor.rotation, glm::vec3(1));
		matRotY = glm::rotate(glm::mat4(1), charctor.rotation.y + Rot, glm::vec3(0, 1, 0));
		bullet->velocity = matRotY * glm::vec4(0, 0, -speed, 1);
		Rot += 0.25f;
		if (RotCount >= 5)
		{
			Rot = -0.5f;
		}
	}
	else if (player.state == 2)
	{
		bullet->Initialize(meshId, texBlue.Get(), 1, 1,
			charctor.position, charctor.rotation, glm::vec3(1));
		matRotY =
			glm::rotate(glm::mat4(1), charctor.rotation.y, glm::vec3(0, 1, 0));
		bullet->velocity = matRotY * glm::vec4(0, 0, -speed, 1);
	}
	else if (player.state == 3)
	{
		matRotY = 
			glm::rotate(glm::mat4(1), charctor.rotation.y, glm::vec3(0, 1, 0));
		const glm::vec3 vecRotY = matRotY * glm::vec4(0, 1, -2.5f, 1);
		bullet->Initialize(meshId, texGreen.Get(), 1, 2,
			charctor.position + vecRotY, charctor.rotation, glm::vec3(3));
		bullet->velocity = matRotY * glm::vec4(0, 2, -3, 1);
	}
	else if (player.state == 0)
	{
	}
	
	bullet->colLocal =
	{ glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(1, 1, 1) };
	return 1.0f / 8.0f;
}

float MainGameScene::EnemyBulletLoad(Actor* bullet, Actor charctor, float speed)
{
	const int meshId = 7; //弾のメッシュID
	glm::mat4 matRotY =
		glm::rotate(glm::mat4(1), charctor.rotation.y, glm::vec3(0, 1, 0));
	if (waterEnemy.state == WaterEnemy::State::disturbAttack)
	{
		if (waterEnemy.type == WaterEnemy::Type::fire)
		{
			bullet->Initialize(meshId, texRed.Get(), 1, 0,
				charctor.position + glm::vec3(0, 1, 0), charctor.rotation, glm::vec3(1.5f));
		}
		else if (waterEnemy.type == WaterEnemy::Type::water)
		{
			bullet->Initialize(meshId, texBlue.Get(), 1, 0,
				charctor.position + glm::vec3(0, 1, 0), charctor.rotation, glm::vec3(1.5f));
		}
		else if (waterEnemy.type == WaterEnemy::Type::wind)
		{
			bullet->Initialize(meshId, texGreen.Get(), 1, 0,
				charctor.position + glm::vec3(0, 1, 0), charctor.rotation, glm::vec3(1.5f));
		}
		bullet->velocity = matRotY * glm::vec4(0, 2, -3, 1);
	}
	if (waterEnemy.state == WaterEnemy::State::midlleAttack)
	{
		enemyRotCount += 1;
		enemyRot += glm::radians(15.0f);
		if (waterEnemy.type == WaterEnemy::Type::fire)
		{
			bullet->Initialize(meshId, texRed.Get(), 1, 0,
				charctor.position + glm::vec3(0, 1, 0), charctor.rotation, glm::vec3(1));
		}
		else if (waterEnemy.type == WaterEnemy::Type::water)
		{
			bullet->Initialize(meshId, texBlue.Get(), 1, 0,
				charctor.position + glm::vec3(0, 1, 0), charctor.rotation, glm::vec3(1));
		}
		else if (waterEnemy.type == WaterEnemy::Type::wind)
		{
			bullet->Initialize(meshId, texGreen.Get(), 1, 0,
				charctor.position + glm::vec3(0, 1, 0), charctor.rotation, glm::vec3(1));
		}
		//ターゲット(プレイヤー)の方向を調べる
		matRotY = glm::rotate(glm::mat4(1), charctor.rotation.y + enemyRot, glm::vec3(0, 1, 0));
		bullet->velocity = matRotY * glm::vec4(0, 0, -speed, 1);
	}
	else if (waterEnemy.state == WaterEnemy::State::farAttack)
	{
		enemyRotCount += 1;
		enemyRot += glm::radians(90.0f);
		bullet->Initialize(meshId, texBlue.Get(), 1, 0,
			charctor.position + glm::vec3(0, 1, 0), charctor.rotation, glm::vec3(1));
		matRotY = glm::rotate(glm::mat4(1), charctor.rotation.y + enemyRot, glm::vec3(0, 1, 0));
		bullet->velocity = matRotY * glm::vec4(0, 0, -speed, 1);
	}

	bullet->colLocal =
	{ glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(1, 1, 1) };
	return 1.0f / 8.0f;
}

void MainGameScene::FixCollision(Actor& ch, Actor& obj)
{
	//Maxは直方体の右上を指す
	//Minは直方体の左下を指す
	ch.colMax = ch.colWorld.origin + ch.colWorld.size;
	ch.colMin = ch.colWorld.origin;
	obj.colMax = obj.colWorld.origin + obj.colWorld.size;
	obj.colMin = obj.colWorld.origin;

	//objとchの差を計算する
	float dx1 = obj.colMin.x - ch.colMax.x;
	float dx2 = obj.colMax.x - ch.colMin.x;
	float dy1 = obj.colMin.y - ch.colMax.y;
	float dy2 = obj.colMax.y - ch.colMin.y;
	float dz1 = obj.colMin.z - ch.colMax.z;
	float dz2 = obj.colMax.z - ch.colMin.z;

	// 絶対値が小さい方をセットする
	float dx = (glm::abs(dx1) < glm::abs(dx2)) ? dx1 : dx2;
	float dy = (glm::abs(dy1) < glm::abs(dy2)) ? dy1 : dy2;
	float dz = (glm::abs(dz1) < glm::abs(dz2)) ? dz1 : dz2;

	// x/y/zのうち最も差が小さいほうで位置を調整
	if (glm::abs(dx) <= glm::abs(dy) &&
		glm::abs(dx) <= glm::abs(dz))
	{
		ch.position.x += dx;
	}
	else if (glm::abs(dz) <= glm::abs(dx) &&
		glm::abs(dz) <= glm::abs(dy))
	{
		ch.position.z += dz;
	}
	else if (glm::abs(dy) <= glm::abs(dx) &&
		glm::abs(dy) <= glm::abs(dz))
	{
		ch.position.y += dy;
	}
}

/*
初期化
*/
bool MainGameScene::Initialize()
{
	/*
	const GLuint vbo = CreateVBO(sizeof(vetices), vetices);
	const GLuint ibo = CreateIBO(sizeof(indices), indices);
	const GLuint vao = CreateVAO(vbo, ibo);
	MeshList meshList;
	*/
	random.seed(std::random_device()());

	if (!meshList.Allocate())
	{
		return false;
	}
	progSimple.Reset(Shader::BuildFromFile("Res/Simple.vert", "Res/Simple.frag"));
	progLighting.Reset(Shader::BuildFromFile("Res/FragmentLighting.vert", "Res/FragmentLighting.frag"));
	
	//テクスチャを作成する
	const int imageWidth = 8;
	const int imageHeight = 8;
	const uint32_t B = 0xff'00'00'00;
	const uint32_t W = 0xff'ff'ff'ff;
	const uint32_t imageData[imageWidth * imageHeight] =
	{
		W, W, B, W, W, W, W, W,
		W, W, B, W, W, W, W, W,
		W, W, B, W, W, W, W, W,
		B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, W,
		W, W, W, W, W, W, B, W,
		W, W, W, W, W, W, B, W,
		B, B, B, B, B, B, B, B,
	};

	const uint32_t imageData2[imageWidth * imageHeight] =
	{
		W, W, W, W, W, W, W, W,
		B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W,
		B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W,
		B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W,
		B, B, B, B, B, B, B, B,
	};

	Texture::Image2D texId(Texture::CreateImage2D(imageWidth, imageHeight,
		imageData, GL_RGBA, GL_UNSIGNED_BYTE));
	Texture::Image2D texId2(Texture::CreateImage2D(imageWidth, imageHeight,
		imageData2, GL_RGBA, GL_UNSIGNED_BYTE));
	texHouse.Reset(Texture::LoadImage2D("Res/tga/House.tga"));
	texHuman.Reset(Texture::LoadImage2D("Res/tga/Human.tga"));
	texGround.Reset(Texture::LoadImage2D("Res/tga/Rock.tga"));
	texBullet.Reset(Texture::LoadImage2D("Res/tga/Bullet2.tga"));
	texRock.Reset(Texture::LoadImage2D("Res/tga/Rock.tga"));
	texZombie.Reset(Texture::LoadImage2D("Res/tga/Zombie.tga"));
	texSphere.Reset(Texture::LoadImage2D("Res/tga/Sphere.tga"));
	texRed.Reset(Texture::LoadImage2D("Res/tga/Red.tga"));
	texBlue.Reset(Texture::LoadImage2D("Res/tga/Blue.tga"));
	texGreen.Reset(Texture::LoadImage2D("Res/tga/Green.tga"));
	texAttribute.Reset(Texture::LoadImage2D("Res/tga/zokuseipng.tga"));
	texNone.Reset(Texture::LoadImage2D("Res/tga/mu.tga"));
	texFire.Reset(Texture::LoadImage2D("Res/tga/hi.tga"));
	texWater.Reset(Texture::LoadImage2D("Res/tga/mizu.tga"));
	texGrass.Reset(Texture::LoadImage2D("Res/tga/w.tga"));
	texTitle_B.Reset(Texture::LoadImage2D("Res/tga/title_B.tga"));
	texTitle_Y.Reset(Texture::LoadImage2D("Res/tga/title_Y.tga"));
	texRetry_B.Reset(Texture::LoadImage2D("Res/tga/retry_B.tga"));
	texRetry_Y.Reset(Texture::LoadImage2D("Res/tga/retry_Y.tga"));
	//情報表示用テクスチャを読み込む
	texScore.Reset(Texture::LoadImage2D("Res/tga/Score.tga"));
	for (int i = 0; i < 10; i++)
	{
		//ナンバーを読み込む
		const std::string filename =
			std::string("Res/tga/Number_") + (char)('0' + i) + ".tga";
		texNumber[i].Reset(Texture::LoadImage2D(filename.c_str()));
	}
	texHP.Reset(Texture::LoadImage2D("Res/tga/HP.tga"));
	texStageClear.Reset(Texture::LoadImage2D("Res/tga/StageClear.tga"));
	texGameOver.Reset(Texture::LoadImage2D("Res/tga/GameOver.tga"));
	texMouse.Reset(Texture::LoadImage2D("Res/tga/mouse32.tga"));
	texPouse.Reset(Texture::LoadImage2D("Res/tga/pouse.tga"));
	texGauge.Reset(Texture::LoadImage2D("Res/tga/gauge.tga"));
	tex100.Reset(Texture::LoadImage2D("Res/tga/100.tga"));
	tex50.Reset(Texture::LoadImage2D("Res/tga/50.tga"));
	tex25.Reset(Texture::LoadImage2D("Res/tga/25.tga"));
	texZokusei.Reset(Texture::LoadImage2D("Res/tga/zokuseihyou.tga"));
	texFireImage.Reset(Texture::LoadImage2D("Res/tga/hiimage.tga"));
	texWaterImage.Reset(Texture::LoadImage2D("Res/tga/mizuimage.tga"));
	texGrassImage.Reset(Texture::LoadImage2D("Res/tga/kiimage.tga"));

	if (texId.IsNull())
	{
		return 1;
	}

	//オーディオ
	

	//ライトの設定
	lights.ambient.color = glm::vec3(0.01f, 0.01f, 0.01f);  //環境光の色
	const glm::vec3 tmpSpotLightDir = glm::normalize(glm::vec3(-1, -2, -3));
	lights.spot.dirAndCutoff[0] = glm::vec4(tmpSpotLightDir, std::cos(glm::radians(20.0f)));
	lights.spot.posAndInnerCutOff[0] = glm::vec4(-6, 6, 8, std::cos(glm::radians(15.0f)));
	lights.spot.color[0] = glm::vec3(0.4f, 0.8f, 1.0f) * 200.0f;

	
	player.Initialize(4, texHuman.Get(), 10, 0,
		glm::vec3( 0.0f, 0.2f, 10.0f), glm::vec3(0), glm::vec3(1));
	playerBullet.Initialize();
	
	floors.Initialize(5, texGround.Get(), 1, 0,
		glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(-1.0f, 1, -1.0f));
	floors.colLocal =
	{
		glm::vec3(-20.0f,0.0f,-20.0f), glm::vec3(40.0f, 0.1f, 40.0f)
	};
	for (int i = 0; i < 4; i++)
	{
		floor[i].Initialize(5, texGround.Get(), 1, 0,
			floorPos[i], glm::vec3(glm::radians(90.0f), floorRot, 0), glm::vec3(1, 1, 0.5f));
		floor[i].colLocal = 
		{
			collisionMin[i], collisionMax[i]
		};
		floorRot += glm::radians(90.0f);
	}
	cube.Initialize(9, texSphere.Get(), 1, 0,
		glm::vec3(10), glm::vec3(0), glm::vec3(0));
	cube.colLocal =
	{
		glm::vec3(-0.5f,0,-0.5f), glm::vec3(1.0f)
	};
	redObject.Initialize(7, texRed.Get(), 1, 0,
		glm::vec3(0.0f,0.5f,-10.0f), glm::vec3(0), glm::vec3(1));
	lights.point.position[3] = redObject.position;
	lights.point.color[3] = glm::vec3(1.0f, 0.0f, 0.0f) * 100.0f;

	greenObject.Initialize(7, texGreen.Get(), 1, 0,
		glm::vec3(-10.0f, 0.5f, 0.0f), glm::vec3(0), glm::vec3(1));
	lights.point.position[1] = greenObject.position;
	lights.point.color[1] = glm::vec3(0.0f, 1.0f, 0.0f) * 50.0f;

	blueObject.Initialize(7, texBlue.Get(), 1, 0,
		glm::vec3(10.0f, 0.5f, 0.0f), glm::vec3(0), glm::vec3(1));
	lights.point.position[2] = blueObject.position;
	lights.point.color[2] = glm::vec3(0.0f, 0.0f, 1.0f) * 50.0f;

	waterEnemy.LocalInitialize();
	w_enemies.resize(128);
	for (auto& waterEnemy : w_enemies)
	{
		waterEnemy = new WaterEnemy;
	}
	Shild.Initialize(7, texBlue.Get(), 3, 0,
		glm::vec3(0.0f, 2.5f, 1.0f), glm::vec3(0), glm::vec3(2, 3, 1));
	
	redShild.Initialize(7, texBlue.Get(), 3, 0,
		glm::vec3(0.0f, 2.5f, 1.0f), glm::vec3(0), glm::vec3(2, 3, 1));

	blueShild.Initialize(7, texBlue.Get(), 3, 0,
		glm::vec3(0.0f, 2.5f, 1.0f), glm::vec3(0), glm::vec3(2, 3, 1));

	greenShild.Initialize(7, texBlue.Get(), 3, 0,
		glm::vec3(0.0f, 2.5f, 1.0f), glm::vec3(0), glm::vec3(2, 3, 1));

	enemies.resize(128);
	for (auto& zombie : enemies)
	{
		zombie = new ZombieActor;
	}
	enemyBullets.resize(128);
	for (auto& enemyBullet : enemyBullets)
	{
		enemyBullet = new EnemyBulletActor;
	}
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	return true;
}

/*
入力の反映
*/
void MainGameScene::ProcessInput()
{
	player.v = waterEnemy.position - player.position;
	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const GamePad gamepad = window.GetGamePad();
	if (state == State::play)
	{
		const float speed = 40.0f; //弾の移動速度(m/秒)
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			NextScene("End");
		}

		//player.ProcessInput();
 		//playerBullet.ProcessInput();

		//ショットボタンが押されていたら弾を発射
		if (window.IsKeyPressed(GLFW_KEY_SPACE) || gamepad.buttons & GamePad::R)
		{
 			if (BulletTimer <= 0)
			{
				Actor* p_Bullet = FindAvailableActor(playerBullet.playerBullets);
				if (p_Bullet)
				{
					if (player.state == 1)
					{
						BulletLoad(p_Bullet, player, speed);
						if (RotCount >= 5)
						{
							BulletTimer = 1.0f;
						}
					}
					else if (player.state == 2)
					{
						BulletLoad(p_Bullet, player, speed);
						BulletTimer = 0.2f;
					}
					else if (player.state == 3)
					{
						BulletLoad(p_Bullet, player, speed);
						BulletTimer = 3.0f;
					}
				}
			}
		}
		if (window.IsKeyPressed(GLFW_KEY_1))
		{
			state = State::pouse;
		}

	}
	else if (state == State::stageClear)
	{
		player.velocity.x = player.velocity.z = 0;
		if (window.IsKeyPressed(GLFW_KEY_LEFT) || gamepad.buttons & GamePad::DPAD_LEFT)
		{
			over = GameOver::title;
		}
		if (window.IsKeyPressed(GLFW_KEY_RIGHT) || gamepad.buttons & GamePad::DPAD_RIGHT)
		{
			over = GameOver::retry;
		}
		if (window.IsKeyPressed(GLFW_KEY_ENTER) || gamepad.buttons & GamePad::A)
		{
			if (over == GameOver::title)
			{
				NextScene("TitleScene");
			}
			else if (over == GameOver::retry)
			{
				NextScene("MainGameScene");
			}
		}
	}
	else if (state == State::pouse)
	{
		if (window.IsMousePressed(GLFW_MOUSE_BUTTON_LEFT) || gamepad.buttons & GamePad::DPAD_LEFT)
		{
			
		}
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE) || gamepad.buttons & GamePad::START)
		{
			state = State::play;
		}
	}
	else if(state == State::gameOver)
	{
		player.velocity.x = player.velocity.z = 0;
		if (window.IsKeyPressed(GLFW_KEY_LEFT) || gamepad.buttons & GamePad::DPAD_LEFT)
		{
			over = GameOver::title;
		}
		if (window.IsKeyPressed(GLFW_KEY_RIGHT) || gamepad.buttons & GamePad::DPAD_RIGHT)
		{
			over = GameOver::retry;
		}
		if (window.IsKeyPressed(GLFW_KEY_ENTER) || gamepad.buttons & GamePad::A)
		{
			if (over == GameOver::title)
			{
				NextScene("TitleScene");
			}
			else if (over == GameOver::retry)
			{
				NextScene("MainGameScene");
			}
		}
	}
}

/*
状態の更新
*/
void MainGameScene::Update()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	//ShowCursor(FALSE);
	if (player.position.z <= 0)
	{
		lights.directional.direction = glm::normalize(glm::vec3(0, -3, 2));  //指向性ライト
		lights.directional.color = glm::vec3(1.0, 1.0f, 1.0f); //指向性ライトの色
	}
	else if (player.position.z > 0)
	{
		lights.directional.direction = glm::normalize(glm::vec3(0, -3, -2));  //指向性ライト
		lights.directional.color = glm::vec3(1.0, 1.0f, 1.0f); //指向性ライトの色
	}
	
	if (state == State::pouse)
	{
		//glutPassiveMotionFunc();
		return;
	}
	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();

	mousePos = window.CursorPos();
	
	Actor* e_Bullet = FindAvailableActor(enemyBullets);
	if (enemyBullet.Timer <= 0)
	{
		if (e_Bullet)
		{
			if (waterEnemy.state == WaterEnemy::State::midlleAttack)
			{
				EnemyBulletLoad(e_Bullet, waterEnemy, 20.0f);
				if (enemyRotCount >= 24)
				{
					enemyBullet.Timer = 5.0f;
					waterEnemy.attackTimer = 5.0f;
					enemyRot = glm::radians(-45.0f);
					waterEnemy.state = WaterEnemy::State::wait;
				}
			}
			else if (waterEnemy.state == WaterEnemy::State::farAttack)
			{
				//EnemyBulletLoad(e_Bullet, waterEnemy, 20.0f);
				if (enemyRotCount >= 2)
				{
					enemyBullet.Timer = 5.0f;
					enemyRot = glm::radians(-45.0f);
				}
			}
			else if (waterEnemy.state == WaterEnemy::State::disturbAttack)
			{
				EnemyBulletLoad(e_Bullet, waterEnemy, 10.0f);
				enemyBullet.Timer = 5.0f;
			}
		}
	}
	glm::vec3 objectVec;
	if (waterEnemy.type == WaterEnemy::Type::fire)
	{
		objectVec = blueObject.position - e_Bullet->position;
	}
	else if (waterEnemy.type == WaterEnemy::Type::water)
	{
		objectVec = greenObject.position - e_Bullet->position;
	}
	else if (waterEnemy.type == WaterEnemy::Type::wind)
	{
		objectVec = redObject.position - e_Bullet->position;
	}
	const float objectLength = glm::length(objectVec);
	if (waterEnemy.state == WaterEnemy::State::disturbAttack &&
		objectLength >= (glm::length(waterEnemy.objectVec) / 2.0f))
	{
		e_Bullet->velocity -= glm::vec3(0, 5, 0);
	}
	
	//プレイヤーの弾の発射タイマーを更新
	if (BulletTimer > 0)
	{
		BulletTimer -= deltaTime;
		RotCount = 0;
	}
	if (enemyBullet.Timer > 0)
	{
		enemyBullet.Timer -= deltaTime;
		enemyRotCount = 0;
	}
	for (int i = 0; i < zombie.Num; i++)
	{
		if (zombies2[i])
		{
			if (zombies2[i]->bulletTimer > 0)
			{
				zombies2[i]->bulletTimer -= deltaTime;
			}
		}

	}
	waterEnemy.playerPosition = player.position;
	waterEnemy.playerRotation = player.rotation;
	if (waterEnemy.state == WaterEnemy::State::wait || waterEnemy.state == WaterEnemy::State::disturbAttack)
	{
		waterEnemy.v = player.position - waterEnemy.position;
		waterEnemy.rushPos = player.position;
		if (waterEnemy.type == WaterEnemy::Type::fire)
		{
			waterEnemy.objectVec = blueObject.position - waterEnemy.position;
			waterEnemy.ObjectToPlayerVec = blueObject.position - player.position;
		}
		else if (waterEnemy.type == WaterEnemy::Type::water)
		{
			waterEnemy.objectVec = greenObject.position - waterEnemy.position;
			waterEnemy.ObjectToPlayerVec = greenObject.position - player.position;
		}
		else if (waterEnemy.type == WaterEnemy::Type::wind)
		{
			waterEnemy.objectVec = redObject.position - waterEnemy.position;
			waterEnemy.ObjectToPlayerVec = redObject.position - player.position;
		}
	}

	//ゾンビの状態を更新
	UpdateActorList(enemies, deltaTime);
	UpdateActorList(w_enemies, deltaTime);

	//ゲームオーバー判定
	if (state == State::play && waterEnemy.helth <= 0)
	{
		state = State::stageClear;
	}
	if (state == State::play && player.helth <= 0)
	{
		state = State::gameOver;
	}
	red = &redObject;
	//プレイヤーの状態を更新する
	player.Update(deltaTime);

	waterEnemy.Update(deltaTime);
	cube.Update(deltaTime);
	floors.Update(deltaTime);
	enemyBullet.Update(deltaTime);
	playerBullet.Update(deltaTime);
	redObject.Update(deltaTime);
	greenObject.Update(deltaTime);
	blueObject.Update(deltaTime);
	

	for (int i = 0; i < 4; i++)
	{
		floor[i].Update(deltaTime);
	}

	if (waterEnemy.state == WaterEnemy::State::nearAttack)
	{
		shildLength += 5.0f * (deltaTime);
	}
	else
	{
		if (shildLength > 1.0f)
		{
			shildLength -= 5.0f * (deltaTime);
		}
		else if (shildLength <= 1.0f)
		{
			shildLength = 1.0f;
		}
	}

	const glm::mat4 PlayerDirRotY = glm::rotate(glm::mat4(1), waterEnemy.rotation.y, glm::vec3(0, 1, 0));
	p_dir = Shild.position - waterEnemy.position;             //曲がる前プレイヤーと矢印間のベクトル
	p_dirTarget = PlayerDirRotY * glm::vec4(0, 0.5f, -shildLength, 1);  //曲がった後のプレイヤーと矢印間ベクトル
	p_dirVeloctiy = p_dirTarget - p_dir;                      //曲がる前と曲がった後のベクトル
	p_dir += p_dirVeloctiy;                                   //その差分進める
	Shild.position = waterEnemy.position + p_dir;       //矢印の位置を調整
	Shild.rotation.y = waterEnemy.rotation.y + glm::radians(180.0f);

	p_dirTarget = PlayerDirRotY * glm::vec4(0, 0.5f, shildLength, 1);  //曲がった後のプレイヤーと矢印間ベクトル
	p_dirVeloctiy = p_dirTarget - p_dir;                      //曲がる前と曲がった後のベクトル
	p_dir += p_dirVeloctiy;                                   //その差分進める
	redShild.position = waterEnemy.position + p_dir;       //矢印の位置を調整
	redShild.rotation.y = waterEnemy.rotation.y + glm::radians(-180.0f);

	p_dirTarget = PlayerDirRotY * glm::vec4(shildLength, 0.5f, 0, 1);  //曲がった後のプレイヤーと矢印間ベクトル
	p_dirVeloctiy = p_dirTarget - p_dir;                      //曲がる前と曲がった後のベクトル
	p_dir += p_dirVeloctiy;                                   //その差分進める
	blueShild.position = waterEnemy.position + p_dir;       //矢印の位置を調整
	blueShild.rotation.y = waterEnemy.rotation.y + glm::radians(90.0f);

	p_dirTarget = PlayerDirRotY * glm::vec4(-shildLength, 0.5f, 0, 1);  //曲がった後のプレイヤーと矢印間ベクトル
	p_dirVeloctiy = p_dirTarget - p_dir;                      //曲がる前と曲がった後のベクトル
	p_dir += p_dirVeloctiy;                                   //その差分進める
	greenShild.position = waterEnemy.position + p_dir;       //矢印の位置を調整
	greenShild.rotation.y = waterEnemy.rotation.y + glm::radians(-90.0f);

	Shild.Update(deltaTime);
	redShild.Update(deltaTime);
	blueShild.Update(deltaTime);
	greenShild.Update(deltaTime);
	
	//プレイヤーの弾の状態を更新
	UpdateActorList(playerBullet.playerBullets, deltaTime);
	UpdateActorList(enemyBullets, deltaTime);
	UpdateActorList(redObject.reds, deltaTime);

	//プレイヤーの弾と敵の衝突判定
	/*
	「[&](Actor&, Actor&){ ~ }」の部分は「無名関数」あるいは「ラムタ式」と呼ばれる
	関数を定義しなくても、必要な場所に直接関数の内容を描くことが出来る
	*/
	
	CharactorCollision(player, enemyBullets, [&](Actor& bullet, Actor& zombie)
	{
		if (waterEnemy.type == WaterEnemy::Type::fire && player.state == 1)
		{
			if (player.helth < 10)
			{
				player.helth += 1;
				playerGauage += 12.6f;
			}
		}
		else if (waterEnemy.type == WaterEnemy::Type::water && player.state == 2)
		{
			if (player.helth < 10)
			{
				player.helth += 1;
				playerGauage += 12.6f;
			}
		}
		else if (waterEnemy.type == WaterEnemy::Type::wind && player.state == 3)
		{
			if (player.helth < 10)
			{
				player.helth += 1;
				playerGauage += 12.6f;
			}
		}
		else
		{
			if (waterEnemy.type == WaterEnemy::Type::fire)
			{
				player.state = 1;
				player.helth -= 1;
				playerGauage -= 12.6f;
			}
			else if (waterEnemy.type == WaterEnemy::Type::water)
			{
				player.state = 2;
				player.helth -= 1;
				playerGauage -= 12.6f;
			}
			else if (waterEnemy.type == WaterEnemy::Type::wind)
			{
				player.state = 3;
				player.helth -= 1;
				playerGauage -= 12.6f;
			}
		}
		zombie.helth = 0;
	});

	ActorCollision(player, redObject, [&](Actor& charctor, Actor& ground)
	{
		
		redObject.helth = 0;
		red = nullptr;
		player.state = 1;
	});
	ActorCollision(player, blueObject, [&](Actor& charctor, Actor& ground)
	{
		blueObject.helth = 0;
		player.state = 2;
	});
	ActorCollision(player, greenObject, [&](Actor& charctor, Actor& ground)
	{
		greenObject.helth = 0;
		player.state = 3;
	});

	CharactorCollision(waterEnemy, playerBullet.playerBullets, [&](Actor& enemy, Actor& bullet)
	{
		if (waterEnemy.type == WaterEnemy::Type::fire && player.state == 2)
		{
			waterEnemy.helth -= 1;
			enemyGauage -= 1.26f;
		}
		else if (waterEnemy.type == WaterEnemy::Type::water && player.state == 3)
		{
			enemyGauage -= 12.6f;
			waterEnemy.helth -= 10;
		}
		else if (waterEnemy.type == WaterEnemy::Type::wind && player.state == 1)
		{
			waterEnemy.helth -= 10;
			enemyGauage -= 12.6f;
		}

		if (waterEnemy.type == WaterEnemy::Type::fire && player.state == 1)
		{
			if (waterEnemy.helth < 100)
			{
				waterEnemy.helth += 1;
				enemyGauage += 1.26f;
			}
		}
		else if (waterEnemy.type == WaterEnemy::Type::water && player.state == 2)
		{
			if (waterEnemy.helth < 100)
			{
				enemyGauage += 12.6f;
				waterEnemy.helth += 10;
			}
			
		}
		else if (waterEnemy.type == WaterEnemy::Type::wind && player.state == 3)
		{
			if (waterEnemy.helth < 100)
			{
				waterEnemy.helth += 10;
				enemyGauage += 12.6;
			}
		}
		if (waterEnemy.helth >= 100)
		{
			waterEnemy.helth = 100;
			enemyGauage = -250;
		}
		waterEnemy.Hit = true;
		bullet.helth = 0;
	});

	DetectCollision(w_enemies, playerBullet.playerBullets, [&](Actor& zombie, Actor& bullet)
	{
		if (bullet.state == 1 && zombie.state == 3)
		{
			zombie.helth = 0;
		}
		else if (bullet.state == 2 && zombie.state == 1)
		{
			zombie.helth = 0;
		}
		else if (bullet.state == 3 && zombie.state == 2)
		{
			zombie.helth = 0;
		}
		bullet.helth = 0;
	});


	ActorCollision(player, floors, [&](Actor& charctor, Actor& ground)
	{
		FixCollision(player, floors);
	});

	for (int i = 0; i < 4; i++)
	{
		ActorCollision(player, floor[i], [&](Actor& charctor, Actor& ground)
		{
			FixCollision(player, floor[i]);
		});
		ActorCollision(waterEnemy, floor[i], [&](Actor& charctor, Actor& ground)
		{
			FixCollision(waterEnemy, floor[i]);
			if (waterEnemy.state == WaterEnemy::State::nearAttack ||
				waterEnemy.state == WaterEnemy::State::farAttack)
			{
				waterEnemy.state = WaterEnemy::State::wait;
			}
		});
	}
	ActorCollision(waterEnemy, floors, [&](Actor& charctor, Actor& ground)
	{
		FixCollision(waterEnemy, floors);
	});

	ActorCollision(player, waterEnemy, [&](Actor& charctor, Actor& ground)
	{
		FixCollision(player, waterEnemy);
		if (waterEnemy.state == WaterEnemy::State::farAttack)
		{
			if (damageTimer <= 0)
			{
				if (waterEnemy.type == WaterEnemy::Type::fire && player.state == 1)
				{
					if (player.helth < 10)
					{
						player.helth += 1;
						playerGauage += 12.6f;
					}
				}
				else if (waterEnemy.type == WaterEnemy::Type::water && player.state == 2)
				{
					if (player.helth < 10)
					{
						player.helth += 1;
						playerGauage += 12.6f;
					}
				}
				else if (waterEnemy.type == WaterEnemy::Type::wind && player.state == 3)
				{
					if (player.helth < 10)
					{
						player.helth += 1;
						playerGauage += 12.6f;
					}
				}
				else
				{
					if (waterEnemy.type == WaterEnemy::Type::fire)
					{
						player.state = 1;
						player.helth -= 1;
						playerGauage -= 12.6f;
					}
					else if (waterEnemy.type == WaterEnemy::Type::water)
					{
						player.state = 2;
						player.helth -= 1;
						playerGauage -= 12.6f;
					}
					else if (waterEnemy.type == WaterEnemy::Type::wind)
					{
						player.state = 3;
						player.helth -= 1;
						playerGauage -= 12.6f;
					}
				}
				damageTimer = 5.0f;
			}
			player.knockBack = true;
			player.back = true;
			
		}
	});

	ActorCollision(Shild, player, [&](Actor& charctor, Actor& ground)
	{
		if (waterEnemy.state == WaterEnemy::State::nearAttack)
		{
			player.helth -= 1;
			playerGauage -= 12.6f;
			player.knockBack = true;
			player.back = true;
		}
		if(waterEnemy.state != WaterEnemy::State::farAttack &&
			glm::length(waterEnemy.v) <= 2.0f)
		{
			FixCollision(player, Shild);
		}
	});

	ActorCollision(redShild, player, [&](Actor& charctor, Actor& ground)
	{
		FixCollision(player, redShild);
		if (waterEnemy.state == WaterEnemy::State::nearAttack)
		{
			player.helth -= 1;
			playerGauage -= 12.6f;
			player.knockBack = true;
			player.back = true;
		}
	});

	ActorCollision(blueShild, player, [&](Actor& charctor, Actor& ground)
	{
		FixCollision(player, blueShild);
		if (waterEnemy.state == WaterEnemy::State::nearAttack)
		{
			player.helth -= 1;
			playerGauage -= 12.6f;
			player.knockBack = true;
			player.back = true;
		}
	});
	ActorCollision(greenShild, player, [&](Actor& charctor, Actor& ground)
	{
		FixCollision(player, greenShild);
		if (waterEnemy.state == WaterEnemy::State::nearAttack)
		{
			player.helth -= 1;
			playerGauage -= 12.6f;
			player.knockBack = true;
			player.back = true;
		}
	});

	CharactorCollision(Shild, playerBullet.playerBullets, [&](Actor& charctor, Actor& bullet)
	{
		Shild.helth -= 1;
		bullet.helth = 0;
	});
	CharactorCollision(redShild, playerBullet.playerBullets, [&](Actor& charctor, Actor& bullet)
	{
		redShild.helth -= 1;
		bullet.helth = 0;
	});
	CharactorCollision(blueShild, playerBullet.playerBullets, [&](Actor& charctor, Actor& bullet)
	{
		blueShild.helth -= 1;
		bullet.helth = 0;
	});
	CharactorCollision(greenShild, playerBullet.playerBullets, [&](Actor& charctor, Actor& bullet)
	{
		greenShild.helth -= 1;
		bullet.helth = 0;
	});
	ActorCollision(cube, floors, [&](Actor& charctor, Actor& ground)
	{
		FixCollision(cube, floors);
	});
	
	damageTimer -= deltaTime;
}

void MainGameScene::RenderLoad()
{
	progLighting.SetLightList(lights);
	meshList.BindVertexArray();
	progLighting.BindTexture(0, player.texture);
	progLighting.Draw(meshList.Get(player.mesh),
		player.position, player.rotation, player.scale);
	progLighting.BindTexture(0, redObject.texture);
	progLighting.Draw(meshList.Get(redObject.mesh),
		redObject.position, redObject.rotation, redObject.scale);
	progLighting.BindTexture(0, blueObject.texture);
	progLighting.Draw(meshList.Get(blueObject.mesh),
		blueObject.position, blueObject.rotation, blueObject.scale);
	progLighting.BindTexture(0, greenObject.texture);
	progLighting.Draw(meshList.Get(redObject.mesh),
		greenObject.position, greenObject.rotation, greenObject.scale);

	progLighting.BindTexture(0, texBullet.Get());
	if (redShild.helth > 0)
	{
		progLighting.Draw(meshList.Get(redShild.mesh),
			redShild.position, redShild.rotation, redShild.scale);
	}
	if (blueShild.helth > 0)
	{
		progLighting.Draw(meshList.Get(blueShild.mesh),
			blueShild.position, blueShild.rotation, blueShild.scale);
	}
	if (greenShild.helth > 0)
	{
		progLighting.Draw(meshList.Get(greenShild.mesh),
			greenShild.position, greenShild.rotation, greenShild.scale);
	}
	if (Shild.helth > 0)
	{
		progLighting.Draw(meshList.Get(Shild.mesh),
			Shild.position, Shild.rotation, Shild.scale);
	}
	

	progLighting.BindTexture(0,cube.texture);
	progLighting.Draw(meshList.Get(cube.mesh),
		cube.position, cube.rotation, cube.scale);
	
	
	RenderActorList(redObject.reds, progLighting, meshList);
	RenderActorList(playerBullet.playerBullets, progLighting, meshList);
	RenderActorList(enemies, progLighting, meshList);
	//RenderActorList(w_enemies, progLighting, meshList);
	RenderActorList(enemyBullets, progLighting, meshList);

	if (waterEnemy.type == WaterEnemy::Type::fire)
	{
		progLighting.BindTexture(0, texRed.Get());
	}
	else if (waterEnemy.type == WaterEnemy::Type::water)
	{
		progLighting.BindTexture(0, texBlue.Get());
	}
	else if (waterEnemy.type == WaterEnemy::Type::wind)
	{
		progLighting.BindTexture(0, texGreen.Get());
	}
	progLighting.Draw(meshList.Get(waterEnemy.mesh),
		waterEnemy.position, waterEnemy.rotation, waterEnemy.scale);

	progLighting.BindTexture(0, texId.Get());

	

	const float treeCount = 10;
	const float houseCount = 2;
	const float radius = 8;
	const float radius2 = 16;

	//地面
	progLighting.BindTexture(0, texGround.Get());
	progLighting.Draw(meshList.Get(5), glm::vec3(0), glm::vec3(0), glm::vec3(1.2f));
}

/*
描画
*/
void MainGameScene::Render()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float deltaTime = (float)window.DeltaTime();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		progLighting.Use();

		//座標変換行列を作成してシェーダーに転送する
		const glm::mat4x4 matProj =
			//透視投影を行うための座標変換行列(プロジェクション変換)を作成している
			/*
			引数１・・・Y軸方向の視野角度
			引数２・・・X軸方向の視野角度を決めるための比率
			引数３・・・奥行きの描画範囲を設定する
			引数４・・・引数３と同じ
			*/
			glm::perspective(glm::radians(45.0f), 1280.0f /720.0f, 0.1f, 500.0f);

		const glm::mat4x4 matView =
			//ワールド座標系からビュー座標系への変換行列を作成している
			/*
			引数１・・・視点の座標
			引数２・・・注視点の座標
			引数３・・・視点の上方向のベクトル
			*/
			glm::lookAt(player.viewPos, player.targetPos, glm::vec3(0, 1, 0));

		progLighting.SetViewProjectionMatrix(matProj * matView);

		
		RenderLoad();
	}


	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		progSimple.Use();

		//正射影で描画するように行列を設定
		const glm::mat4x4 matProj = glm::ortho(
			-400.0f, 400.0f, -300.0f, 300.0f, 1.0f, 500.0f);
		const glm::mat2x4 matView = glm::lookAt(
			glm::vec3(0, 0, 100), glm::vec3(0), glm::vec3(0, 1, 0));
		progSimple.SetViewProjectionMatrix(matProj * matView);

		glViewport(0, 0, viewHeight, viewWidth);

		const Mesh planeMeshId = meshList.Get(6);

		

		//敵の体力を表示
		progSimple.BindTexture(0, texGauge.Get());
		progSimple.Draw(planeMeshId,
			glm::vec3(-250, 250, 0), glm::vec3(0), glm::vec3(128, 32, 1));
		float enemyHelth = waterEnemy.helth * 1.26f;
		if (waterEnemy.helth > 0)
		{
			if (waterEnemy.helth > 50)
			{
				progSimple.BindTexture(0, tex100.Get());
			}
			else if (waterEnemy.helth <= 50 && waterEnemy.helth > 20)
			{
				progSimple.BindTexture(0, tex50.Get());
			}
			else if (waterEnemy.helth <= 20)
			{
				progSimple.BindTexture(0, tex25.Get());
			}
			progSimple.Draw(planeMeshId,
				glm::vec3(enemyGauage, 250, 0), glm::vec3(0), glm::vec3(enemyHelth, 30, 1));
		}

		if (waterEnemy.type == WaterEnemy::Type::fire)
		{
			progSimple.BindTexture(0, texFireImage.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(-80, 250, 0), glm::vec3(0), glm::vec3(28, 28, 1));
		}
		else if (waterEnemy.type == WaterEnemy::Type::water)
		{
			progSimple.BindTexture(0, texWaterImage.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(-80, 250, 0), glm::vec3(0), glm::vec3(28, 28, 1));
		}
		else if (waterEnemy.type == WaterEnemy::Type::wind)
		{
			progSimple.BindTexture(0, texGrassImage.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(-80, 250, 0), glm::vec3(0), glm::vec3(28, 28, 1));
		}

		//プレイヤーの体力を表示
		progSimple.BindTexture(0, texGauge.Get());
		progSimple.Draw(planeMeshId,
			glm::vec3(-250, -250, 0), glm::vec3(0), glm::vec3(128, 32, 1));
		float playerHelth = player.helth * 12.6f;
		if (player.helth > 0)
		{
			if (player.helth > 5)
			{
				progSimple.BindTexture(0, tex100.Get());
			}
			else if (player.helth <= 5 && player.helth > 2)
			{
				progSimple.BindTexture(0, tex50.Get());
			}
			else if (player.helth <= 2)
			{
				progSimple.BindTexture(0, tex25.Get());
			}
			progSimple.Draw(planeMeshId,
				glm::vec3(playerGauage, -250, 0), glm::vec3(0), glm::vec3(playerHelth, 30, 1));
		}
		
		
		progSimple.BindTexture(0, texZokusei.Get());
		progSimple.Draw(planeMeshId,
			glm::vec3(-300, -50, 0), glm::vec3(0), glm::vec3(100, 100, 1));

		//属性を表示
		progSimple.BindTexture(0, texAttribute.Get());
		progSimple.Draw(planeMeshId,
			glm::vec3(-320, -170, 0), glm::vec3(0), glm::vec3(64, 64, 1));
		if (player.state == 0)
		{
			progSimple.BindTexture(0, texNone.Get());
		}
		else if (player.state == 1)
		{
			progSimple.BindTexture(0, texFire.Get());
		}
		else if (player.state == 2)
		{
			progSimple.BindTexture(0, texWater.Get());
		}
		else if (player.state == 3)
		{
			progSimple.BindTexture(0, texGrass.Get());
		}
		progSimple.Draw(planeMeshId,
			glm::vec3(-200, -170, 0), glm::vec3(0), glm::vec3(64, 64, 1));

		if (player.state == 1)
		{
			progSimple.BindTexture(0, texFireImage.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(-110, -165, 0), glm::vec3(0), glm::vec3(28, 28, 1));
		}
		else if (player.state == 2)
		{
			progSimple.BindTexture(0, texWaterImage.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(-110, -165, 0), glm::vec3(0), glm::vec3(28, 28, 1));
		}
		else if (player.state == 3)
		{
			progSimple.BindTexture(0, texGrassImage.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(-110, -165, 0), glm::vec3(0), glm::vec3(28, 28, 1));
		}
		
		
		
		

		//ステージクリア・ゲームオーバー表示
		if (state == State::stageClear || state == State::gameOver)
		{
			if (over == GameOver::title)
			{
				progSimple.BindTexture(0, texTitle_Y.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(-60, -60, 0), glm::vec3(0), glm::vec3(60, 60, 1));
				progSimple.BindTexture(0, texRetry_B.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(60, -60, 0), glm::vec3(0), glm::vec3(60, 60, 1));
			}
			else if (over == GameOver::retry)
			{
				progSimple.BindTexture(0, texTitle_B.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(-60, -60, 0), glm::vec3(0), glm::vec3(60, 60, 1));
				progSimple.BindTexture(0, texRetry_Y.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(60, -60, 0), glm::vec3(0), glm::vec3(60, 60, 1));
			}

		}
		if (state == State::stageClear)
		{
			progSimple.BindTexture(0, texStageClear.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(60,60,0), glm::vec3(0), glm::vec3(300, 60, 1));
		}
		else if (state == State::gameOver)
		{
			progSimple.BindTexture(0, texGameOver.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(60, 60, 0), glm::vec3(0), glm::vec3(300, 60, 1));
		}
		else if (state == State::pouse)
		{
			//progSimple.BindTexture(0, texPouse.Get());
			//progSimple.Draw(planeMeshId,
				//glm::vec3(0), glm::vec3(0), glm::vec3(60, 60, 1));
		}
	}
	progLighting.BindTexture(0, 0);
}

/*
終了
*/
void MainGameScene::Finalize()
{
	ClearActorList(playerBullets);
	ClearActorList(enemies);
	ClearActorList(enemyBullets);
}