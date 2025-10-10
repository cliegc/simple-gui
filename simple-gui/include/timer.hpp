#pragma once
#include <SDL3/SDL_timer.h>
#include <memory>
#include "signal.hpp"


namespace SimpleGui {
	class Timer final {
		friend class TimerManager;
	public:
		Signal<> timeout;

		Timer() = default;
		explicit Timer(float interval);
		~Timer() = default;

		float GetInterval() const { return m_interval; }
		void SetInterval(float interval) { m_interval = interval; }

		bool IsOneShot() const { return m_oneShot; }
		void SetOneShot(bool val) { m_oneShot = val;}

		bool IsPaused() const { return m_paused; }
		void SetPaused(bool val) { m_paused = val; }

		void Start();
		void Update();
		
	private:
		float m_interval{};
		bool m_oneShot{};
		bool m_paused{};
		bool m_kill{};
		size_t m_count{};
		Uint64 m_lastTime{};

		void Kill();
	};

	class TimerManager final {
		friend class GuiManager;
	public:
		TimerManager() = default;
		~TimerManager() = default;

	private:
		std::vector<std::unique_ptr<Timer>> m_timers;

		Timer* GetTimer(float interval);
		void KillTimer(Timer* timer);
		void Update();
	};
}