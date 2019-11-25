#pragma once
/*
@file TitleScene.h
*/
#ifndef TITLESCENE_H_INCLUDED
#define TITLESCENE_H_INCLUDED
#include "GLFWEW.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshList.h"
#include "Scene.h"
#include "Enemy.h"

/*
タイトル画面
*/
class TitleScene : public Scene
{
public:
	TitleScene() = default;
	virtual ~TitleScene() = default;

	virtual bool Initialize() override;
	virtual void ProcessInput() override;  //入力を受けて内部状態を設定する
	virtual void Update() override;        //プログラムの状態を更新する
	virtual void Render() override;        //モデルの表示などを行う
	virtual void Finalize() override;      //Initialize関数で準備したデータを破棄する

	bool IsFinish() const;

private:
	MeshList meshList;

	Texture::Image2D texLogo;
	Texture::Image2D texBackground;
	Texture::Image2D texStartButton_B;
	Texture::Image2D texStartButton_Y;
	Texture::Image2D texGameEndButton_B;
	Texture::Image2D texGameEndButton_Y;

	Shader::Program progSimple;

	float timer;
	bool isFinish;

	enum class Title
	{
		start,
		end
	};
	Title title = Title::start;
};

#endif // !TITLESCENE_H_INCLUDED


