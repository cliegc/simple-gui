#include "component/slider.hpp"


namespace SimpleGui {
	Slider::Slider(Direction direction, float value, float minValue, float maxValue): Range(value, minValue, maxValue) {
		m_direction = direction;
		m_handlerRadius = 5.f;
		m_editabel = true;
		m_scrollable = true;
	}

	bool Slider::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

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
		renderer.FillCircle(m_handlerGPos, m_handlerRadius, GetThemeColor(ThemeColorFlags::SliderHandler));

		BaseComponent::Render(renderer);
	}

	void Slider::UpdateDirection() {
		float scale = GetValueToMinValueInterval() / GetInterval();

		if (m_direction == Direction::Horizontal) {

		}
		else {

		}
	}

	bool Slider::IsCircleContainPoint(const Vec2& center, float radius, const Vec2& point) {
		float l = center.DistanceSquard(point);
		float r = radius * radius;
		return l < r || IsEqualApprox(l, r);
	}

}