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
		m_slotGRect = CalcVisibleGlobalRect(m_visibleGRect, contentGRect, m_slotGRect);
		m_valueGRect = CalcVisibleGlobalRect(m_visibleGRect, contentGRect, m_valueGRect);
		m_handlerVisibelGRect = CalcVisibleGlobalRect(m_visibleGRect, contentGRect, m_handlerGRect);
	}

	void Slider::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.FillRect(m_slotGRect, GetThemeColor(ThemeColorFlags::SliderSlot));
		renderer.FillRect(m_valueGRect, GetThemeColor(ThemeColorFlags::SliderValue));
		if (m_mouseState == MouseState::Hovering) {
			renderer.FillRect(m_handlerVisibelGRect, GetThemeColor(ThemeColorFlags::SliderHandlerHovered));
		}
		else {
			renderer.FillRect(m_handlerVisibelGRect, GetThemeColor(ThemeColorFlags::SliderHandler));
		}

		BaseComponent::Render(renderer);

		//debug
		//renderer.DrawRect(GetGlobalRect(), Color::GREEN);
	}

	void Slider::UpdateDirection() {
		Rect contentGRect = GetContentGlobalRect();
		Vec2 contentCenterGPos = contentGRect.Center();

		if (m_direction == Direction::Horizontal) {
			m_slotGRect.position.x = contentGRect.Left() + m_handlerGRect.size.w / 2;
			m_slotGRect.position.y = contentCenterGPos.y - m_slotGRect.size.h / 2;
			m_slotGRect.size.w = contentGRect.size.w - m_handlerGRect.size.w;

			if (!m_dragData.dragging) {
				float scale = GetValueToMinValueInterval() / GetInterval();
				m_handlerGRect.position.x = scale * m_slotGRect.size.w + m_slotGRect.Left() - m_handlerGRect.size.w / 2;
			}

			m_handlerGRect.position.y = contentCenterGPos.y - m_handlerGRect.size.h / 2;
			m_valueGRect.position = m_slotGRect.position;
			m_valueGRect.size.w = m_handlerGRect.Center().x - contentGRect.Left();
		}
		else {
			m_slotGRect.position.x = contentCenterGPos.x - m_slotGRect.size.w / 2;
			m_slotGRect.position.y = contentGRect.Top() + m_handlerGRect.size.h / 2;
			m_slotGRect.size.h = contentGRect.size.h - m_handlerGRect.size.h;

			if (!m_dragData.dragging) {
				float scale = GetValueToMinValueInterval() / GetInterval();
				m_handlerGRect.position.y = m_slotGRect.Bottom() - scale * m_slotGRect.size.h - m_handlerGRect.size.h / 2;
			}

			m_handlerGRect.position.x = contentCenterGPos.x - m_handlerGRect.size.w / 2;
			m_valueGRect.position.x = m_slotGRect.position.x;
			m_valueGRect.position.y = m_handlerGRect.Center().y;
			m_valueGRect.size.w = m_slotGRect.size.w;
			m_valueGRect.size.h = m_slotGRect.Bottom() - m_handlerGRect.Center().y;
		}
	}

	bool Slider::HandleDragHandler(Event* event) {
		if (!m_editabel) return false;

		Vec2 mousePos = SG_GuiManager.GetMousePosition();
		m_mouseState = MouseState::Normal;

		if (!m_dragData.dragging &&
			m_handlerGRect.ContainPoint(mousePos)) {
			m_mouseState = MouseState::Hovering;

			if (auto ev = event->Convert<MouseButtonEvent>();
				ev && ev->IsPressed(MouseButton::Left)) {
				m_dragData.mouseStartPos = ev->GetPosition();
				m_dragData.handleStartPos = m_handlerGRect.position;
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
			m_handlerGRect.position = m_dragData.handleStartPos + ev->GetPosition() - m_dragData.mouseStartPos;

			float scale = 1.f;
			if (m_direction == Direction::Horizontal) {
				m_handlerGRect.position.x = Clamp(
					m_handlerGRect.position.x,
					m_slotGRect.Left() - m_handlerGRect.size.w / 2,
					m_slotGRect.Right() - m_handlerGRect.size.w / 2 );

				scale = (m_handlerGRect.Center().x - m_slotGRect.Left()) / m_slotGRect.size.w;
			}
			else {
				m_handlerGRect.position.y = Clamp(
					m_handlerGRect.position.y,
					m_slotGRect.Top() - m_handlerGRect.size.h / 2,
					m_slotGRect.Bottom() - m_handlerGRect.size.h / 2);

				scale = (m_slotGRect.Bottom() - m_handlerGRect.Center().y) / m_slotGRect.size.h;
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
			if (!m_slotGRect.ContainPoint(ev->GetPosition())) return false;

			float scale = 1.f;
			if (m_direction == Direction::Horizontal) {
				scale = (ev->GetPosition().x - m_slotGRect.Left()) / m_slotGRect.size.w;
			}
			else {
				scale = (m_slotGRect.Bottom() - ev->GetPosition().y) / m_slotGRect.size.h;
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
			m_slotGRect.size.h = 5;
			m_handlerGRect.size = H_HANDLER_SIZE;
			m_valueGRect.size.h = m_slotGRect.size.h;
		}
		else {
			m_slotGRect.size.w = 5;
			m_handlerGRect.size = V_HANDLER_SIZE;
			m_valueGRect.size.w = m_slotGRect.size.w;
		}
	}

}