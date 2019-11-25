#include "Player.h"

extern int Total;
extern glm::vec3 Pos;
glm::vec3 PlayerPos;
glm::vec3 PlayerRot;

void PlayerActor::Initialize(int mesh, GLuint tex, int hp, int state,
	const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
{
	colLocal =
	{
		glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(1.0f, 1.7f, 1.0f)
	};
	viewoffset = glm::vec3(0, 2, 3);
	targetoffset = glm::vec3(0, 0, -5);
	viewPos = glm::vec3(0, 4, 6) + pos; //�J�����̈ʒu
	targetPos = glm::vec3(0, 0, -5) + pos;//�����_�̈ʒu(��U�Œ�)
	PlayerPos = pos;
	PlayerRot = rot;
	
	Actor::Initialize(mesh, tex, hp, state, pos, rot, scale);
}

void PlayerActor::ProcessInput()
{
	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const GamePad gamepad = window.GetGamePad();
	
	const glm::mat4 pVelocityRotY =
		glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
	float pforwardSpeed = 0.0f;
	float prightSpeed = 0.0f;
	const float speed = 40.0f; //�e�̈ړ����x(m/�b)
	if (knockBack)
	{
		return;
	}
	
	velocity = glm::vec3(0);

	//�V���b�g�{�^����������Ă��Ȃ���Ε����]��
	if (!window.IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT) || !gamepad.buttons & GamePad::L)
	{
		if (window.IsKeyPressed(GLFW_KEY_A) || gamepad.buttons & GamePad::DPAD_LEFT)
		{
			rotation.y += glm::radians(90.0f) * deltaTime;
			
		}
		else if (window.IsKeyPressed(GLFW_KEY_D) || gamepad.buttons & GamePad::DPAD_RIGHT)
		{
			rotation.y += glm::radians(-90.0f) * deltaTime;
		}
		
	}
	else
	{
		if (window.IsKeyPressed(GLFW_KEY_A) || gamepad.buttons & GamePad::DPAD_RIGHT)
		{
			prightSpeed -= 1;
		}
		else if (window.IsKeyPressed(GLFW_KEY_D) || gamepad.buttons & GamePad::DPAD_LEFT)
		{
			prightSpeed += 1;
		}
	}

	if (window.IsKeyPressed(GLFW_KEY_W) || gamepad.buttons & GamePad::DPAD_UP)
	{
		pforwardSpeed -= 1;
		rotation.x = glm::radians(-30.0f);
	}
	else if (window.IsKeyPressed(GLFW_KEY_S) || gamepad.buttons & GamePad::DPAD_DOWN)
	{
		pforwardSpeed += 1;
	}
	else
	{
		rotation.x = glm::radians(0.0f);
	}

	if (pforwardSpeed < 0 || pforwardSpeed > 0)
	{
		if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT) && window.IsKeyPressed(GLFW_KEY_S))
		{
			/*
			std::atan...�ړ��x�N�g������������v�Z���邽�߂̊֐�
			������Y,X�̃x�N�g�����w�肷��ƁA���̃x�N�g���̌��������W�A���̒l��
			�Ԃ��Ă����
			*/
			rotation.y = std::atan2(-rotation.y, 180.0f);
			rotation.y -= glm::radians(180.0f);
		}

		const float speed = 10.0f;
		velocity = pVelocityRotY * glm::vec4(0, 0, pforwardSpeed, 1);
		velocity = glm::normalize(velocity);
		velocity *= speed;
	}
	if (prightSpeed < 0 || prightSpeed > 0)
	{
		const float speed = 5.0f;
		velocity = pVelocityRotY * glm::vec4(prightSpeed, 0, 0, 1);
		velocity = glm::normalize(velocity);
		velocity *= speed;
	}
	
	//�G�𒍎�����
	if (window.IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT) || gamepad.buttons & GamePad::L)
	{
		float radian;
		float vRadian;
		//�^�[�Q�b�g(�v���C���[)�̕����𒲂ׂ�
		const glm::vec3 vTarget = glm::normalize(v);
		radian = std::atan2(-vTarget.z, vTarget.x);

		const glm::vec3 vFront =
			glm::rotate(
				glm::mat4(1), rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, -1);
		const glm::vec3 playerFront = glm::normalize(vFront);
		vRadian = std::atan2(-playerFront.z, playerFront.x);
		const float front = radian - vRadian;

		if (front > 0)
		{
			rotation.y += glm::radians(360.0f) * deltaTime;
		}
		else if (front < 0)
		{
			rotation.y -= glm::radians(360.0f) * deltaTime;
		}

	}

	//�V���b�g�{�^����������Ă�����e�𔭎�
	if (window.IsKeyPressed(GLFW_KEY_SPACE))
	{
		
	}

}

/*
�v���C���[�̏�Ԃ��X�V����

@param deltaTime �o�ߎ���
*/
void PlayerActor::Update(float deltaTime)
{
	const glm::mat4 pVelocityRotY =
		glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
	float pforwardSpeed = 5.0f;
	//�v���C���[������ł�����A�n�ʂɓ|���(-90�x�ɂȂ�)�܂Łu�|���v��傫�����Ă���
	if (helth <= 0)
	{
		rotation.x -= glm::radians(45.0f) * deltaTime;
		if (rotation.x < glm::radians(-90.0f))
		{
			rotation.x = glm::radians(-90.0f);
		}
	}

	if (knockBack)
	{
		knockBackTimer += deltaTime;
		const float speed = 10.0f;
		velocity = pVelocityRotY * glm::vec4(0, 0, pforwardSpeed, 1);
		velocity = glm::normalize(velocity);
		velocity *= speed;

		if (back)
		{
			rotation.x += glm::radians(30.0f) * deltaTime;
			if (rotation.x < glm::radians(30.0f))
			{
				back = false;
				revers = true;
				rotation.x = glm::radians(30.0f);
			}
		}
		if (revers)
		{
			rotation.x -= glm::radians(30.0f) * deltaTime;
			if (rotation.x < glm::radians(0.0f))
			{
				revers = false;
				knockBack = false;
				rotation.x = glm::radians(0.0f);
			}
		}
		

		if (knockBackTimer >= 0.5f)
		{
			//knockBack = false;
			//knockBackTimer = 0.0f;
		}
	}

	
	PlayerActor::ProcessInput(); //���͂��ꂽ�l���X�V

	velocity.y -= Gravity;

	//Y��
	const glm::mat4 CameraRotY = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
	const glm::mat4 TargetRotY = glm::rotate(glm::mat4(1), mousePos.y, glm::vec3(0, 1, 0));
	const glm::mat4 TargetRotX = glm::rotate(glm::mat4(1), mousePos.x, glm::vec3(1, 0, 0));
	const glm::mat4 TargetRotXY = TargetRotX + TargetRotY;

	//�J����
	vCamera = viewPos - position;   //�Ȃ���O�̃v���C���[�ƃJ�����̃x�N�g��
	vCameraTraget = CameraRotY * glm::vec4(0, 4, 6, 1); //�Ȃ�������̃v���C���[�ƃJ�����x�N�g��
	cameraVelocity = vCameraTraget - vCamera; //�Ȃ���O�ƋȂ�������̃x�N�g��
	vCamera += cameraVelocity * (deltaTime * 5);    //���̃x�N�g�����AdeltaTime���i�߂���

	//�����_
	vTraget = targetPos - position; //�v���C���[�ƒ����_�̃x�N�g��
	vTragetoffset = CameraRotY * glm::vec4(0, 0, -5, 1);
	TragetVelocity = vTragetoffset - vTraget;
	vTraget += TragetVelocity * (deltaTime * 5);
	viewPos = position + vCamera;   //�J�����̈ʒu���X�V
	targetPos = position + vTraget; //�����_�̈ʒu���X�V

	PlayerPos = position;

	const float mapSize = 12;
	for (size_t i = 0; i < 3; ++i)
	{
		if (position[i] < -mapSize)
		{
			position[i] = mapSize;
		}
		if (position[i] > mapSize)
		{
			position[i] = -mapSize;
		}
	}


	//��ɕK�v�ȏ����͊��N���X���������Ă���
	Actor::Update(deltaTime);
}

void PlayerBullet::Initialize()
{
	PlayerBullet playerBullet;
	playerBullets.resize(128);
	for (auto& bullet : playerBullets)
	{
		bullet = new PlayerBullet;
	}
}

float PlayerBullet::BulletLoad(Actor* bullet, glm::vec3 pos, glm::vec3 rot, float speed)
{
	const int meshId = 7; //�e�̃��b�V��ID
	const glm::mat4 matRotY =
		glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
	texRed.Reset(Texture::LoadImage2D("Res/tga/Red.tga"));
	texBlue.Reset(Texture::LoadImage2D("Res/tga/Blue.tga"));
	texGreen.Reset(Texture::LoadImage2D("Res/tga/Green.tga"));
	texBullet.Reset(Texture::LoadImage2D("Res/tga/Bullet2.tga"));
	pos += glm::vec3(0, 5, 0);
	if (state == 0)
	{
		bullet->Initialize(meshId, texBullet.Get(), 1, 0,
			pos, rot, glm::vec3(1));
	}
	else if (state == 1)
	{
		bullet->Initialize(meshId, texRed.Get(), 1, 1,
			pos, rot, glm::vec3(1));
	}
	else if (state == 2)
	{
		bullet->Initialize(meshId, texBlue.Get(), 1, 2,
			pos, rot, glm::vec3(1));
	}
	else if (state == 3)
	{
		bullet->Initialize(meshId, texGreen.Get(), 1, 3,
			pos, rot, glm::vec3(1));
	}

	bullet->velocity = matRotY * glm::vec4(0, 0, -speed, 1);
	bullet->colLocal =
	{ glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(1, 1, 1) };
	return 1.0f / 8.0f;
}

void PlayerBullet::ProcessInput()
{
	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float speed = 40.0f; //�e�̈ړ����x(m/�b)
	PlayerActor player;
	
	//�V���b�g�{�^����������Ă�����e�𔭎�
	if (window.IsKeyPressed(GLFW_KEY_SPACE))
	{

		if (Timer <= 0)
		{
			Actor* p_Bullet = FindAvailableActor(playerBullets);
			if (p_Bullet)
			{
				BulletLoad(p_Bullet, PlayerPos, PlayerRot, speed);
				Timer = BulletLoad(p_Bullet, PlayerPos, PlayerRot, speed);
			}
		}
		else
		{
			Timer = 0.0f;
		}
	}
}

/*
�v���C���[�̒e�̏�Ԃ��X�V����
*/
void PlayerBullet::Update(float deltaTime)
{
	if (position.y >= 3)
	{
		velocity.y -= 0.1f;
	}
	
	//�}�b�v�̍L����20cm �~ 20cm�Ƃ��āA�e�����͈̔͂𒴂����������悤�ɂ���
	const float mapSize = 12;
	for (size_t i = 0; i < 3; ++i)
	{
		if (position[i] < -mapSize || position[i] > mapSize)
		{
			helth = 0;
			break;
		}
	}
	//�v���C���[�̒e�̔��˃^�C�}�[���X�V
	if (Timer > 0)
	{
		Timer -= deltaTime;
	}
	UpdateActorList(playerBullets, deltaTime);
	Actor::Update(deltaTime);
}