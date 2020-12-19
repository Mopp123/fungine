#pragma once

#include <string>
#include <unordered_map>
#include "Common.h"

// Debug defines ----------------------------------------------------------------------------------------- Debug defines
#define DEBUG__ERROR_LEVEL__NONE 0
#define DEBUG__ERROR_LEVEL__WARNING 1
#define DEBUG__ERROR_LEVEL__ERROR 2
#define DEBUG__ERROR_LEVEL__FATAL_ERROR 3

// Debug modes(Only one can be defined at a time):
	// Enables all debug stuff
		#define DEBUG__MODE_FULL

	// Enables only required debug stuff (disables graphics api related debug stuff..)
		//#define DEBUG__MODE_NONE

// Some helper macros ----------------------------------------------------------------------------------------- Some helper macros
//	..quite disgusting..
#ifdef DEBUG__MODE_FULL
#define GL_FUNC(func)	func;																				\
switch (glGetError()){																						\
case GL_NO_ERROR:break;																						\
case GL_INVALID_ENUM:					Debug::log("Opengl Error: GL_INVALID_ENUM");break;					\
case GL_INVALID_VALUE:					Debug::log("Opengl Error: GL_INVALID_VALUE");break;					\
case GL_INVALID_OPERATION:				Debug::log("Opengl Error: GL_INVALID_OPERATION");break;				\
case GL_INVALID_FRAMEBUFFER_OPERATION:	Debug::log("Opengl Error: GL_INVALID_FRAMEBUFFER_OPERATION");break;	\
case GL_OUT_OF_MEMORY:					Debug::log("Opengl Error: GL_OUT_OF_MEMORY");break;					\
case GL_STACK_UNDERFLOW:				Debug::log("Opengl Error: GL_STACK_UNDERFLOW");break;				\
case GL_STACK_OVERFLOW:					Debug::log("Opengl Error: GL_STACK_OVERFLOW");break;				\
default: break;}
#endif

#ifdef DEBUG__MODE_NONE
#define GL_FUNC(func)	func;
#endif

namespace fungine
{
	class Debug
	{
	private:
	public:

		static void log(const std::string& message, int errorLevel = DEBUG__ERROR_LEVEL__NONE);
		static void notify_on_destroy(const std::string& objName);
	};
}