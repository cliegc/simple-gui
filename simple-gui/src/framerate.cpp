#include "framerate.hpp"
#include "window.hpp"


namespace SimpleGui {
	FrameRateController::FrameRateController(Window* window, uint32_t fps) {
		m_lastFrameTime = Clock::now();
		m_targetFrameTime = std::chrono::nanoseconds(static_cast<int64_t>(1000000000.0 / static_cast<double>(fps)));
		m_sumDeltaTime = std::chrono::nanoseconds(0);
		m_targetFrameRate = fps;
		m_frameCount = 0;
		m_realFrameRate = fps;
		m_delta = 0;
		m_isUnlimited = false;
		m_window = window;
	}

	void FrameRateController::Update() {
		auto deltaTime = Clock::now() - m_lastFrameTime;
		
		if (!m_isUnlimited && !m_window->IsEnabledVsync()) {
			if (deltaTime < m_targetFrameTime) {
				SDL_DelayNS((m_targetFrameTime - deltaTime).count());
			}
		}

		auto endFrameTime = Clock::now();
		m_frameCount++;
		m_sumDeltaTime += endFrameTime - m_lastFrameTime;
		if (m_sumDeltaTime >= std::chrono::milliseconds(100)) {
			m_realFrameRate = static_cast<double>(m_frameCount) / (static_cast<double>(m_sumDeltaTime.count()) / 1000000000.0);
			m_frameCount = 0;
			m_sumDeltaTime = std::chrono::nanoseconds(0);
		}
		m_delta = static_cast<double>((endFrameTime - m_lastFrameTime).count()) / 1000000000.0;
		m_lastFrameTime = endFrameTime;
	}

	void FrameRateController::SetTargetFrameRate(uint32_t fps) {
		m_targetFrameTime = std::chrono::nanoseconds(static_cast<int64_t>(1000000000.0 / static_cast<double>(fps)));
		m_targetFrameRate = fps;
		m_realFrameRate = fps;
	}
}