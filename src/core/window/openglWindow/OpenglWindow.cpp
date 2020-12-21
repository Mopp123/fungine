
#include <GLEW/glew.h>
#include "OpenglWindow.h"
#include "core/Common.h"
#include "core/Debug.h"

namespace fungine
{
	namespace core
	{

		static void glfw_key_callback(GLFWwindow* w, int key, int scancode, int action, int mods)
		{
			InputHandler::key_callbacks(key, scancode, action, mods);
		}
		static void glfw_mouse_button_callback(GLFWwindow* w, int button, int action, int mods)
		{
			InputHandler::mouse_button_callbacks(button, action, mods);
		}
		static void glfw_cursor_pos_callback(GLFWwindow* w, double x, double y)
		{
			InputHandler::cursor_pos_callbacks(x, y);
		}

		OpenglWindow::OpenglWindow(const char* title, const int& width, const int& height, const int& swapInterval, const int& MSAASamples, bool fullscreen) :
			Window(title, width, height, swapInterval, MSAASamples, fullscreen)
		{
			if (!glfwInit())
			{
				Debug::log(
					"Location: OpenglWindow::OpenglWindow(const char* title, const int width, const int height, const int swapInterval)"
					"Failed glfwInit()",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				return;
			}

			glfwWindowHint(GLFW_SAMPLES, MSAASamples);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, DEFAULT__OPENGL_VERSION_MAJOR);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, DEFAULT__OPENGL_VERSION_MINOR);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
			glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

			_glfwWindowHandle = glfwCreateWindow(s_width, s_height, title, _isFullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
			if (!_glfwWindowHandle)
			{
				Debug::log(
					"Location: OpenglWindow::OpenglWindow(const char* title, const int width, const int height, const int swapInterval)\n"
					"Failed glfwCreateWindow()",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				glfwTerminate();
				return;
			}

			glfwMakeContextCurrent(_glfwWindowHandle);
			glfwSwapInterval(_swapInterval);

			if (glewInit() != GLEW_OK)
			{
				Debug::log(
					"Location: OpenglWindow::OpenglWindow(const char* title, const int width, const int height, const int swapInterval)\n"
					"Failed glewInit()",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				glfwTerminate();
				return;
			}

			
			// Tie our event system with this glfw window thing..
			glfwSetKeyCallback(_glfwWindowHandle, glfw_key_callback);
			glfwSetMouseButtonCallback(_glfwWindowHandle, glfw_mouse_button_callback);
			glfwSetCursorPosCallback(_glfwWindowHandle, glfw_cursor_pos_callback);

			Debug::log("OpenglWindow created successfully!");
		}

		OpenglWindow::~OpenglWindow()
		{
			Debug::notify_on_destroy("OpenglWindow");
		}


		bool OpenglWindow::isCloseRequested()
		{
			return (bool)(glfwWindowShouldClose(_glfwWindowHandle)) || _isCloseRequested;
		}
		void OpenglWindow::close()
		{
			_isCloseRequested = true;
		}
		void OpenglWindow::cleanUp()
		{
			glfwTerminate();
		}


		void OpenglWindow::swapBuffers() const
		{
			glfwSwapBuffers(_glfwWindowHandle);
		}
		void OpenglWindow::updateEvents()
		{
			_inputHandler.prepareForNextFrame();
			glfwPollEvents();
		}



		// Creates input mappings, for key names, actions, etc from windowing api format to our format.
		void OpenglWindow::createInputMapping() const
		{
			std::unordered_map<int, int>& actionMapping = InputHandler::get_action_mapping();
			actionMapping[GLFW_RELEASE] =	FUNGINE_INPUT_RELEASE;
			actionMapping[GLFW_PRESS] =		FUNGINE_INPUT_PRESS;
			actionMapping[GLFW_REPEAT] =	FUNGINE_INPUT_REPEAT;

			// ..and omg this is retarded..
			std::unordered_map<int, int>& keyMapping = InputHandler::get_key_mapping();
			keyMapping[GLFW_KEY_A] = FUNGINE_KEY_A;
			keyMapping[GLFW_KEY_B] = FUNGINE_KEY_B;
			keyMapping[GLFW_KEY_C] = FUNGINE_KEY_C;
			keyMapping[GLFW_KEY_D] = FUNGINE_KEY_D;
			keyMapping[GLFW_KEY_E] = FUNGINE_KEY_E;
			keyMapping[GLFW_KEY_F] = FUNGINE_KEY_F;
			keyMapping[GLFW_KEY_G] = FUNGINE_KEY_G;
			keyMapping[GLFW_KEY_H] = FUNGINE_KEY_H;
			keyMapping[GLFW_KEY_I] = FUNGINE_KEY_I;
			keyMapping[GLFW_KEY_J] = FUNGINE_KEY_J;
			keyMapping[GLFW_KEY_K] = FUNGINE_KEY_K;
			keyMapping[GLFW_KEY_L] = FUNGINE_KEY_L;
			keyMapping[GLFW_KEY_M] = FUNGINE_KEY_M;
			keyMapping[GLFW_KEY_N] = FUNGINE_KEY_N;
			keyMapping[GLFW_KEY_O] = FUNGINE_KEY_O;
			keyMapping[GLFW_KEY_P] = FUNGINE_KEY_P;
			keyMapping[GLFW_KEY_Q] = FUNGINE_KEY_Q;
			keyMapping[GLFW_KEY_R] = FUNGINE_KEY_R;
			keyMapping[GLFW_KEY_S] = FUNGINE_KEY_S;
			keyMapping[GLFW_KEY_T] = FUNGINE_KEY_T;
			keyMapping[GLFW_KEY_U] = FUNGINE_KEY_U;
			keyMapping[GLFW_KEY_V] = FUNGINE_KEY_V;
			keyMapping[GLFW_KEY_W] = FUNGINE_KEY_W;
			keyMapping[GLFW_KEY_X] = FUNGINE_KEY_X;
			keyMapping[GLFW_KEY_Y] = FUNGINE_KEY_Y;
			keyMapping[GLFW_KEY_Z] = FUNGINE_KEY_Z;

			keyMapping[GLFW_KEY_LEFT_CONTROL] = FUNGINE_KEY_LEFT_CTRL;
			keyMapping[GLFW_KEY_LEFT_SHIFT] =	FUNGINE_KEY_LEFT_SHIFT;
			keyMapping[GLFW_KEY_SPACE] =		FUNGINE_KEY_SPACE;
			keyMapping[GLFW_KEY_ENTER] =		FUNGINE_KEY_ENTER;
			keyMapping[GLFW_KEY_ESCAPE] =		FUNGINE_KEY_ESCAPE;
		
			keyMapping[GLFW_KEY_LEFT] =		FUNGINE_KEY_LEFT;
			keyMapping[GLFW_KEY_RIGHT] =	FUNGINE_KEY_RIGHT;
			keyMapping[GLFW_KEY_UP] =		FUNGINE_KEY_UP;
			keyMapping[GLFW_KEY_DOWN] =		FUNGINE_KEY_DOWN;

		}

	}
}