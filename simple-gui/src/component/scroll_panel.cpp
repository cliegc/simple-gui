#include "component/scroll_panel.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	ScrollPanel::ScrollPanel() {
		m_hScrollBar = std::make_unique<ScrollBar>(Direction::Horizontal);
		m_vScrollBar = std::make_unique<ScrollBar>(Direction::Vertical);
		m_shiftPressed = false;

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
		float ofsX = m_vScrollBar->m_visible ? m_vScrollBar->m_size.w : 0.f;
		float ofsY = m_hScrollBar->m_visible ? m_hScrollBar->m_size.h : 0.f;
		return Vec2(
			m_size.w - m_padding.left - m_padding.right - ofsX,
			m_size.h - m_padding.top - m_padding.bottom - ofsY
		);
	}

	bool ScrollPanel::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		if (m_hScrollBar->HandleEvent(event)) return true;
		if (m_vScrollBar->HandleEvent(event)) return true;

		//if (!m_visibleGRect.ContainPoint(SG_GuiManager.GetMousePosition())) return false;

		// mouse wheel to h scroll
		// shift-mouse_wheel to v scroll 
		if (auto ev = event->Convert<KeyBoardButtonEvent>()) {
			m_shiftPressed = (ev->IsPressed() && ev->GetKeyMod() == SDL_KMOD_LSHIFT) ? true : false;
		}

		if (auto ev = event->Convert<MouseWheelEvent>()) {
			if (!m_shiftPressed) {
				m_vScrollBar->UpdateSliderPositionByWheel(m_vScrollBar->m_mouseWheelDelta, -ev->GetDirection().y);
			}
			else {
				m_hScrollBar->UpdateSliderPositionByWheel(m_hScrollBar->m_mouseWheelDelta, -ev->GetDirection().y);
			}
		}

		if (BaseComponent::HandleEvent(event)) return true;

		return false;
	}

	void ScrollPanel::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		BaseComponent::Update();

		Rect globalRect = GetGlobalRect();
		Rect contentGRect = GetContentGlobalRect();
		Rect boundaryGRect = CalcChildrenBoundaryGlobalRect(this);
		boundaryGRect = ScrollBar::AdjustTargetChildrenBoundaryGlobalRect(boundaryGRect, contentGRect);
		
		// update hScrollBar
		m_hScrollBar->SetGlobalPosition(globalRect.Left(), globalRect.Bottom() - m_hScrollBar->m_size.h);
		m_hScrollBar->SetWidth(m_size.w - m_vScrollBar->m_size.w);
		m_hScrollBar->m_visibleGRect = CalcVisibleGlobalRect(m_visibleGRect, m_visibleGRect, m_hScrollBar->GetGlobalRect());
		m_hScrollBar->UpdateHorizontalSlider(boundaryGRect, contentGRect);
		
		// update vScrollBar
		m_vScrollBar->SetGlobalPosition(globalRect.Right() - m_vScrollBar->m_size.w, globalRect.Top());
		m_vScrollBar->SetHeight(m_size.h - m_hScrollBar->m_size.h);
		m_vScrollBar->m_visibleGRect = CalcVisibleGlobalRect(m_visibleGRect, m_visibleGRect, m_vScrollBar->GetGlobalRect());
		m_vScrollBar->UpdateVerticalSlider(boundaryGRect, contentGRect);

		m_hScrollBar->m_visible = !(IsZeroApprox(m_hScrollBar->m_slider.globalRect.size.w));
		m_vScrollBar->m_visible = !(IsZeroApprox(m_vScrollBar->m_slider.globalRect.size.h));

		if (!m_hScrollBar->m_visible && m_hScrollBar->m_dragSliderData.canDragging) m_hScrollBar->m_dragSliderData.canDragging = false;
		if (!m_vScrollBar->m_visible && m_vScrollBar->m_dragSliderData.canDragging) m_vScrollBar->m_dragSliderData.canDragging = false;
	}

	void ScrollPanel::Render(Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.RenderRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::ScrollPanelBackground), true);
		BaseComponent::Render(renderer);
		m_hScrollBar->Render(renderer);
		m_vScrollBar->Render(renderer);
		renderer.SetRenderClipRect(m_visibleGRect);
		renderer.RenderRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::ScrollPanelBorder), false);
		renderer.ClearRenderClipRect();

		//debug
		//renderer.DrawRect(GetContentGlobalRect(), Color::MAGENTA);
		//renderer.DrawRect(CalcChildrenBoundaryGlobalRect(this), Color::GREEN);
	}
}