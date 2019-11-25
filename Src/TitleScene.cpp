/*
@file TitleScene.cpp
*/
#include "TitleScene.h"
#include <glm/gtc/matrix_transform.hpp>

extern int Total;
extern int Left;

/*
初期化
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

	//一定時間入力を受け付けないようにする
	timer = 1.0f;
	isFinish = false;

	Left = 1;
	Total = 1;

	return true;
}

/*
入力の反映
*/
void TitleScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const GamePad gamepad = window.GetGamePad();
	if (!isFinish && timer <= 0.0f)
	{
		//ENTERキーで終了
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
状態の更新
*/
void TitleScene::Update()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	//経過時間だけ減らす
	if (timer > 0.0f)
	{
		timer -= window.DeltaTime();
	}
	
}

/*
描画
*/
void TitleScene::Render()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//半透明合成機能を有効にする
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	meshList.BindVertexArray();

	progSimple.Use();

	//正射影で描画するように行列を設定
	const glm::mat4x4 matProj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 500.0f);
	const glm::mat4x4 matView =
		glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	progSimple.SetViewProjectionMatrix(matProj * matView);

	//背景とロゴをウィンドウを中心に描画
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
終了
*/
void TitleScene::Finalize()
{

}

/*
タイトル画面が終わったかどうか調べる

@retval true  終わった
@retval false 終わっていない
*/
bool TitleScene::IsFinish() const
{
	return isFinish;
}