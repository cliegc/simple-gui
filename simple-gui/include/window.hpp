#pragma once
#include <SDL3/SDL_video.h>
#include <string>
#include <string_view>
#include <memory>
#include <optional>
#include "signal.hpp"
#include "renderer.hpp"
#include "style.hpp"
#include "font.hpp"
#include "component/root_component.hpp"


namespace SimpleGui {
	class Event;

	using WindowID = SDL_WindowID;

	class Window final {
	public:
		Window(std::string_view title, int w, int h);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

		std::string GetTitle() const;
		void SetTitle(std::string_view title);

		void SetIcon(std::string_view path);

		Vec2 GetPosition() const;
		void SetPosition(int x, int y);

		Vec2 GetSize() const;
		void SetSize(int w, int h);

		bool IsResizable() const;
		void SetResizable(bool resizable);

		bool IsAllwaysOnTop() const;
		void SetAllwaysOnTop(bool top);

		bool IsFullScreen() const;
		void SetFullScreen(bool full);

		bool IsMaximized() const;
		void ToggleMaximize() const;

		bool IsMinimized() const;
		void ToggleMinimize() const;

		WindowID GetID() const;

		bool IsEnabledVsync() const;
		bool EnableVsync(bool enable);

		Font& GetFont() const;
		void SetFont(std::string_view path, float ptsize);

		Style& GetCurrentStyle() const;
		std::optional<std::reference_wrapper<Style>> GetStyle(const std::string& name);
		bool RegisterStyle(const std::string& name, std::unique_ptr<Style> style);
		bool UnregisterStyle(const std::string& name);
		void SwitchStyle(const std::string& name);
		void SetStyleFollowSystem();

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args) {
			return m_rootCmp->AddChild<T>(std::forward<Args>(args)...);
		}

		inline SDL_Window& GetSDLWindow() const { return *m_window; }
		inline SDL_Renderer& GetSDLRenderer() const { return m_renderer->GetSDLRenderer(); }
		inline TTF_TextEngine& GetTTFTextEngine() const { return m_renderer->GetTTFTextEngine(); }
		inline Renderer& GetRenderer() const { return *m_renderer; }

	public:
		Signal<> closed;
		Signal<> resized;
		Signal<> maximized;
		Signal<> minimized;

	private:
		friend class GuiManager;

		bool m_visible;
		SDL_Window* m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<StyleManager> m_styleManager;
		std::unique_ptr<Font> m_font;
		std::unique_ptr<RootComponent> m_rootCmp;

	private:
		void HandleEvent(Event* event);
		void Update();
		void Render();
	};
}