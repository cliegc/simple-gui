#include "component/common.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	Caret::Caret(): 
		m_color(Color::WHITE), m_blink(false), m_visible(true) {
		m_gRect.size.w = 1;
		m_timer = std::make_unique<Timer>(0.65f);
		m_timer->timeout.Connect("SG_CARET_TIMER_on_timeout",
			[this]() {
				m_visible = !m_visible;
			});
		m_timer->Start();
	}

	void Caret::Update() const {
		m_timer->Update();
	}

	void Caret::Render(const Renderer& renderer) const {
		if (!m_visible) return;
		renderer.FillRect(m_gRect, m_color);
	}
}