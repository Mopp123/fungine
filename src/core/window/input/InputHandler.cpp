
#include "InputHandler.h"

namespace fungine
{
	namespace core
	{
		// Create default events
		void DefaultKeyEvent::callbackFunc(int key, int scancode, int action, int mods)
		{
			InputHandler::s_keyState[key] = action != FUNGINE_INPUT_RELEASE;
		}

		void DefaultMouseButtonEvent::callbackFunc(int button, int action, int mods)
		{
			InputHandler::s_mouseButtonState[button] = action != FUNGINE_INPUT_RELEASE;
		}
		
		void DefaultCursorPosEvent::callbackFunc(double x, double y)
		{
			InputHandler::s_mouseX = x;
			InputHandler::s_mouseY = y;
		}


		std::vector<KeyEvent*>			InputHandler::s_keyEvents;
		std::vector<MouseButtonEvent*>	InputHandler::s_mouseButtonEvents;
		std::vector<CursorPosEvent*>	InputHandler::s_cursorPosEvents;

		std::unordered_map<int, int> InputHandler::s_keyMapping;
		std::unordered_map<int, int> InputHandler::s_actionMapping;

		bool InputHandler::s_keyState[FUNGINE_KEY_COUNT] = { false };
		bool InputHandler::s_mouseButtonState[FUNGINE_MOUSE_BUTTON_COUNT] = { false };
		double InputHandler::s_mouseX = 0.0f;
		double InputHandler::s_mouseY = 0.0f;
		double InputHandler::s_prevMouseX = 0.0f;
		double InputHandler::s_prevMouseY = 0.0f;
		double InputHandler::s_mouseDX = 0.0f;
		double InputHandler::s_mouseDY = 0.0f;

		DefaultKeyEvent			InputHandler::s_defaultKeyEvent;
		DefaultMouseButtonEvent	InputHandler::s_defaultMouseButtonEvent;
		DefaultCursorPosEvent	InputHandler::s_defaultCursorPosEvent;

		InputHandler::InputHandler()
		{
			add_key_event(&s_defaultKeyEvent);
			add_mouse_button_event(&s_defaultMouseButtonEvent);
			add_cursor_pos_event(&s_defaultCursorPosEvent);
		}

		InputHandler::~InputHandler()
		{
			clear_events();
		}

		void InputHandler::prepareForNextFrame()
		{
			s_mouseDX = s_mouseX - s_prevMouseX;
			s_mouseDY = -(s_mouseY - s_prevMouseY);

			s_prevMouseX = s_mouseX;
			s_prevMouseY = s_mouseY;
		}

		bool InputHandler::is_key_down(int keyName)		{ return s_keyState[keyName]; }
		bool InputHandler::is_mouse_down(int button)	{ return s_mouseButtonState[button]; }
		double InputHandler::get_mouse_x()				{ return s_mouseX; }
		double InputHandler::get_mouse_y()				{ return s_mouseY; }
		double InputHandler::get_mouse_dx()				{ return s_mouseDX; }
		double InputHandler::get_mouse_dy()				{ return s_mouseDY; }

		std::unordered_map<int, int>& InputHandler::get_key_mapping()
		{
			return s_keyMapping;
		}
		std::unordered_map<int, int>& InputHandler::get_action_mapping()
		{
			return s_actionMapping;
		}

		void InputHandler::add_key_event(KeyEvent* e)					{ s_keyEvents.push_back(e); }
		void InputHandler::add_mouse_button_event(MouseButtonEvent* e)	{ s_mouseButtonEvents.push_back(e); }
		void InputHandler::add_cursor_pos_event(CursorPosEvent* e)		{ s_cursorPosEvents.push_back(e); }
		
		void InputHandler::key_callbacks(int key_windowApi, int scancode_windowApi, int action_windowApi, int mods_windowApi)
		{
			for (KeyEvent* e : InputHandler::s_keyEvents)
				e->callbackFunc(s_keyMapping[key_windowApi], scancode_windowApi, s_actionMapping[action_windowApi], mods_windowApi);
		}
		void InputHandler::mouse_button_callbacks(int button_windowApi, int action_windowApi, int mods_windowApi)
		{
			for (MouseButtonEvent* e : InputHandler::s_mouseButtonEvents)
				e->callbackFunc(s_keyMapping[button_windowApi], s_actionMapping[action_windowApi], mods_windowApi);
		}
		void InputHandler::cursor_pos_callbacks(double x_windowApi, double y_windowApi)
		{
			for (CursorPosEvent* e : InputHandler::s_cursorPosEvents)
				e->callbackFunc(x_windowApi, y_windowApi);
		}


		void InputHandler::clear_events()
		{
			s_keyEvents.clear();
			s_mouseButtonEvents.clear();
			s_cursorPosEvents.clear();
		}
	}
}