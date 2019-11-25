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
	GLFW��GLEW�̃��b�p�[�N���X
	*/
	class Window
	{
	private:
		Window();
		~Window();
		/*
		GLFW��GLEW���Ǘ�����I�u�W�F�N�g�𕡐�����悤�ɂȂ��
		������������I���������I�u�W�F�N�g�̐��������s����Ă��܂��댯�������邽��
		Window�N���X�̗��p�҂��쐬�E�R�s�[���邱�Ƃ��֎~���Ă���
		*/
		Window(const Window&) = delete;  //�R�s�[�R���X�g���N�^
		Window& operator = (const Window&) = delete;  //�R�s�[������Z�q
		void UpdateGamePad();
		
		bool isGLFWInitialized = false;
		bool isInitialized = false;
		GLFWwindow* window = nullptr;
		double previousTime = 0;
		double deltaTime = 0;
		GamePad gamePad;

	public:
		/*
		Window�N���X�𗘗p�ł��Ȃ����߁Ainstance�֐���static�֐��Ƃ���
		��`���A���̊֐��̒��ŃI�u�W�F�N�g���쐬���A�Ǘ�����悤�ɂ���
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
