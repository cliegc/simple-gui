#include "timer.hpp"
#include <SDL3/SDL_log.h>


namespace SimpleGui {
	Timer::Timer(float interval) {
		m_interval = interval;
		m_oneShot = false;
		m_paused = true;
		m_kill = false;
		m_count = 0;
		m_lastTime = 0;
	}

	void Timer::Update() {
		if (m_paused) return;
		if (m_oneShot && m_count) return;
		else m_count = 0;

		Uint64 currTime = SDL_GetTicksNS();
		if (currTime - m_lastTime >= static_cast<Uint64>(m_interval * 1000000000)) {
			timeout.Emit();
			m_lastTime = SDL_GetTicksNS();

			m_count = m_oneShot ? 1 : 0;
		}
	}

	void Timer::Start() {
		m_paused = false;
		m_lastTime = SDL_GetTicksNS();
	}

	void Timer::Kill() {
		m_paused = true;
		m_kill = true;
	}

	Timer* TimerManager::GetTimer(float interval) {
		Timer* timer = new Timer(interval);
		m_timers.push_back(std::move(std::unique_ptr<Timer>(timer)));
		return timer;
	}

	void TimerManager::KillTimer(Timer* timer) {
		auto it = std::find_if(m_timers.begin(), m_timers.end(),
			[timer](auto& t) {
				return t.get() == timer;
			});

		if (it == m_timers.end()) return;
		else (*it)->Kill();
	}

	void TimerManager::Update() {
		for (auto it = m_timers.begin(); it != m_timers.end();) {
			if ((*it)->m_kill) {
				it = m_timers.erase(it);
				SDL_Log("TimerManager: timer count = %d", m_timers.size());
			}
			else {
				(*it)->Update();
				++it;
			}
		}
	}
}