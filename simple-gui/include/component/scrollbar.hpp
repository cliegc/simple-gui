#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class ScrollBar final : public BaseComponent {
	public:
		ScrollBar(Direction direction);
		~ScrollBar() = default;

		inline BaseComponent* GetTarget() const { return m_target; }
		inline void SetTarget(BaseComponent* target) { m_target = target; }

		virtual bool HandleEvent(const SDL_Event& event) override;
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
		};

	private:
		Direction m_direction;
		MouseState m_mouseState;
		Slider m_slider;

		DragSliderData m_dragSliderData;

		BaseComponent* m_target = nullptr;;

	private:
		void UpdateHorizontalSlider();
		void UpdateVerticalSlider();
	};
}