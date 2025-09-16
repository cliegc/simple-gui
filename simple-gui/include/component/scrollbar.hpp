#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class ScrollBar final : public BaseComponent {
		friend class ScrollPanel;
	public:
		ScrollBar(Direction direction);
		~ScrollBar() = default;

		inline BaseComponent* GetTarget() const { return m_target; }
		inline void SetTarget(BaseComponent* target) { m_target = target; }

		void SetScroll(float scale);

		inline float GetMouseWheelDelta() const { return m_mouseWheelDelta; }
		inline void SetMouseWheelDelta(float delta) { m_mouseWheelDelta = delta; }

		virtual bool HandleEvent(Event* event) override;
		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

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
		void UpdateHorizontalSlider();
		void UpdateVerticalSlider();
		void UpdateTargetChildrenPosition(BaseComponent* cmp, float distance) const;
		void UpdateSliderPositionByWheel(float delta, int direction);
	};
}