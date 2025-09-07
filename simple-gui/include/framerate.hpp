#pragma once
#include <SDL3/SDL_timer.h>
#include <chrono>


namespace SimpleGui {
	class Window;

	class FrameRateController final {
	public:
		FrameRateController(Window* window, uint32_t fps = 60);
		~FrameRateController() = default;

	private:
		friend class GuiManager;
		friend class Window;

		using Clock = std::chrono::high_resolution_clock;

		Clock::time_point m_lastFrameTime;
		std::chrono::nanoseconds m_targetFrameTime;
		std::chrono::nanoseconds m_sumDeltaTime;
		uint32_t m_targetFrameRate;
		uint32_t m_frameCount;
		double m_realFrameRate;
		double m_delta;
		bool m_isUnlimited;

		Window* m_window;

		void Update();

		inline void SetUnlimitedFrameRate(bool value) { m_isUnlimited = value; }
		inline double GetRealFrameRate() const { return m_realFrameRate; }
		inline double GetDelta() const { return m_delta; }
		inline uint32_t GetTargetFrameRate() const { return m_targetFrameRate; }
		void SetTargetFrameRate(uint32_t fps);
	};
}
