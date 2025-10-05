#pragma once
#include <SDL3/SDL_time.h>
#include "math.hpp"
#include "timer.hpp"
#include "component/common/utils.hpp"


namespace SimpleGui {
	class Renderer;

	class Caret {
	public:
		Caret();
		virtual ~Caret() = default;

		void Update() const;
		void Render(const Renderer& renderer) const;

		inline Rect& GetGlobalRect() { return m_gRect; }

		inline Color GetColor() const { return m_color; }
		inline void SetColor(const Color& color) { m_color = color; }

		inline bool IsBlink() const { return m_blink; }
		inline void SetBlink(bool blink) {
			m_blink = blink;
			if (m_blink) m_timer->Start();
			else m_timer->SetPaused(true);
			m_blinkFlag = true;
		}

		inline float GetBlinkInterval() const { return m_timer->GetInterval(); }
		inline void SetBlinkInterval(float interval) { m_timer->SetInterval(interval); }

		inline bool IsVisible() const { return m_visible; }
		inline void SetVisible(bool visible) {
			m_visible = visible;
			if (m_blink) {
				m_visible ? m_timer->Start() : m_timer->SetPaused(true);
			}
			m_blinkFlag = true;
		}

	private:
		Rect m_gRect;
		Color m_color;
		bool m_blink;
		bool m_blinkFlag;
		bool m_visible;
		std::unique_ptr<Timer> m_timer;
	};
}