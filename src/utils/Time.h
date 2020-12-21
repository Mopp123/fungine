#pragma once


namespace fungine
{
	class Time
	{
	private:
		static double s_deltaTime;
		static double s_fps;

		static int s_frames;

		static double s_frameStartTime;

	public:

		Time();
		static void update();

		inline static double get_fps() { return s_fps; }
	};
}