#pragma once
#include <SDL3/SDL_timer.h>
#include <chrono>


namespace SimpleGui {
	class FrameRateController final {
	public:
		FrameRateController(uint32_t fps = 60);
		~FrameRateController() = default;

	private:
		friend class GuiManager;
		friend class Window;

		using Clock = std::chrono::high_resolution_clock;

		Clock::time_point m_currentFrameTime;
		Clock::time_point m_lastFrameTime;
		std::chrono::microseconds m_targetFrameTime;
		std::chrono::microseconds m_frameTimeSum;
		uint32_t m_frameCount;
		uint32_t m_targetFrameRate;
		double m_realFrameRate;
		bool m_isUnlimited;

		bool Update();
		Clock::time_point CalcNextWakeTime(const Clock::time_point& initWakeTime) const;
		
		static void Sleep(const Clock::time_point& time);

		inline void SetUnlimitedFrameRate(bool value) { m_isUnlimited = value; }
		inline double GetRealFrameRate() const { return m_realFrameRate; }
		inline uint32_t GetTargetFrameRate() const { return m_targetFrameRate; }
		void SetTargetFrameRate(uint32_t fps);
	};
}
