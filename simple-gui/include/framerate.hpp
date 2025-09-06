#pragma once
#include <SDL3/SDL_timer.h>
#include <chrono>


namespace SimpleGui {
	class FrameRateController final {
	public:
		FrameRateController();
		~FrameRateController() = default;

	private:
		friend class Window;

		//std::chrono::steady_clock::time_point 
		Uint32 m_lastTicks;
		Uint32 m_targetDuration;
		double m_framerate;

		double Update();
		double GetFrameRate() const;
		void SetFrameRate(uint32_t fps);
	};
}
