#include "framerate.hpp"
#include <thread>


namespace SimpleGui {
	FrameRateController::FrameRateController(uint32_t fps) {
		m_currentFrameTime = Clock::now();
		m_lastFrameTime = Clock::now();
		m_targetFrameTime = std::chrono::microseconds(static_cast<int64_t>(1000000.0 / static_cast<double>(fps)));
		m_frameTimeSum = std::chrono::microseconds(0);
		m_frameCount = 0;
		m_targetFrameRate = fps;
		m_realFrameRate = static_cast<double>(fps);
		m_isUnlimited = false;
	}

	bool FrameRateController::Update() {
		m_currentFrameTime = Clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(
			m_currentFrameTime - m_lastFrameTime);

		if (deltaTime >= m_targetFrameTime) {
			m_frameTimeSum += deltaTime;
			m_lastFrameTime = m_currentFrameTime;
			m_frameCount++;

			if (m_frameTimeSum >= std::chrono::seconds(1)) {
				m_realFrameRate = 1000000.0 / (static_cast<double>(m_frameTimeSum.count()) / m_frameCount);
				m_frameTimeSum = std::chrono::microseconds(0);
				m_frameCount = 0;
			}

			return true;
		}

		return false;
	}

	FrameRateController::Clock::time_point FrameRateController::CalcNextWakeTime(const Clock::time_point& initWakeTime) const {
		Clock::time_point nextWakeTime = initWakeTime;
		auto timeUntilNextFrame = m_targetFrameTime -
			std::chrono::duration_cast<std::chrono::microseconds>(
				m_currentFrameTime - m_lastFrameTime);
		auto wakeTimeForWindow = m_currentFrameTime + timeUntilNextFrame;
		if (wakeTimeForWindow < nextWakeTime) {
			nextWakeTime = wakeTimeForWindow;
		}
		return nextWakeTime;
	}

	void FrameRateController::Sleep(const Clock::time_point& time) {
		if (time < std::chrono::high_resolution_clock::time_point::max()) {
			std::this_thread::sleep_until(time);
		}
	}

	void FrameRateController::SetTargetFrameRate(uint32_t fps) {
		m_targetFrameTime = std::chrono::microseconds(static_cast<int64_t>(1000000.0 / static_cast<double>(fps)));
		m_targetFrameRate = fps;
		m_realFrameRate = static_cast<double>(fps);
	}
}