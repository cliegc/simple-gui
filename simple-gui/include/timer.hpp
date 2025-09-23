#pragma once
#include <SDL3/SDL_timer.h>
#include <memory>
#include "signal.hpp"


namespace SimpleGui {
	class Timer final {
		friend class TimerManager;
	public:
		Signal<> timeout;

		~Timer() = default;

		inline float GetInterval() const { return m_interval; }
		inline void SetInterval(float interval) { m_interval = interval; }

		inline bool IsOneShot() const { return m_oneShot; }
		inline void SetOneShot(bool val) { m_oneShot = val;}

		inline bool IsPaused() const { return m_paused; }
		inline void SetPaused(bool val) { m_paused = val; }

		void Start();

	private:
		float m_interval;
		bool m_oneShot;
		bool m_paused;
		size_t m_count;
		Uint64 m_lastTime;

		Timer(float interval);
		void Update();
	};

	class TimerManager final {
		friend class GuiManager;
	public:
		TimerManager() = default;
		~TimerManager() = default;

	private:
		std::vector<std::unique_ptr<Timer>> m_timers;

		Timer* GetTimer(float interval);
		void KillTiemr(Timer* timer);

		void Update();
	};
}