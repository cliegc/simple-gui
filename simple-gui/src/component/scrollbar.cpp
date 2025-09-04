#include "component/scrollbar.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	ScrollBar::ScrollBar(Direction direction) {
		m_direction = direction;
		m_mouseState = MouseState::Normal;
		//m_padding = SG_GuiManager.GetCurrentStyle().componentPadding;
	}

	bool ScrollBar::HandleEvent(const SDL_Event& event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		//Vec2 renderPos = SG_GuiManager.GetRenderer().GetRenderPositionFromMouse();
		Vec2 renderPos;
		if (!m_dragSliderData.canDragging && m_slider.visibleGRect.ContainPoint(renderPos)) {
			m_mouseState = MouseState::Hovering;

			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				m_mouseState = MouseState::Pressed;
				m_dragSliderData.startPos = m_slider.globalRect.position;
				m_dragSliderData.startMousePos = renderPos;
				//m_dragSliderData.dragging = true;
				m_dragSliderData.canDragging = true;
			}
			return true;
		}

		m_dragSliderData.dragging = false;

		if (m_dragSliderData.canDragging) {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
				m_mouseState = MouseState::Normal;
				m_dragSliderData.dragging = false;
				m_dragSliderData.canDragging = false;
				return true;
			}
			else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				m_dragSliderData.dragging = true;
				Vec2 pos = renderPos - m_dragSliderData.startMousePos + m_dragSliderData.startPos;
				if (m_direction == Direction::Horizontal) {
					m_slider.globalRect.position.x = pos.x;
				}
				else {
					m_slider.globalRect.position.y = pos.y;
				}
				return true;
			}
		}


		if (m_visibleGRect.ContainPoint(renderPos)) {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				// µã»÷²Û£¬»¬¿éÒÆ¶¯µ½µã»÷Î»ÖÃ

				return true;
			}

			if (event.type == SDL_EVENT_MOUSE_WHEEL) {
				// ¹öÂÖ¹ö¶¯»¬¿é

				return true;
			}
		}

		return BaseComponent::HandleEvent(event);
	}

	void ScrollBar::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		BaseComponent::Update();

		if (m_direction == Direction::Horizontal) UpdateHorizontalSlider();
		else UpdateVerticalSlider();
	}

	void ScrollBar::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		// draw slot
		renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::ScrollbarSlot));

		renderer.SetClipRect(m_visibleGRect);
		// draw slider
		if (m_mouseState == MouseState::Normal)
			renderer.FillRect(m_slider.visibleGRect, GetThemeColor(ThemeColorFlags::ScrollbarSlider));
		else if (m_mouseState == MouseState::Hovering)
			renderer.FillRect(m_slider.visibleGRect, GetThemeColor(ThemeColorFlags::ScrollbarSliderHovered));
		else if (m_mouseState == MouseState::Pressed)
			renderer.FillRect(m_slider.visibleGRect, GetThemeColor(ThemeColorFlags::ScrollbarSliderPressed));
		//draw border
		renderer.DrawRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::ScrollbarBorder));
		renderer.ClearClipRect();

		BaseComponent::Render(renderer);
	}

	void ScrollBar::UpdateHorizontalSlider() {
		Rect boundaryGRect = CalcChildrenBoundaryGlobalRect(m_target);
		Vec2 targetContentSize = GetComponentContentSize(m_target);
		if (boundaryGRect.size.w <= targetContentSize.w) {
			m_slider.globalRect.size.x = 0;
			m_slider.globalRect.size.y = 0;
			m_slider.visibleGRect.size.x = 0;
			m_slider.visibleGRect.size.y = 0;
			return;
		}

		float scale = targetContentSize.w / boundaryGRect.size.w;
		Rect contentGRect = GetContentGlobalRect();

		// update slider global rect
		m_slider.globalRect.size.w = scale * contentGRect.size.w;
		m_slider.globalRect.size.h = contentGRect.size.h;
		m_slider.globalRect.position.x = SDL_clamp(m_slider.globalRect.position.x,
			contentGRect.position.x, contentGRect.position.x + contentGRect.size.w - m_slider.globalRect.size.w);
		m_slider.globalRect.position.y = contentGRect.position.y;

		// update slider visible global rect
		m_slider.visibleGRect = m_slider.globalRect;

		// update position of children of the target
		if (!m_dragSliderData.dragging) return;

	}

	void ScrollBar::UpdateVerticalSlider() {

	}
}