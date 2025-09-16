#include "component/scrollbar.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	ScrollBar::ScrollBar(Direction direction) {
		m_direction = direction;
		m_mouseWheelDelta = 5.f;
		m_mouseState = MouseState::Normal;
		m_padding = { 5, 5, 5, 5 };
		if (m_direction == Direction::Horizontal) {
			SetSize(200, 15);
		}
		else {
			SetSize(15, 200);
		}
	}

	bool ScrollBar::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;
		if (!m_target) return false;

		Vec2 renderPos = SG_GuiManager.GetMousePosition();
		if (!m_dragSliderData.canDragging && m_slider.visibleGRect.ContainPoint(renderPos)) {
			m_mouseState = MouseState::Hovering;

			if (auto ev = event->Convert<MouseButtonEvent>();
				ev && ev->IsPressed(MouseButton::Left)) {
				m_mouseState = MouseState::Pressed;
				m_dragSliderData.startPos = m_slider.globalRect.position;
				m_dragSliderData.startMousePos = renderPos;
				//m_dragSliderData.dragging = true;
				m_dragSliderData.canDragging = true;
				return true;
			}
		}
		else if (!m_dragSliderData.canDragging && !m_slider.visibleGRect.ContainPoint(renderPos)){
			m_mouseState = MouseState::Normal;
		}

		m_dragSliderData.dragging = false;

		if (m_dragSliderData.canDragging) {
			if (auto ev = event->Convert<MouseButtonEvent>();
				ev && ev->IsReleased(MouseButton::Left)) {
				m_mouseState = MouseState::Normal;
				m_dragSliderData.dragging = false;
				m_dragSliderData.canDragging = false;
				return true;
			}
			else if (event->IsMouseMotionEvent()) {
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
			if (auto ev = event->Convert<MouseButtonEvent>();
			ev && ev->IsPressed(MouseButton::Left)) {
				// 点击槽，滑块移动到点击位置, bug
				Rect contentGRect = GetContentGlobalRect();
				if (m_direction == Direction::Horizontal) {
					float scale = (renderPos.x - contentGRect.Left()) / (contentGRect.size.w - m_slider.globalRect.size.w);
					SetScroll(scale);
				}
				else {
					float scale = (renderPos.y - contentGRect.Top()) / (contentGRect.size.h - m_slider.globalRect.size.h);
					SetScroll(scale);
				}
				return true;
			}

			if (auto ev = event->Convert<MouseWheelEvent>()) {
				// 滚轮滚动滑块
				UpdateSliderPositionByWheel(m_mouseWheelDelta, -ev->GetDirection().y);
				return true;
			}
		}

		return BaseComponent::HandleEvent(event);
	}

	void ScrollBar::Update() {
		SG_CMP_UPDATE_CONDITIONS;
		if (!m_target) return;

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
		
		// debug
		//renderer.DrawRect(CalcChildrenBoundaryGlobalRect(m_target), Color::GREEN);

		BaseComponent::Render(renderer);
	}

	void ScrollBar::UpdateHorizontalSlider() {
		// CalcChildrenBoundaryGlobalRect无需一直计算，可缓存优化
		// 在BaseComponent中添加m_childrenBoundaryGRect成员变量，在添加子组件和子组件大小改变时重新计算
		Rect boundaryGRect = CalcChildrenBoundaryGlobalRect(m_target);
		Rect targetContentGRect = m_target->GetContentGlobalRect();
		if (targetContentGRect.ContainRect(boundaryGRect)) {
			m_slider.globalRect.size.w = 0;
			m_slider.globalRect.size.h = 0;
			m_slider.visibleGRect.size.w = 0;
			m_slider.visibleGRect.size.h = 0;
			return;
		}

		// calc alignment offset
		m_alignmentOffset.x = boundaryGRect.position.x - targetContentGRect.position.x;

		float scale = targetContentGRect.size.w / boundaryGRect.size.w;
		Rect contentGRect = GetContentGlobalRect();

		// update slider global rect
		auto sliderLength = scale * contentGRect.size.w;
		m_slider.globalRect.size.w = sliderLength < m_slider.minLength ? m_slider.minLength : sliderLength;
		m_slider.globalRect.size.h = contentGRect.size.h;
		m_slider.globalRect.position.x = SDL_clamp(m_slider.globalRect.position.x,
			contentGRect.position.x, contentGRect.Right() - m_slider.globalRect.size.w);
		m_slider.globalRect.position.y = contentGRect.position.y;

		// update slider visible global rect
		m_slider.visibleGRect = CalcVisibleGlobalRect(m_visibleGRect, contentGRect, m_slider.globalRect);

		// update position of children of the target
		if (m_dragSliderData.dragging) {
			float maxMovingDistance = contentGRect.size.w - m_slider.globalRect.size.w;
			float mScale = (m_slider.globalRect.Left() - contentGRect.Left()) / maxMovingDistance;
			float distance = (boundaryGRect.size.w - targetContentGRect.size.w) * mScale;
			m_target->ForEachChild([this, distance](BaseComponent* cmp) { this->UpdateTargetChildrenPosition(cmp, -distance); });
		}
		else {
			float tScale = (targetContentGRect.position.x - boundaryGRect.position.x) / (boundaryGRect.size.w - targetContentGRect.size.w);
			float offset = tScale * (contentGRect.size.w - m_slider.globalRect.size.w);
			m_slider.globalRect.position.x = contentGRect.position.x + offset;
		}
	}

	void ScrollBar::UpdateVerticalSlider() {
		// CalcChildrenBoundaryGlobalRect无需一直计算，可缓存优化
		// 在BaseComponent中添加m_childrenBoundaryGRect成员变量，在添加子组件和子组件大小改变时重新计算
		Rect boundaryGRect = CalcChildrenBoundaryGlobalRect(m_target);
		Rect targetContentGRect = m_target->GetContentGlobalRect();
		if (targetContentGRect.ContainRect(boundaryGRect)) {
			m_slider.globalRect.size.w = 0;
			m_slider.globalRect.size.h = 0;
			m_slider.visibleGRect.size.w = 0;
			m_slider.visibleGRect.size.h = 0;
			return;
		}

		// calc alignment offset
		m_alignmentOffset.y = boundaryGRect.position.y - targetContentGRect.position.y;

		float scale = targetContentGRect.size.h / boundaryGRect.size.h;
		Rect contentGRect = GetContentGlobalRect();

		// update slider global rect
		auto sliderLength = scale * contentGRect.size.h;
		m_slider.globalRect.size.w = contentGRect.size.w;
		m_slider.globalRect.size.h = sliderLength < m_slider.minLength ? m_slider.minLength : sliderLength;
		m_slider.globalRect.position.x = contentGRect.position.x;
		m_slider.globalRect.position.y = SDL_clamp(m_slider.globalRect.position.y,
			contentGRect.position.y, contentGRect.Bottom() - m_slider.globalRect.size.h);

		// update slider visible global rect
		m_slider.visibleGRect = CalcVisibleGlobalRect(m_visibleGRect, contentGRect, m_slider.globalRect);

		// update position of children of the target
		if (m_dragSliderData.dragging) {
			float maxMovingDistance = contentGRect.size.h - m_slider.globalRect.size.h;
			float mScale = (m_slider.globalRect.Top() - contentGRect.Top()) / maxMovingDistance;
			float distance = (boundaryGRect.size.h - targetContentGRect.size.h) * mScale;
			m_target->ForEachChild([this, distance](BaseComponent* cmp) { this->UpdateTargetChildrenPosition(cmp, -distance); });
		}
		else {
			float tScale = (targetContentGRect.position.y - boundaryGRect.position.y) / (boundaryGRect.size.h - targetContentGRect.size.h);
			float offset = tScale * (contentGRect.size.h - m_slider.globalRect.size.h);
			m_slider.globalRect.position.y = contentGRect.position.y + offset;
		}
	}

	void ScrollBar::UpdateTargetChildrenPosition(BaseComponent* cmp, float distance) const {
		if (m_direction == Direction::Horizontal) {
			float x = cmp->GetPosition().x;
			cmp->SetPositionX(x + distance - m_alignmentOffset.x);
		}
		else {
			float y = cmp->GetPosition().y;
			cmp->SetPositionY(y + distance - m_alignmentOffset.y);
		}
	}

	void ScrollBar::UpdateSliderPositionByWheel(float delta, int direction) {
		direction = direction < 0 ? -1 : (direction > 0 ? 1 : 0);
		if (direction == 0) return;

		m_dragSliderData.dragging = true;
		if (m_direction == Direction::Horizontal) {
			m_slider.globalRect.position.x += delta * direction;
		}
		else {
			m_slider.globalRect.position.y += delta * direction;
		}
	}

	void ScrollBar::SetScroll(float scale) {
		scale = SDL_clamp(scale, 0.f, 1.f);
		m_dragSliderData.dragging = true;
		Rect contentGRect = GetContentGlobalRect();
		if (m_direction == Direction::Horizontal) {
			// bug, 未更新前m_slider.globalRect.size为0
			float offset = scale * (contentGRect.size.w - m_slider.globalRect.size.w);
			m_slider.globalRect.position.x = contentGRect.Left() + offset;
		}
		else {
			float offset = scale * (contentGRect.size.h - m_slider.globalRect.size.h);
			m_slider.globalRect.position.y = contentGRect.Top() + offset;
		}
	}
}