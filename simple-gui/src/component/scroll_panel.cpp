#include "component/scroll_panel.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	ScrollPanel::ScrollPanel() {
		m_hScrollBar = std::make_unique<ScrollBar>(Direction::Horizontal);
		m_vScrollBar = std::make_unique<ScrollBar>(Direction::Vertical);

		m_hScrollBar->SetTarget(this);
		m_vScrollBar->SetTarget(this);
	}

	void ScrollPanel::EnteredComponentTree() {
		SetComponentOwner(m_hScrollBar.get(), m_window, this);
		SetComponentOwner(m_vScrollBar.get(), m_window, this);
		BaseComponent::EnteredComponentTree(m_hScrollBar.get());
		BaseComponent::EnteredComponentTree(m_vScrollBar.get());
	}

	inline Vec2 ScrollPanel::GetContentSize() const {
		return Vec2(
			m_size.w - m_padding.left - m_padding.right - m_vScrollBar->GetSize().w,
			m_size.h - m_padding.top - m_padding.bottom - m_hScrollBar->GetSize().h
		);
	}

	bool ScrollPanel::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		if (BaseComponent::HandleEvent(event)) return true;
		if (!m_visibleGRect.ContainPoint(SG_GuiManager.GetMousePosition())) return false;

		// mouse wheel to h scroll
		// shift-mouse_wheel to v scroll 
		if (auto ev = event->Convert<KeyBoardButtonEvent>()) {
			m_shiftPressed = (ev->IsPressed() && ev->GetKeyCode() == SDLK_LSHIFT) ? true : false;
		}

		if (auto ev = event->Convert<MouseWheelEvent>()) {
			 if (m_shiftPressed) {
				 m_vScrollBar->UpdateSliderPositionByWheel(m_vScrollBar->m_mouseWheelDelta, -ev->GetDirection().y);
			 }
			 else {
				 m_hScrollBar->UpdateSliderPositionByWheel(m_hScrollBar->m_mouseWheelDelta, -ev->GetDirection().y);
			 }
		}

		return false;
	}

	void ScrollPanel::Update() {
		SG_CMP_UPDATE_CONDITIONS;
	
		BaseComponent::Update();

		// update scroll bar, position, visible rect
		m_hScrollBar->Update();
		m_vScrollBar->Update();
	}

	void ScrollPanel::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::ScrollPanelBackground));
		m_hScrollBar->Render(renderer);
		m_vScrollBar->Render(renderer);
		BaseComponent::Render(renderer);
		renderer.SetClipRect(m_visibleGRect);
		renderer.DrawRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::ScrollPanelBorder));
		renderer.ClearClipRect();
	}
}