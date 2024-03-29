/*
@file GLFWE..cpp
*/

#include "GLFWEW.h"
#include <iostream>

//GLFWとGLEWをラップするための名前空間
namespace GLFWEW
{

	/*
	GLFWからのエラー報告を処理する

	@param error  エラー番号
	@param desc   エラーの内容
	*/
	void ErrorCallback(int error, const char* desc)
	{
		std::cerr << "ERROR: " << desc << std::endl;
	}

	/*
	シングルトンインスタンスを取得する

	@return windowのシングルトンインスタンス
	*/

	Window& Window::Instance()
	{
		static Window instance;
		return instance;
	}

	/*
	コンストラクタ
	*/
	Window::Window()
	{
	}

	/*
	デストラクタ
	*/
	Window::~Window()
	{
		if (isGLFWInitialized)
		{
			glfwTerminate();
		}
	}

	/*
	GLFW/GLEWの初期化

	@param w ウィンドウの描画範囲の幅(ピクセル)
	@param h ウィンドウの描画範囲の高さ(ピクセル)
	@param title ウィンドウタイトル(UTF-8の0終端文字列)
	
	@retval true   初期化成功
	@retval false  初期化失敗
	*/

	bool Window::Init(int w, int h, const char* title)
	{
		if (isInitialized)
		{
			std::cerr << "ERROR: GLFWEWはすでに初期化されています." << std::endl;
			return false;
		}
		if (!isGLFWInitialized)
		{
			
			glfwSetErrorCallback(ErrorCallback);
			//OpenGL及びGLFWの初期化を行う
			if (glfwInit() != GL_TRUE)
			{
				return false;
			}
			isGLFWInitialized = true;
		}

		if (!window)
		{
			//グラフィック描画用のウィンドウを作成する
			window = glfwCreateWindow(w, h, title, nullptr, nullptr);
			if (!window)
			{
				return false;
			}
			//指定したウィンドウに対応するOpenGLコンテキストを描画対象に設定する
			glfwMakeContextCurrent(window);
		}

		if (glewInit() != GLEW_OK)
		{
			std::cerr << "ERROR: GLEWの初期化に失敗しました." << std::endl;
			return false;
		}

		//OpenGLの情報をコンソールウィンドウへ出力する
		/*
		glGetString -> OpenGLコンテキストに関連付けらてたいくつかの情報を取得する関数
		GL_RENDERER・・・OpenGLを描画するグラフィックスハードウェア名、またはドライバ名
		*/
		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer: " << renderer << std::endl;
		//GL_VERSION・・・グラフィックスハードウェア、またはドライバが対応する最大のOpenGL
		const GLubyte* version = glGetString(GL_VERSION);
		std::cout << "Version: " << version << std::endl;

		isInitialized = true;
		return true;
	}

	/*
	ウィンドウを閉じるべきか調べる

	@retval true   閉じる
	@retval false  閉じない
	*/
	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(window) != 0;
	}

	/*
	フロントバッファとバックバッファを切り替える
	フロントバッファは表示側のフレームバッファ
	バックバッファは描画側のフレームバッファ
	*/
	void Window::SwapBuffers() const
	{
		//GLFWがOSから送られたイベントを処理する関数
		glfwPollEvents();
		/*
		フレームバッファの描画側(バック)と表示側(フロント)を入れ替える
		フレームバッファは絵を描画するためのメモリ
		*/
		glfwSwapBuffers(window);
	}

	/*
	キーが押されているか調べる

	@param key 調べたいキー・コード(GLFW_KEY_Aなど)

	@retval true  キーが押されている
	@retval false キーが押されていない

	使用できるキー・コード一覧はglfw3.hにあります(「keyboard」で検索)
	*/

	bool Window::IsKeyPressed(int key) const
	{
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	bool Window::IsMousePressed(int mouse) const
	{
		return glfwGetMouseButton(window, mouse) == GLFW_PRESS;
	}

	glm::vec3 Window::CursorPos() const
	{
		glm::vec3 mousePos = glm::vec3(0);
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		
		mousePos.x = x - windowWidth / 2;
		mousePos.y = y - windowHeight / 2;
		if (mousePos.x > 180)
		{
			mousePos.x = 180;
		}
		else if (mousePos.x < -180)
		{
			mousePos.x = -180;
		}
		if (mousePos.y > 180)
		{
			mousePos.y = 180;
		}
		else if (mousePos.y < -180)
		{
			mousePos.y = -180;
		}
		mousePos.z = 0;
		//printf("x = %1f , y = %1f",mouseX,mouseY);
		return mousePos;
	}

	/*
	タイマーを初期化する
	*/
	void Window::InitTimer()
	{
		glfwSetTime(0.0);
		previousTime = 0.0;
		deltaTime = 0.0;
	}

	/*
	タイマーを更新する
	*/
	void Window::UpdateTimer()
	{
		//経過時間を計測
		const double currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		//経過時間が長くなりすぎないように調整
		const float upperLimit = 0.25f; //経過時間として許容される上限
		if (deltaTime > upperLimit)
		{
			deltaTime = 0.1f;
		}

		UpdateGamePad();
	}

	/*
	経過時間を取得する

	@retrun 直前の2回のUpdateTimer()呼び出しの間に経過した時間
	*/
	double Window::DeltaTime() const
	{
		return deltaTime;
	}

	/*
	ゲームパッドの状態を取得する

	@return ゲームパッドの状態
	*/
	const GamePad& Window::GetGamePad() const
	{
		return gamePad;
	}

	/*
	ゲームパッドのアナログ入力位置ID

	@note 順序はXBOX360ゲームパッド準拠
	*/
	enum GamePad_AXES
	{
		GAMEPAD_AXES_LEFT_X,  ///< 左スティックのX軸.
		GAMEPAD_AXES_LEFT_Y,  ///< 左スティックのY軸.
		GAMEPAD_AXES_TRIGGER, ///< アナログトリガー.
		GAMEPAD_AXES_RIGHT_Y, ///< 右スティックのY軸.
		GAMEPAD_AXES_RIGHT_X, ///< 右スティックのX軸.
	};

	/**
	* ゲームパッドのデジタル入力装置ID.
	*
	* @note Xbox360ゲームパッド準拠.
	*/
	enum GAMEPAD_BUTTON {
		GAMEPAD_BUTTON_A, ///< Aボタン.
		GAMEPAD_BUTTON_B, ///< Bボタン.
		GAMEPAD_BUTTON_X, ///< Xボタン.
		GAMEPAD_BUTTON_Y, ///< Yボタン.
		GAMEPAD_BUTTON_L, ///< Lボタン.
		GAMEPAD_BUTTON_R, ///< Rボタン.
		GAMEPAD_BUTTON_BACK, ///< Backボタン.
		GAMEPAD_BUTTON_START, ///< Startボタン.
		GAMEPAD_BUTTON_L_THUMB, ///< 左スティックボタン.
		GAMEPAD_BUTTON_R_THUMB, ///< 右スティックボタン.
		GAMEPAD_BUTTON_UP, ///< 上キー.
		GAMEPAD_BUTTON_RIGHT, ///< 右キー.
		GAMEPAD_BUTTON_DOWN, ///< 下キー.
		GAMEPAD_BUTTON_LEFT, ///< 左キー.
	};

	/*
	ゲームパッドの状態を取得する
	*/
	void Window::UpdateGamePad()
	{
		//butonDownを生成するために、更新前の入力状態を保存しておく
		const uint32_t prevButtons = gamePad.buttons;

		//アナログ入力とボタン入力を取得
		int axesCount, buttonCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		const uint8_t* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		//両方の配列がnullptrでなく、最低限必要なデータ数を満たしていれば、有効なゲームパッドが接続されているとみなす
		if (axes && buttons && axesCount >= 2 && buttonCount >= 8)
		{
				//有効なゲームパッドが接続されている状態

			//方向キーの入力状態を消去して、左スティックの入力で置き換える
			gamePad.buttons &=
					~(GamePad::DPAD_UP|GamePad::DPAD_DOWN|GamePad::DPAD_LEFT|GamePad::DPAD_RIGHT);
			static const float digitalThreshold = 0.3f; //デジタル入力とみなす閾値
			if (axes[GAMEPAD_AXES_LEFT_Y] >= digitalThreshold)
			{
				gamePad.buttons |= GamePad::DPAD_UP;
			}
			else if (axes[GAMEPAD_AXES_LEFT_Y] <= -digitalThreshold) 
			{
				gamePad.buttons |= GamePad::DPAD_DOWN;
			}
			if (axes[GAMEPAD_AXES_LEFT_X] >= digitalThreshold)
			{
				gamePad.buttons |= GamePad::DPAD_LEFT;
			}
			else if (axes[GAMEPAD_AXES_LEFT_X] <= -digitalThreshold) 
			{
				gamePad.buttons |= GamePad::DPAD_RIGHT;
			}

			// 配列インデックスとGamePadキーの対応表.
			static const struct
			{
				int dataIndex;
				uint32_t gamepadBit;
			} 
			keyMap[] = 
			{
			  { GAMEPAD_BUTTON_A, GamePad::A },
			  { GAMEPAD_BUTTON_B, GamePad::B },
			  { GAMEPAD_BUTTON_X, GamePad::X },
			  { GAMEPAD_BUTTON_Y, GamePad::Y },
			  { GAMEPAD_BUTTON_L, GamePad::L },
			  { GAMEPAD_BUTTON_R, GamePad::R },
			  { GAMEPAD_BUTTON_START, GamePad::START },
			  { GAMEPAD_BUTTON_UP, GamePad::DPAD_UP },
			  { GAMEPAD_BUTTON_DOWN, GamePad::DPAD_DOWN },
			  { GAMEPAD_BUTTON_LEFT, GamePad::DPAD_LEFT },
			  { GAMEPAD_BUTTON_RIGHT, GamePad::DPAD_RIGHT },
			};

			for (const auto& e : keyMap)
			{
				if (buttons[e.dataIndex] == GLFW_PRESS)
				{
					gamePad.buttons |= e.gamepadBit;
				}
				else if (buttons[e.dataIndex] == GLFW_RELEASE)
				{
					gamePad.buttons &= ~e.gamepadBit;
				}
			}
		}

		// 前回の更新で押されてなくて、今回押されているキーの情報をbuttonDownに格納.
		gamePad.buttonDown = gamePad.buttons & ~prevButtons;
	}

}//namespace GLFWEW