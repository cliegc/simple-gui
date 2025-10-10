#pragma once
#include <SDL3/SDL_video.h>
#include <string>
#include <string_view>
#include <memory>
#include <optional>
#include "renderer.hpp"
#include "style.hpp"
#include "font.hpp"
#include "component/root_component.hpp"


namespace SimpleGui {
	class Event;

	using WindowID = SDL_WindowID;

	enum class WindowState {
		Normal,
		Minimized,
		Maximized,
		FullScreen,
	};

	class Window final {
	public:
		Window(std::string_view title, int w, int h);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

		std::string GetTitle() const;
		void SetTitle(std::string_view title) const;

		void SetIcon(std::string_view path) const;

		Vec2 GetPosition() const;
		void SetPosition(int x, int y) const;

		Vec2 GetSize() const;
		void SetSize(int w, int h) const;

		bool IsResizable() const;
		void SetResizable(bool resizable) const;

		bool IsAllwaysOnTop() const;
		void SetAllwaysOnTop(bool top) const;

		bool IsFullScreen() const;
		void SetFullScreen(bool full) const;

		bool IsMaximized() const;
		void ToggleMaximize() const;

		bool IsMinimized() const;
		void ToggleMinimize() const;

		WindowState GetState() const;
		void SetState(WindowState state);

		WindowID GetID() const;

		bool IsEnabledVsync() const;
		bool EnableVsync(bool enable) const;

		Font& GetFont() const;
		void SetFont(std::string_view path, float ptsize);

		Style& GetCurrentStyle() const;
		std::optional<std::reference_wrapper<Style>> GetStyle(const std::string& name) const;
		bool RegisterStyle(const std::string& name, std::unique_ptr<Style> style) const;
		bool UnregisterStyle(const std::string& name) const;
		void SwitchStyle(const std::string& name) const;
		void SetStyleFollowSystem() const;

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args) {
			return m_rootCmp->AddChild<T>(std::forward<Args>(args)...);
		}

		RootComponent& GetRootComponent() const { return *m_rootCmp; }

		SDL_Window& GetSDLWindow() const { return *m_window; }
		SDL_Renderer& GetSDLRenderer() const { return m_renderer->GetSDLRenderer(); }
		TTF_TextEngine& GetTTFTextEngine() const { return m_renderer->GetTTFTextEngine(); }
		Renderer& GetRenderer() const { return *m_renderer; }

	private:
		friend class GuiManager;
		friend class EventManager;

		SDL_Window* m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<StyleManager> m_styleManager;
		std::unique_ptr<Font> m_font;
		std::unique_ptr<RootComponent> m_rootCmp;

	private:
		void HandleEvent(Event* event) const;
		void UpdateAndRender() const;
	};
}