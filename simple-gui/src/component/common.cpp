#include "component/common.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	Caret::Caret(): 
		m_width(5.f), m_color(Color::WHITE), m_blink(false), m_visible(true){
		m_timer = SG_GuiManager.GetTimer(0.65f);
		m_timer->timeout.Connect("on_timeout",
			[this]() {
				m_visible = !m_visible;
			});
		m_timer->Start();
	}

	Caret::~Caret() {
		SG_GuiManager.KillTimer(m_timer);
	}

	void Caret::Render(const Renderer& renderer, Rect& rect) const {
		if (!m_visible) return;

		rect.size.w = m_width;
		renderer.FillRect(rect, m_color);
	}
}