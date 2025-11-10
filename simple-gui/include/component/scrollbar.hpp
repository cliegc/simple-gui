#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class ScrollBar final : public BaseComponent {
		friend class ScrollPanel;
	public:
		explicit ScrollBar(Direction direction = Direction::Vertical);
		~ScrollBar() override = default;

		BaseComponent* GetTarget() const { return m_target; }
		void SetTarget(BaseComponent* target) { m_target = target; }

		Direction GetDirection() const { return m_direction; }
		void SetDirection(Direction direction) { m_direction = direction; }

		void SetScroll(float scale);

		float GetMouseWheelDelta() const { return m_mouseWheelDelta; }
		void SetMouseWheelDelta(float delta) { m_mouseWheelDelta = delta; }

		bool HandleEvent(Event* event) override;
		void Update() override;
		void Render(Renderer& renderer) override;

	private:
		struct DragSliderData {
			Vec2 startPos;
			Vec2 startMousePos;
			bool canDragging = false;
			bool dragging = false;
		};

		struct Slider final {
			Rect globalRect;
			Rect visibleGRect;
			float minLength = 10;
		};

	private:
		Direction m_direction;
		MouseState m_mouseState;
		Slider m_slider;
		DragSliderData m_dragSliderData;
		BaseComponent* m_target = nullptr;
		Vec2 m_alignmentOffset;
		float m_mouseWheelDelta;

	private:
		void UpdateHorizontalSlider(const Rect& boundaryGRect, const Rect& targetContentGRect);
		void UpdateVerticalSlider(const Rect& boundaryGRect, const Rect& targetContentGRect);
		void UpdateTargetChildrenPosition(BaseComponent* cmp, float distance) const;
		void UpdateSliderPositionByWheel(float delta, int direction);
		static Rect AdjustTargetChildrenBoundaryGlobalRect(const Rect& boundaryGRect, const Rect& targetContentGRect);
	};
}