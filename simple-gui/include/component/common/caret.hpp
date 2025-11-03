#pragma once
#include "math.hpp"
#include "timer.hpp"


namespace SimpleGui {
	class Renderer;

	class Caret {
	public:
		Caret();
		virtual ~Caret() = default;

		void Update() const;
		void Render(Renderer& renderer) const;

		Rect& GetGlobalRect() { return m_gRect; }

		Color GetColor() const { return m_color; }
		void SetColor(const Color& color) { m_color = color; }

		bool IsBlink() const { return m_blink; }
		void SetBlink(bool blink) {
			m_blink = blink;
			if (m_blink) m_timer->Start();
			else m_timer->SetPaused(true);
			m_blinkFlag = true;
		}

		float GetBlinkInterval() const { return m_timer->GetInterval(); }
		void SetBlinkInterval(float interval) const { m_timer->SetInterval(interval); }

		bool IsVisible() const { return m_visible; }
		void SetVisible(bool visible) {
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