#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class ScrollBar final : public BaseComponent {
	public:
		ScrollBar(Direction direction);
		~ScrollBar() = default;

		inline BaseComponent* GetTarget() const { return m_target; }
		inline void SetTarget(BaseComponent* target) { 
			m_target = target;
			Rect boundaryGRect = CalcChildrenBoundaryGlobalRect(m_target);
			Rect targetContectGRect = m_target->GetContentGlobalRect();
			SDL_Log("SetTarget: boundaryGRect.size.w=%f\n", boundaryGRect.size.w);
			SDL_Log("SetTarget: targetContectGRect.size.w=%f\n", targetContectGRect.size.w);
		}

		void SetScroll(float scale);

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
			//float 
		};

	private:
		Direction m_direction;
		MouseState m_mouseState;
		Slider m_slider;
		DragSliderData m_dragSliderData;
		BaseComponent* m_target = nullptr;
		Vec2 m_alignmentOffset;

	private:
		void UpdateHorizontalSlider();
		void UpdateVerticalSlider();
		void UpdateTargetChildrenPosition(BaseComponent* cmp, float distance) const;
	};
}