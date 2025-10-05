#pragma once
#include "base_component.hpp"
#include "common/range.hpp"


namespace SimpleGui {
	class Slider final : public BaseComponent, public Range {
	public:
		Slider(Direction direction = Direction::Horizontal, float value = 0, float minValue = 0, float maxValue = 100);
		~Slider() = default;

		virtual bool HandleEvent(Event* event) override;
		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

		inline Direction GetDirection() const { return m_direction; }
		void SetDirection(Direction direction);

		inline bool IsEditable() const { return m_editabel; }
		inline void SetEditbale(bool value) { m_editabel = value; }

		inline bool IsScrollable() const { return m_scrollable; }
		inline void SetScrollable(bool value) { m_scrollable = value; }

	private:
		struct DragData {
			Vec2 mouseStartPos;
			Vec2 handleStartPos;
			bool canDrag;
			bool dragging;
		};

	private:
		Direction m_direction{};
		ComponentElementRect m_slotRect{};
		ComponentElementRect m_valueRect{};
		ComponentElementRect m_handlerRect{};
		DragData m_dragData{};
		MouseState m_mouseState{};
		bool m_editabel{};
		bool m_scrollable{};

	private:
		void UpdateDirection();
		bool HandleDragHandler(Event* event);
		bool HandleMouseClickedToChangeValue(Event* event);
		bool HandleMouseWheelToChangeValue(Event* event);

		static bool IsCircleContainPoint(const Vec2& center, float radius, const Vec2& point);
	};
}