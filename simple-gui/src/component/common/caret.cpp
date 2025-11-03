#include "component/common/caret.hpp"
#include "window.hpp"


namespace SimpleGui {
	Caret::Caret() :
		m_color(Color::WHITE), m_blink(false), m_blinkFlag(true), m_visible(true) {
		m_gRect.size.w = 1;
		m_timer = std::make_unique<Timer>(0.65f);
		m_timer->timeout.Connect("SG_CARET_TIMER_on_timeout",
			[this]() {
				m_blinkFlag = !m_blinkFlag;
			});
	}

	void Caret::Update() const {
		m_timer->Update();
	}

	void Caret::Render(Renderer& renderer) const {
		if (!m_visible || !m_blinkFlag) return;
		renderer.RenderRect(m_gRect, m_color, true);
	}
}