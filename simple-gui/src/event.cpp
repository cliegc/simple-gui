#include "event.hpp"


namespace SimpleGui {
	EventManager::EventManager(Window* window): m_window(window) {
	}

	Event* EventManager::PollEvent() {
		if (SDL_PollEvent(&m_event)) {
			return ConvertEvent(m_event);
		}

		return nullptr;
	}

	Event* EventManager::ConvertEvent(const SDL_Event& event) const {
		switch (event.type) {
		case SDL_EVENT_QUIT: {
			auto ev = new ApplicationQuitEvent();
			ev->Setup(event.window.windowID, event.window.timestamp);
			return ev;
		}
		case SDL_EVENT_WINDOW_SHOWN: 
		case SDL_EVENT_WINDOW_HIDDEN: {
			auto ev = new WindowShowEvent();
			ev->Setup(event.window.windowID, event.window.timestamp);
			ev->m_hidden = event.window.type == SDL_EVENT_WINDOW_SHOWN ? false : true;
			return ev;
		}
		case SDL_EVENT_WINDOW_MOVED: {
			WindowMovedEvent* ev = new WindowMovedEvent();
			ev->Setup(event.window.windowID, event.window.timestamp);
			ev->m_position = Vec2(event.window.data1, event.window.data2);
			return ev;
		}
		case SDL_EVENT_WINDOW_RESIZED: {
			WindowResizedEvent* ev = new WindowResizedEvent();
			ev->Setup(event.window.windowID, event.window.timestamp);
			ev->m_size = Vec2(event.window.data1, event.window.data2);
			return ev;
		}
		case SDL_EVENT_WINDOW_MINIMIZED:
		case SDL_EVENT_WINDOW_MAXIMIZED:
		case SDL_EVENT_WINDOW_RESTORED:
		case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
		case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: {
			WindowStateChangedEvent* ev = new WindowStateChangedEvent();
			ev->Setup(event.window.windowID, event.window.timestamp);
			if (event.window.type == SDL_EVENT_WINDOW_MINIMIZED) {
				ev->m_state = WindowState::Minimized;
			}
			else if (event.window.type == SDL_EVENT_WINDOW_MAXIMIZED) {
				ev->m_state = WindowState::Maximized;
			}
			else if (event.window.type == SDL_EVENT_WINDOW_RESTORED) {
				ev->m_state = WindowState::Normal;
			}
			else if (event.window.type == SDL_EVENT_WINDOW_ENTER_FULLSCREEN) {
				ev->m_state = WindowState::FullScreen;
			}
			else {
				ev->m_state = m_window->GetState();
				ev->m_isLeaveFullScreen = true;
			}
			return ev;
		}
		case SDL_EVENT_WINDOW_MOUSE_ENTER:
		case SDL_EVENT_WINDOW_MOUSE_LEAVE: {
			auto ev = new WindowMouseEvent();
			ev->Setup(event.window.windowID, event.window.timestamp);
			ev->m_isEnter = event.window.type == SDL_EVENT_WINDOW_MOUSE_ENTER ? true : false;
			return ev;
		}
		case SDL_EVENT_WINDOW_FOCUS_GAINED:
		case SDL_EVENT_WINDOW_FOCUS_LOST: {
			auto ev = new WindowFocusEvent();
			ev->Setup(event.window.windowID, event.window.timestamp);
			ev->m_isGained = event.window.type == SDL_EVENT_WINDOW_FOCUS_GAINED ? true : false;
			return ev;
		}
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
			auto ev = new WindowCloseRequestedEvent();
			ev->Setup(event.window.windowID, event.window.timestamp);
			return ev;
		}
		case SDL_EVENT_WINDOW_DESTROYED: {
			auto ev = new WindowDestroyEvent();
			ev->Setup(event.window.windowID, event.window.timestamp);
			return ev;
		}
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP: {
			auto ev = new MouseButtonEvent();
			ev->Setup(event, *m_window, Vec2(event.button.x, event.button.y));
			ev->m_pressed = event.button.down;
			ev->m_doubleClick = event.button.clicks == 2 ? true : false;
			if (event.button.button == SDL_BUTTON_LEFT) ev->m_buttonIndex = MouseButton::Left;
			if (event.button.button == SDL_BUTTON_MIDDLE) ev->m_buttonIndex = MouseButton::Middle;
			if (event.button.button == SDL_BUTTON_RIGHT) ev->m_buttonIndex = MouseButton::Right;
			return ev;
		}
		case SDL_EVENT_MOUSE_MOTION: {
			auto ev = new MouseMotionEvent();
			ev->Setup(event, *m_window, Vec2(event.motion.x, event.motion.y));
			ev->m_direction.x = event.motion.xrel;
			ev->m_direction.y = event.motion.yrel;
			return ev;
		}
		case SDL_EVENT_MOUSE_WHEEL: {
			auto ev = new MouseWheelEvent();
			ev->Setup(event, *m_window, Vec2(event.wheel.mouse_x, event.wheel.mouse_y));
			ev->m_direction.x = event.wheel.x;
			ev->m_direction.y = event.wheel.y;
			return ev;
		}
		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP: {
			auto ev = new KeyBoardButtonEvent();
			ev->Setup(event);
			ev->m_pressed = event.key.down;
			ev->m_repeat = event.key.repeat;
			return ev;
		}
		case SDL_EVENT_DROP_FILE:
		case SDL_EVENT_DROP_TEXT:
		case SDL_EVENT_DROP_POSITION:{
			auto ev = new DropEvent();
			ev->Setup(event.drop.windowID, event.drop.timestamp);
			if (event.drop.type == SDL_EVENT_DROP_FILE || event.drop.type == SDL_EVENT_DROP_TEXT) {
				ev->m_content = event.drop.data;
				ev->m_itemType = event.drop.type == SDL_EVENT_DROP_FILE ? DropEvent::DropItemType::File : DropEvent::DropItemType::Text;
			}
			ev->m_position = Vec2(event.drop.x, event.drop.y);
			return ev;
		}
		}

		return nullptr;
	}

	void EventManager::FreeEvent(Event* event) const {
		delete event;
	}
}