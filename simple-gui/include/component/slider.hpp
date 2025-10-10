#pragma once
#include "base_component.hpp"
#include "common/range.hpp"


namespace SimpleGui {
	class Slider final : public BaseComponent, public Range {
	public:
		explicit Slider(Direction direction = Direction::Horizontal, float value = 0, float minValue = 0, float maxValue = 100);
		~Slider() override = default;

		bool HandleEvent(Event* event) override;
		void Update() override;
		void Render(Renderer& renderer) override;

		Direction GetDirection() const { return m_direction; }
		void SetDirection(Direction direction);

		bool IsEditable() const { return m_editable; }
		void SetEditable(bool value) { m_editable = value; }

		bool IsScrollable() const { return m_scrollable; }
		void SetScrollable(bool value) { m_scrollable = value; }

	private:
		struct DragData {
			Vec2 mouseStartPos;
			Vec2 handleStartPos;
			bool canDrag{};
			bool dragging{};
		};

	private:
		Direction m_direction{};
		ComponentElementRect m_slotRect{};
		ComponentElementRect m_valueRect{};
		ComponentElementRect m_handlerRect{};
		DragData m_dragData{};
		MouseState m_mouseState{};
		bool m_editable{};
		bool m_scrollable{};

	private:
		void UpdateDirection();
		bool HandleDragHandler(Event* event);
		bool HandleMouseClickedToChangeValue(Event* event);
		bool HandleMouseWheelToChangeValue(Event* event);

		static bool IsCircleContainPoint(const Vec2& center, float radius, const Vec2& point);
	};
}