#pragma once
#include <SDL3/SDL_events.h>
#include <chrono>
#include "math.hpp"
#include "window.hpp"


namespace SimpleGui {
	enum class EventType {
		Unknown = 0,

		ApplicationQuitEvent,

		WindowEvent = 1 << 1,
		WindowShowEvent = 1 << 2,
		WindowMovedEvent = 1 << 3,
		WindowResizedEvent = 1 << 4,
		WindowStateChangedEvent = 1 << 5,
		WindowMouseEvent = 1 << 6,
		WindowFocusEvent = 1 << 7,
		WindowCloseRequestedEvent = 1 << 8,
		WindowDestroyEvent = 1 << 9,

		MouseEvent = 1 << 10,
		MouseButtonEvent = 1 << 11,
		MouseMotionEvent = 1 << 12,
		MouseWheelEvent = 1 << 13,

		KeyBoardEvent = 1 << 14,
		KeyBoardButtonEvent = 1 << 15,
		KeyBoardTextInputEvent = 1 << 16,
		KeyBoardTextEditingEvent = 1 << 17,

		DropEvent = 1 << 18,
	};

	inline constexpr EventType operator|(EventType type1, EventType type2) {
		return static_cast<EventType>(static_cast<int>(type1) | static_cast<int>(type2));
	}

	inline constexpr EventType operator&(EventType type1, EventType type2) {
		return static_cast<EventType>(static_cast<int>(type1) & static_cast<int>(type2));
	}

#define SG_EVENT_GET_TYPE(type) public: inline constexpr virtual EventType GetType() const override { return (type); } \
								inline static constexpr EventType GetStaticType() { return type; }
#define SG_EVENT_DETERMINE_TYPE_FUNC(type, type_name) inline constexpr bool Is##type_name() const { return (GetType() & type) == type; }

	class Event {
		friend class EventManager;

	public:
		Event() = default;
		~Event() = default;

		inline WindowID GetWindowID() const { return m_winID; }
		inline Uint64 GetTimestamp() const { return m_timestamp; }
		inline constexpr virtual EventType GetType() const { return EventType::Unknown; }

		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::ApplicationQuitEvent, ApplicationQuitEvent)

		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::WindowEvent, WindowEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::WindowShowEvent, WindowShowEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::WindowMovedEvent, WindowMovedEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::WindowResizedEvent, WindowResizedEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::WindowStateChangedEvent, WindowStateChangedEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::WindowMouseEvent, WindowMouseEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::WindowFocusEvent, WindowFocusEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::WindowCloseRequestedEvent, WindowCloseRequestedEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::WindowDestroyEvent, WindowDestroyEvent)

		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::MouseEvent, MouseEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::MouseButtonEvent, MouseButtonEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::MouseMotionEvent, MouseMotionEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::MouseWheelEvent, MouseWheelEvent)

		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::KeyBoardEvent, KeyBoardEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::KeyBoardButtonEvent, KeyBoardButtonEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::KeyBoardTextInputEvent, KeyBoardTextInputEvent)
		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::KeyBoardTextEditingEvent, KeyBoardTextEditingEvent)

		SG_EVENT_DETERMINE_TYPE_FUNC(EventType::DropEvent, DropEvent)

		template<typename T>
		T* Convert() {
			static_assert(std::is_base_of<Event, T>::value, "T 必须继承自Event");
			if (GetType() == T::GetStaticType()) {
				return static_cast<T*>(this);
			}
			return nullptr;
		}

	protected:
		WindowID m_winID;
		Uint64 m_timestamp = 0;

	protected:
		inline void Setup(WindowID id, Uint64 timestamp) {
			m_winID = id;
			m_timestamp = timestamp;
		}
	};

	class ApplicationQuitEvent final : public Event {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::ApplicationQuitEvent)
	};

#pragma region Window Event
	class WindowEvent : public Event {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::WindowEvent)
	};

	class WindowShowEvent final : public WindowEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::WindowEvent | EventType::WindowShowEvent)
	public:

		inline bool IsShown() const { return !m_hidden; }
		inline bool IsHidden() const { return m_hidden; }

	private:
		bool m_hidden = false;
	};

	class WindowMovedEvent final : public WindowEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::WindowEvent | EventType::WindowMovedEvent)
	public:

		inline Vec2 GetPosition() const { return m_position; }

	private:
		Vec2 m_position;
	};

	class WindowResizedEvent final : public WindowEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::WindowEvent | EventType::WindowResizedEvent)
	public:

		inline Vec2 GetSize() const { return m_size; }

	private:
		Vec2 m_size;
	};

	class WindowStateChangedEvent final : public WindowEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::WindowEvent | EventType::WindowStateChangedEvent)
	public:

		inline WindowState GetState() const { return m_state; }
		inline bool IsLeaveFullScreen() const { return m_isLeaveFullScreen; }

	private:
		WindowState m_state;
		bool m_isLeaveFullScreen = false;
	};

	class WindowMouseEvent final : public WindowEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::WindowEvent | EventType::WindowMouseEvent)
	public:

		inline bool IsEnter() const { return m_isEnter; }
		inline bool IsLeave() const { return !m_isEnter; }

	private:
		bool m_isEnter = false;
	};

	class WindowFocusEvent final : public WindowEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::WindowEvent | EventType::WindowFocusEvent)
	public:

		inline bool IsGained() const { return m_isGained; }
		inline bool IsLost() const { return !m_isGained; }

	private:
		bool m_isGained = false;
	};

	class WindowCloseRequestedEvent final : public WindowEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::WindowEvent | EventType::WindowCloseRequestedEvent)
	public:
	};

	class WindowDestroyEvent final :public WindowEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::WindowEvent | EventType::WindowDestroyEvent)
	};
#pragma endregion

#pragma region Mouse Event
	class MouseEvent : public Event {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::MouseEvent)
	public:

		inline Vec2 GetPosition() const { return m_position; }
		inline Vec2 GetGlobalPosition() const { return m_globalPosition; }

	protected:
		Vec2 m_position;
		Vec2 m_globalPosition;

		inline void Setup(const SDL_Event& event, const Window& win, const Vec2& pos) {
			m_winID = event.button.windowID;
			m_timestamp = event.button.timestamp;

			SDL_RenderCoordinatesFromWindow(
				SDL_GetRenderer(SDL_GetWindowFromID(m_winID)),
				pos.x, pos.y,
				&m_position.x, &m_position.y);
			m_globalPosition = win.GetPosition() + m_position;
		}
	};

	enum class MouseButton {
		None,
		Left,
		Right,
		Middle,
	};

	class MouseButtonEvent final : public MouseEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::MouseEvent | EventType::MouseButtonEvent)
	public:

		inline MouseButton GetButtonIndex() const { return m_buttonIndex; }
		inline bool IsPressed() const { return m_pressed; }
		inline bool IsPressed(MouseButton btn) const { return m_pressed && btn == m_buttonIndex; }
		inline bool IsReleased() const { return !m_pressed; }
		inline bool IsReleased(MouseButton btn) const { return !m_pressed && btn == m_buttonIndex; }
		inline bool IsDoubleClick() const { return m_doubleClick; }
		inline bool IsDoubleClick(MouseButton btn) const { return m_doubleClick && btn == m_buttonIndex; }
		inline uint8_t GetClickCount() const { return m_clicks; }

	private:
		MouseButton m_buttonIndex = MouseButton::None;
		uint8_t m_clicks;
		bool m_pressed = false;
		bool m_doubleClick = false;
	};

	class MouseMotionEvent final : public MouseEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::MouseEvent | EventType::MouseMotionEvent)
	public:

		// 返回的向量没有归一化
		inline Vec2 GetDirection() const { return m_direction; }

	private:
		Vec2 m_direction;
	};

	class MouseWheelEvent final : public MouseEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::MouseEvent | EventType::MouseWheelEvent)
	public:

		inline Vec2 GetDirection() const { return m_direction; }

	private:
		Vec2 m_direction;
	};
#pragma endregion

#pragma region KeyBoard Event
	using ScanCode = SDL_Scancode;
	using KeyCode = SDL_Keycode;
	using KeyMod = SDL_Keymod;

	class KeyBoardEvent : public Event {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::KeyBoardEvent)
	};

	class KeyBoardButtonEvent final : public KeyBoardEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::KeyBoardEvent | EventType::KeyBoardButtonEvent)

	public:
		inline ScanCode GetScanCode() const { return m_scanCode; }
		inline KeyCode GetKeyCode() const { return m_key; }
		inline KeyMod GetKeyMod() const { return m_mod; }

		inline bool IsPressed() const { return m_pressed; }
		inline bool IsReleased() const { return !m_pressed; }
		inline bool IsRepeat() const { return m_repeat; }

	private:
		ScanCode m_scanCode;
		KeyCode m_key;
		KeyMod m_mod;
		bool m_pressed = false;
		bool m_repeat = false;

		inline void Setup(const SDL_Event& event) {
			m_winID = event.key.windowID;
			m_timestamp = event.key.timestamp;
			m_scanCode = event.key.scancode;
			m_key = event.key.scancode;
			m_mod = event.key.mod;
			m_pressed = event.key.down;
			m_repeat = event.key.repeat;
		}
	};

	class KeyBoardTextInputEvent final : public KeyBoardEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::KeyBoardEvent | EventType::KeyBoardTextInputEvent)

	public:
		inline std::string GetInputText() const { return m_text; }

	private:
		std::string m_text;
	};

	class KeyBoardTextEditingEvent final : public KeyBoardEvent {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::KeyBoardEvent | EventType::KeyBoardTextEditingEvent)

	public:
		inline std::string GetEditingText() const { return m_text; }
		inline Sint32 GetSelectedEditingTextStartCursorPos() const { return m_start; }
		inline Sint32 GetSelectedEditingTextLength() const { return m_length; }

	private:
		std::string m_text;
		Sint32 m_start;
		Sint32 m_length;
	};
#pragma endregion

#pragma region Drop Event
	class DropEvent final : public Event {
		friend class EventManager;
		SG_EVENT_GET_TYPE(EventType::DropEvent)
	public:

		inline std::string GetContent() const { return m_content; }
		inline bool IsDropFile() const { return m_itemType == DropItemType::File; }
		inline bool IsDropText() const { return m_itemType == DropItemType::Text; }
		inline Vec2 GetPosition() const { return m_position; }

	private:
		enum class DropItemType {
			File,
			Text,
		};

	private:
		std::string m_content;
		DropItemType m_itemType = DropItemType::File;
		Vec2 m_position;
	};
#pragma endregion

#pragma region Event Pool
	//template<typename T>
	//struct EventItem final {
	//	bool active;
	//	std::unique_ptr<T> event;
	//};

	//template<typename T>
	//class EventPool final {
	//	friend class EventManager;
	//public:
	//	~EventCaches() = default;

	//	T* Get() {
	//	}

	//private:
	//	std::vector<EventItem<T>> m_activeEvents;
	//	std::vector<EventItem<T>> m_inactiveEvents;
	//	size_t m_size;
	//	bool m_canExpand;

	//	EventPool(size_t size);
	//};
#pragma endregion

	class EventManager final {
		friend class GuiManager;
	public:
		EventManager(Window*);
		~EventManager() = default;

		EventManager(const EventManager&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(EventManager&&) = delete;

	private:
		SDL_Event m_event{};
		Window* m_window;

		Event* PollEvent();
		Event* ConvertEvent(const SDL_Event& event) const;
		void FreeEvent(Event* event) const;
	};
}
