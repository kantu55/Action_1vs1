/*
@file TitleScene.cpp
*/
#include "TitleScene.h"
#include <glm/gtc/matrix_transform.hpp>

extern int Total;
extern int Left;

/*
������
*/
bool TitleScene::Initialize()
{
	if (!meshList.Allocate())
	{
		return false;
	}
	progSimple.Reset(Shader::BuildFromFile("Res/Simple.vert", "Res/Simple.frag"));

	texLogo.Reset(Texture::LoadImage2D("Res/tga/TitleLogo.tga"));
	texBackground.Reset(Texture::LoadImage2D("Res/tga/TitleBack.tga"));
	texStartButton_B.Reset(Texture::LoadImage2D("Res/tga/GameStart_B.tga"));
	texStartButton_Y.Reset(Texture::LoadImage2D("Res/tga/GameStart_Y.tga"));
	texGameEndButton_B.Reset(Texture::LoadImage2D("Res/tga/GameEnd_B.tga"));
	texGameEndButton_Y.Reset(Texture::LoadImage2D("Res/tga/GameEnd_Y.tga"));

	//��莞�ԓ��͂��󂯕t���Ȃ��悤�ɂ���
	timer = 1.0f;
	isFinish = false;

	Left = 1;
	Total = 1;

	return true;
}

/*
���͂̔��f
*/
void TitleScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const GamePad gamepad = window.GetGamePad();
	if (!isFinish && timer <= 0.0f)
	{
		//ENTER�L�[�ŏI��
		if (window.IsKeyPressed(GLFW_KEY_ENTER) || gamepad.buttons & GamePad::A)
		{
			if (title == Title::start)
			{
				NextScene("MainGameScene");
				isFinish = true;
			}
			else if (title == Title::end)
			{
				NextScene("End");
				isFinish = true;
			}
		}
		if (window.IsKeyPressed(GLFW_KEY_RIGHT) || gamepad.buttons & GamePad::DPAD_RIGHT)
		{
				title = Title::end;
		}
		if (window.IsKeyPressed(GLFW_KEY_LEFT) || gamepad.buttons & GamePad::DPAD_LEFT)
		{
			title = Title::start;
		}
		if (window.IsKeyPressed(GLFW_KEY_1))
		{
			Left = 1;
			Total = 1;
		}
		if (window.IsKeyPressed(GLFW_KEY_2))
		{
			Left = 2;
			Total = 2;
		}
		if (window.IsKeyPressed(GLFW_KEY_3))
		{
			Left = 3;
			Total = 3;
		}
	}
}

/*
��Ԃ̍X�V
*/
void TitleScene::Update()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	//�o�ߎ��Ԃ������炷
	if (timer > 0.0f)
	{
		timer -= window.DeltaTime();
	}
	
}

/*
�`��
*/
void TitleScene::Render()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//�����������@�\��L���ɂ���
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	meshList.BindVertexArray();

	progSimple.Use();

	//���ˉe�ŕ`�悷��悤�ɍs���ݒ�
	const glm::mat4x4 matProj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 500.0f);
	const glm::mat4x4 matView =
		glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	progSimple.SetViewProjectionMatrix(matProj * matView);

	//�w�i�ƃ��S���E�B���h�E�𒆐S�ɕ`��
	progSimple.BindTexture(0, texBackground.Get());
	progSimple.Draw(meshList.Get(6),
		glm::vec3(400, 300, -1), glm::vec3(0), glm::vec3(400, 300, 1));
	progSimple.BindTexture(0, texLogo.Get());
	progSimple.Draw(meshList.Get(6),
		glm::vec3(400, 300, 0), glm::vec3(0), glm::vec3(400, 300, 1));


	if (title == Title::start)
	{
		progSimple.BindTexture(0, texStartButton_Y.Get());
		progSimple.Draw(meshList.Get(6),
			glm::vec3(300, 150, 1), glm::vec3(0), glm::vec3(100, 50, 1));
		progSimple.BindTexture(0, texGameEndButton_B.Get());
		progSimple.Draw(meshList.Get(6),
			glm::vec3(500, 150, 1), glm::vec3(0), glm::vec3(100, 50, 1));
	}
	else if (title == Title::end)
	{
		progSimple.BindTexture(0, texStartButton_B.Get());
		progSimple.Draw(meshList.Get(6),
			glm::vec3(300, 150, 1), glm::vec3(0), glm::vec3(100, 50, 1));
		progSimple.BindTexture(0, texGameEndButton_Y.Get());
		progSimple.Draw(meshList.Get(6),
			glm::vec3(500, 150, 1), glm::vec3(0), glm::vec3(100, 50, 1));
	}
}

/*
�I��
*/
void TitleScene::Finalize()
{

}

/*
�^�C�g����ʂ��I��������ǂ������ׂ�

@retval true  �I�����
@retval false �I����Ă��Ȃ�
*/
bool TitleScene::IsFinish() const
{
	return isFinish;
}