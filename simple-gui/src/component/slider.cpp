#include "component/slider.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	const Vec2 H_HANDLER_SIZE = Vec2(8, 15);
	const Vec2 V_HANDLER_SIZE = Vec2(15, 8);

	Slider::Slider(Direction direction, float value, float minValue, float maxValue) : Range(value, minValue, maxValue) {
		m_editabel = true;
		m_scrollable = true;
		SetDirection(direction);
	}

	bool Slider::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		if (BaseComponent::HandleEvent(event)) return true;

		if (HandleDragHandler(event)) return true;
		if (HandleMouseClickedToChangeValue(event)) return true;
		if (HandleMouseWheelToChangeValue(event)) return true;

		return false;
	}

	void Slider::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		BaseComponent::Update();
		UpdateDirection();

		Rect contentGRect = GetContentGlobalRect();
		m_slotRect.visibleGRect = CalcVisibleGlobalRect(m_visibleGRect, contentGRect, m_slotRect.gRect);
		m_valueRect.visibleGRect = CalcVisibleGlobalRect(m_visibleGRect, contentGRect, m_valueRect.gRect);
		m_handlerRect.visibleGRect = CalcVisibleGlobalRect(m_visibleGRect, contentGRect, m_handlerRect.gRect);
	}

	void Slider::Render(Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.RenderRect(m_slotRect.visibleGRect, GetThemeColor(ThemeColorFlags::SliderSlot), true);
		renderer.RenderRect(m_valueRect.visibleGRect, GetThemeColor(ThemeColorFlags::SliderValue), true);
		if (m_mouseState == MouseState::Hovering) {
			renderer.RenderRect(m_handlerRect.visibleGRect, GetThemeColor(ThemeColorFlags::SliderHandlerHovered), true);
		}
		else {
			renderer.RenderRect(m_handlerRect.visibleGRect, GetThemeColor(ThemeColorFlags::SliderHandler), true);
		}

		BaseComponent::Render(renderer);

		//debug
		//renderer.DrawRect(GetGlobalRect(), Color::GREEN);
	}

	void Slider::UpdateDirection() {
		Rect contentGRect = GetContentGlobalRect();
		Vec2 contentCenterGPos = contentGRect.Center();

		if (m_direction == Direction::Horizontal) {
			m_slotRect.gRect.position.x = contentGRect.Left() + m_handlerRect.gRect.size.w / 2;
			m_slotRect.gRect.position.y = contentCenterGPos.y - m_slotRect.gRect.size.h / 2;
			m_slotRect.gRect.size.w = contentGRect.size.w - m_handlerRect.gRect.size.w;

			if (!m_dragData.dragging) {
				float scale = GetValueToMinValueInterval() / GetInterval();
				m_handlerRect.gRect.position.x = scale * m_slotRect.gRect.size.w + m_slotRect.gRect.Left() - m_handlerRect.gRect.size.w / 2;
			}

			m_handlerRect.gRect.position.y = contentCenterGPos.y - m_handlerRect.gRect.size.h / 2;
			m_valueRect.gRect.position = m_slotRect.gRect.position;
			m_valueRect.gRect.size.w = m_handlerRect.gRect.Center().x - contentGRect.Left();
		}
		else {
			m_slotRect.gRect.position.x = contentCenterGPos.x - m_slotRect.gRect.size.w / 2;
			m_slotRect.gRect.position.y = contentGRect.Top() + m_handlerRect.gRect.size.h / 2;
			m_slotRect.gRect.size.h = contentGRect.size.h - m_handlerRect.gRect.size.h;

			if (!m_dragData.dragging) {
				float scale = GetValueToMinValueInterval() / GetInterval();
				m_handlerRect.gRect.position.y = m_slotRect.gRect.Bottom() - scale * m_slotRect.gRect.size.h - m_handlerRect.gRect.size.h / 2;
			}

			m_handlerRect.gRect.position.x = contentCenterGPos.x - m_handlerRect.gRect.size.w / 2;
			m_valueRect.gRect.position.x = m_slotRect.gRect.position.x;
			m_valueRect.gRect.position.y = m_handlerRect.gRect.Center().y;
			m_valueRect.gRect.size.w = m_slotRect.gRect.size.w;
			m_valueRect.gRect.size.h = m_slotRect.gRect.Bottom() - m_handlerRect.gRect.Center().y;
		}
	}

	bool Slider::HandleDragHandler(Event* event) {
		if (!m_editabel) return false;

		Vec2 mousePos = SG_GuiManager.GetMousePosition();
		m_mouseState = MouseState::Normal;

		if (!m_dragData.dragging &&
			m_handlerRect.gRect.ContainPoint(mousePos)) {
			m_mouseState = MouseState::Hovering;

			if (auto ev = event->Convert<MouseButtonEvent>();
				ev && ev->IsPressed(MouseButton::Left)) {
				m_dragData.mouseStartPos = ev->GetPosition();
				m_dragData.handleStartPos = m_handlerRect.gRect.position;
				m_dragData.canDrag = true;
			}
			return true;
		}

		if (m_dragData.canDrag || m_dragData.dragging) {
			if (auto ev = event->Convert<MouseButtonEvent>();
				ev && ev->IsReleased(MouseButton::Left)) {
				m_mouseState = MouseState::Normal;
				m_dragData.canDrag = false;
				m_dragData.dragging = false;
				return true;
			}
		}

		if (auto ev = event->Convert<MouseMotionEvent>();
			ev && m_dragData.canDrag) {
			m_mouseState = MouseState::Hovering;
			m_dragData.dragging = true;
			m_handlerRect.gRect.position = m_dragData.handleStartPos + ev->GetPosition() - m_dragData.mouseStartPos;

			float scale = 1.f;
			if (m_direction == Direction::Horizontal) {
				m_handlerRect.gRect.position.x = Clamp(
					m_handlerRect.gRect.position.x,
					m_slotRect.gRect.Left() - m_handlerRect.gRect.size.w / 2,
					m_slotRect.gRect.Right() - m_handlerRect.gRect.size.w / 2 );

				scale = (m_handlerRect.gRect.Center().x - m_slotRect.gRect.Left()) / m_slotRect.gRect.size.w;
			}
			else {
				m_handlerRect.gRect.position.y = Clamp(
					m_handlerRect.gRect.position.y,
					m_slotRect.gRect.Top() - m_handlerRect.gRect.size.h / 2,
					m_slotRect.gRect.Bottom() - m_handlerRect.gRect.size.h / 2);

				scale = (m_slotRect.gRect.Bottom() - m_handlerRect.gRect.Center().y) / m_slotRect.gRect.size.h;
			}

			SetValue(scale * GetInterval() + GetMinValue());
			return true;
		}

		return false;
	}

	bool Slider::HandleMouseClickedToChangeValue(Event* event) {
		if (!m_editabel) return false;

		if (auto ev = event->Convert<MouseButtonEvent>();
			ev && ev->IsPressed(MouseButton::Left)) {
			if (!m_slotRect.gRect.ContainPoint(ev->GetPosition())) return false;

			float scale = 1.f;
			if (m_direction == Direction::Horizontal) {
				scale = (ev->GetPosition().x - m_slotRect.gRect.Left()) / m_slotRect.gRect.size.w;
			}
			else {
				scale = (m_slotRect.gRect.Bottom() - ev->GetPosition().y) / m_slotRect.gRect.size.h;
			}

			SetValue(scale * GetInterval() + GetMinValue());
			return true;
		}

		return false;
	}

	bool Slider::HandleMouseWheelToChangeValue(Event* event) {
		if (!m_editabel || !m_scrollable) return false;

		if (auto ev = event->Convert<MouseWheelEvent>();
			ev && m_visibleGRect.ContainPoint(ev->GetPosition())) {
			float value = GetValue();
			if (ev->GetDirection().y > 0) {
				value += 1;
			}
			else if (ev->GetDirection().y < 0) {
				value -= 1;
			}
			SetValue(value);
			return true;
		}
		return false;
	}

	bool Slider::IsCircleContainPoint(const Vec2& center, float radius, const Vec2& point) {
		float l = center.DistanceSquard(point);
		float r = radius * radius;
		return l < r || IsEqualApprox(l, r);
	}

	void Slider::SetDirection(Direction direction) {
		m_direction = direction;

		if (m_direction == Direction::Horizontal) {
			m_slotRect.gRect.size.h = 5;
			m_handlerRect.gRect.size = H_HANDLER_SIZE;
			m_valueRect.gRect.size.h = m_slotRect.gRect.size.h;
		}
		else {
			m_slotRect.gRect.size.w = 5;
			m_handlerRect.gRect.size = V_HANDLER_SIZE;
			m_valueRect.gRect.size.w = m_slotRect.gRect.size.w;
		}
	}

}