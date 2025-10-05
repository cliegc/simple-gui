#include "component/slider.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	Slider::Slider(Direction direction, float value, float minValue, float maxValue): Range(value, minValue, maxValue) {
		m_direction = direction;
		m_handler.radius = 5.f;
		m_editabel = true;
		m_scrollable = true;
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
	}

	void Slider::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.FillRect(m_slotGRect, GetThemeColor(ThemeColorFlags::SliderSlot));
		renderer.FillRect(m_valueGRect, GetThemeColor(ThemeColorFlags::SliderValue));
		renderer.FillCircle(m_handler.globalPositon, m_handler.radius, GetThemeColor(ThemeColorFlags::SliderHandler));

		BaseComponent::Render(renderer);
	}

	void Slider::UpdateDirection() {
		float scale = GetValueToMinValueInterval() / GetInterval();

		if (m_direction == Direction::Horizontal) {

		}
		else {

		}
	}

	bool Slider::HandleDragHandler(Event* event) {
		Vec2 mousePos = SG_GuiManager.GetMousePosition();
		if (!IsCircleContainPoint(m_handler.globalPositon, m_handler.radius, mousePos)) return false;

		if (auto ev = event->Convert<MouseButtonEvent>();
			ev && ev->IsPressed(MouseButton::Left)) {

		}

		return false;
	}

	bool Slider::HandleMouseClickedToChangeValue(Event* event) {
		return false;
	}

	bool Slider::HandleMouseWheelToChangeValue(Event* event) {
		return false;
	}

	bool Slider::IsCircleContainPoint(const Vec2& center, float radius, const Vec2& point) {
		float l = center.DistanceSquard(point);
		float r = radius * radius;
		return l < r || IsEqualApprox(l, r);
	}

}