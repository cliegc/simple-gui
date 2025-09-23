#include "timer.hpp"


namespace SimpleGui {
	Timer::Timer(float interval) {
		m_interval = interval;
		m_oneShot = false;
		m_paused = true;
		m_count = 0;
		m_lastTime = 0;
	}

	void Timer::Update() {
		if (m_paused) return;
		if (m_oneShot && m_count) return;
		else m_count = 0;

		Uint64 currTime = SDL_GetTicksNS();
		if (currTime - m_lastTime >= m_interval * 1000000000) {
			timeout.Emit();
			m_lastTime = SDL_GetTicksNS();

			m_count = m_oneShot ? 1 : 0;
		}
	}

	void Timer::Start() {
		m_paused = false;
		m_lastTime = SDL_GetTicksNS();
	}

	Timer* TimerManager::GetTimer(float interval) {
		Timer* timer = new Timer(interval);
		m_timers.push_back(std::move(std::unique_ptr<Timer>(timer)));
		return timer;
	}

	void TimerManager::KillTiemr(Timer* timer) {
		auto it = std::find_if(m_timers.begin(), m_timers.end(),
			[timer](auto& tm) {
				return tm.get() == timer;
			});

		if (it != m_timers.end()) {
			m_timers.erase(it);
		}
	}

	void TimerManager::Update() {
		for (auto& timer : m_timers) {
			timer->Update();
		}
	}
}