
#include <GLFW/glfw3.h> // *->TEMP Just temporarely using directly glfw here!!! fix later!
#include "Time.h"
#include <stdio.h>

namespace fungine
{

	double Time::s_deltaTime = 0.0;
	double Time::s_fps = 0.0;

	int Time::s_frames = 0;

	double Time::s_frameStartTime = 0.0;


	Time::Time()
	{
		s_frameStartTime = glfwGetTime();
	}

	void Time::update()
	{
		if (glfwGetTime() - s_frameStartTime > 0.25 && s_frames > 10)
		{
			s_fps = (double)(s_frames) / (glfwGetTime() - s_frameStartTime);
			printf("Fps: %f\n", s_fps);
			s_frames = 0;
			s_frameStartTime = glfwGetTime();
		}
		s_frames++;
	}

}