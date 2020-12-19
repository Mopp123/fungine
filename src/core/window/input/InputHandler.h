#pragma once

#include "KeyEvent.h"
#include "MouseButtonEvent.h"
#include "CursorPosEvent.h"

#include <vector>
#include <unordered_map>

#define INPUT_MAX_KEYS 256
#define INPUT_ACTION__TYPE_COUNT 2

#define INPUT_ACTION__SLOT__RELEASE 0
#define INPUT_ACTION__SLOT__PRESS 1

// KEY NAMES ------------------------- KEY NAMES
#define FUNGINE_KEY_A 0
#define FUNGINE_KEY_B 1
#define FUNGINE_KEY_C 2
#define FUNGINE_KEY_D 3
#define FUNGINE_KEY_E 4
#define FUNGINE_KEY_F 5
#define FUNGINE_KEY_G 6
#define FUNGINE_KEY_H 7
#define FUNGINE_KEY_I 8
#define FUNGINE_KEY_J 9
#define FUNGINE_KEY_K 10
#define FUNGINE_KEY_L 11
#define FUNGINE_KEY_M 12
#define FUNGINE_KEY_N 13
#define FUNGINE_KEY_O 14
#define FUNGINE_KEY_P 15
#define FUNGINE_KEY_Q 16
#define FUNGINE_KEY_R 16
#define FUNGINE_KEY_S 18
#define FUNGINE_KEY_T 19
#define FUNGINE_KEY_U 20
#define FUNGINE_KEY_V 21
#define FUNGINE_KEY_W 22
#define FUNGINE_KEY_X 23
#define FUNGINE_KEY_Y 24
#define FUNGINE_KEY_Z 25

#define FUNGINE_KEY_LEFT_CTRL 26
#define FUNGINE_KEY_LEFT_SHIFT 27
#define FUNGINE_KEY_SPACE 28
#define FUNGINE_KEY_ENTER 29
#define FUNGINE_KEY_ESCAPE 30

#define FUNGINE_KEY_LEFT	31
#define FUNGINE_KEY_RIGHT	32
#define FUNGINE_KEY_UP		33
#define FUNGINE_KEY_DOWN	34

#define FUNGINE_KEY_COUNT 35


// MOUSE BUTTON NAMES ------------------------- MOUSE BUTTON NAMES
#define FUNGINE_MOUSE_LEFT_BUTTON	0
#define FUNGINE_MOUSE_RIGHT_BUTTON	1
#define FUNGINE_MOUSE_MIDDLE_BUTTON 2

#define FUNGINE_MOUSE_BUTTON_COUNT 3


// INPUT ACTION NAMES ------------------------- INPUT ACTION NAMES
#define FUNGINE_INPUT_RELEASE	0
#define FUNGINE_INPUT_PRESS		1
#define FUNGINE_INPUT_REPEAT	2

#define FUNGINE_INPUT_TYPES_COUNT 3


namespace fungine
{
	namespace core
	{
		class DefaultKeyEvent : public KeyEvent
		{
		public:
			virtual void callbackFunc(int key, int scancode, int action, int mods) override;
		};
		class DefaultMouseButtonEvent : public MouseButtonEvent
		{
		public:
			virtual void callbackFunc(int button, int action, int mods) override;
		};
		class DefaultCursorPosEvent : public CursorPosEvent
		{
		public:
			virtual void callbackFunc(double x, double y) override;
		};

		class InputHandler
		{
		private:
			static std::vector<KeyEvent*>			s_keyEvents;
			static std::vector<MouseButtonEvent*>	s_mouseButtonEvents;
			static std::vector<CursorPosEvent*>		s_cursorPosEvents;

			/*
				How we abstract windowing api keys, actions, etc away?:

				Idea here is that we get something from windowing api and then we convert that into "our key identifier".
				For example, in KeyEvent, we can say if(key == FUNGINE_KEY_W), and all windowing api shit is abstracted away
				when we specify our own input key events.
				
				windowing api -> to our mapping
				key = windowing api identifier, value = "our identifier"
			*/
			static std::unordered_map<int, int> s_keyMapping;
			static std::unordered_map<int, int> s_actionMapping;

			static bool s_keyState[FUNGINE_KEY_COUNT];
			static bool s_mouseButtonState[FUNGINE_MOUSE_BUTTON_COUNT];
			static double s_mouseX;
			static double s_mouseY;
			static double s_prevMouseX;
			static double s_prevMouseY;
			static double s_mouseDX;
			static double s_mouseDY;

			static DefaultKeyEvent			s_defaultKeyEvent;
			static DefaultMouseButtonEvent	s_defaultMouseButtonEvent;
			static DefaultCursorPosEvent	s_defaultCursorPosEvent;

			friend DefaultKeyEvent;
			friend DefaultMouseButtonEvent;
			friend DefaultCursorPosEvent;

		public:
			InputHandler();
			~InputHandler();

			// Needs to be called at the end of a frame.
			// Needs to be called before Window calls "updateEvents()"
			void prepareForNextFrame();

			static bool is_key_down(int keyName);
			static bool is_mouse_down(int button);
			static double get_mouse_x();
			static double get_mouse_y();
			static double get_mouse_dx();
			static double get_mouse_dy();

			static std::unordered_map<int,int>& get_key_mapping();
			static std::unordered_map<int, int>& get_action_mapping();

			static void add_key_event(KeyEvent* e);
			static void add_mouse_button_event(MouseButtonEvent* e);
			static void add_cursor_pos_event(CursorPosEvent* e);
			
			static void key_callbacks(int key_windowApi, int scancode_windowApi, int action_windowApi, int mods_windowApi);
			static void mouse_button_callbacks(int button_windowApi, int action_windowApi, int mods_windowApi);
			static void cursor_pos_callbacks(double x_windowApi, double y_windowApi);

			static void clear_events();
		};
	}
}