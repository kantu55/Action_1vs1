/*
@file GLFWEW.h
*/
#ifndef GLFWEW_H_INCLUDED
#define GLFWEW_H_INCLUDED
#include <Windows.h>
#include <GL\glew.h>
#include "GamePad.h"
#include <GLFW\glfw3.h>
#include <glm\vec3.hpp>

namespace GLFWEW
{
	/*
	GLFWとGLEWのラッパークラス
	*/
	class Window
	{
	private:
		Window();
		~Window();
		/*
		GLFWやGLEWを管理するオブジェクトを複数作れるようになると
		初期化処理や終了処理がオブジェクトの数だけ実行されてしまう危険性があるため
		Windowクラスの利用者が作成・コピーすることを禁止している
		*/
		Window(const Window&) = delete;  //コピーコンストラクタ
		Window& operator = (const Window&) = delete;  //コピー代入演算子
		void UpdateGamePad();
		
		bool isGLFWInitialized = false;
		bool isInitialized = false;
		GLFWwindow* window = nullptr;
		double previousTime = 0;
		double deltaTime = 0;
		GamePad gamePad;

	public:
		/*
		Windowクラスを利用できないため、instance関数をstatic関数として
		定義し、この関数の中でオブジェクトを作成し、管理するようにする
		*/
		static Window& Instance();
		bool Init(int w, int h, const char* title);
		bool ShouldClose() const;
		void SwapBuffers() const;
		float windowWidth = 1280;
		float windowHeight = 720;
		void InitTimer();
		void UpdateTimer();
		double DeltaTime() const;

		bool IsKeyPressed(int key) const;
		bool IsMousePressed(int mouse) const;
		const GamePad& GetGamePad() const;
		glm::vec3 CursorPos() const;
	};
} // namespace GLFEEW

#endif // GLFWEW_H_INCLUDED
