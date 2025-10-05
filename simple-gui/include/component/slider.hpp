#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class Slider final : public BaseComponent, public Range {
	public:
		Slider(Direction direction, float value = 0, float minValue = 0, float maxValue = 100);
		~Slider() = default;

		virtual bool HandleEvent(Event* event);
		virtual void Update();
		virtual void Render(const Renderer& renderer);

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
		Direction m_direction;
		Rect m_slotGRect;
		Rect m_valueGRect;
		Vec2 m_handlerGPos;
		float m_handlerRadius;
		bool m_editabel;
		bool m_scrollable;

	private:
		void UpdateDirection();

		static bool IsCircleContainPoint(const Vec2& center, float radius, const Vec2& point);
	};
}