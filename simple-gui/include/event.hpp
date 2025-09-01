#pragma once
#include <SDL3/SDL_events.h>
#include <math.hpp>


namespace SimpleGui {
	enum class EventType {
		Unknown = 0,

		MouseEvent = 1 << 1,
		MouseButtonEvent = 1 << 2,
		MouseMotionEvent = 1 << 3,
		MouseWheelEvent = 1 << 4,

		KeyBoardEvent = 1 << 5,

		WindowEvent = 1 << 6,
		WindowResizeEvent = 1 << 7,
	};

#define SG_EVENT_DETERMINE_TYPE_FUNC(type) inline bool Is##type() const { return\
											(static_cast<int>(m_type) & static_cast<int>(EventType::##type)) == \
											static_cast<int>(EventType::##type); }

	class Event {
	public:
		Event() = default;
		~Event() = default;

		SG_EVENT_DETERMINE_TYPE_FUNC(MouseEvent);
		SG_EVENT_DETERMINE_TYPE_FUNC(MouseButtonEvent);
		SG_EVENT_DETERMINE_TYPE_FUNC(MouseMotionEvent);
		SG_EVENT_DETERMINE_TYPE_FUNC(MouseWheelEvent);

		SG_EVENT_DETERMINE_TYPE_FUNC(KeyBoardEvent);

		SG_EVENT_DETERMINE_TYPE_FUNC(WindowEvent);
		SG_EVENT_DETERMINE_TYPE_FUNC(WindowResizeEvent);

	protected:
		EventType m_type = EventType::Unknown;
	};

	class MouseEvent : public Event {
	public:
		inline Vec2 GetPosition() const { return m_position; }
		inline void SetPosition(const Vec2& pos) { m_position = pos; }

		inline Vec2 GetGlobalPosition() const { return m_globalPosition; }
		inline void SetGlobalPosition(const Vec2& pos) { m_globalPosition = pos; }

	private:
		Vec2 m_position;
		Vec2 m_globalPosition;
	};

	enum class MouseButton {
		None,
		Left,
		Right,
		Middle,
		WheelUp,
		WheelDown,
	};

	class MouseButtonEvent : public MouseEvent {
	public:
		inline MouseButton GetButtonIndex() const { return m_buttonIndex; }
		inline void SetButtonIndex(MouseButton index) { m_buttonIndex = index; }

		inline bool IsPressed() const { return m_pressed; }
		inline void SetPressed(bool pressed) { m_pressed = pressed; }

		inline bool IsDoubleClick() const { return m_doubleClick; }
		inline void SetDoubleClick(bool doubleClick) { m_doubleClick = doubleClick; }

	private:
		MouseButton m_buttonIndex = MouseButton::None;
		bool m_pressed = false;
		bool m_doubleClick = false;
	};

	class MouseMotionEvent : public MouseEvent {
	public:


	private:

	};

	class MouseWheelEvent : public MouseEvent {
	public:

	private:
		Vec2 m_direction;
	};

}
