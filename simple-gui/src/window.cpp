#include "window.hpp"
#include <SDL3/SDL_dialog.h>
#include <SDL3_image/SDL_image.h>

#include <memory>
#include "event.hpp"


namespace SimpleGui {
	Window::Window(std::string_view title, int w, int h) {
		m_window = SDL_CreateWindow(title.data(), w, h, SDL_WINDOW_RESIZABLE);
		if (!m_window) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "error", SDL_GetError(), nullptr);
			SDL_Log("%s\n", SDL_GetError());
			TTF_Quit();
			SDL_Quit();
			exit(-1);
		}

		m_renderer = std::make_unique<Renderer>(m_window);
		m_styleManager = std::make_unique<StyleManager>();
		m_rootCmp = std::unique_ptr<RootComponent>(new RootComponent(this));
	}

	Window::~Window() {
		m_rootCmp.reset();
		m_styleManager.reset();
		m_font.reset();
		m_renderer.reset();
		SDL_DestroyWindow(m_window);
	}

	std::string Window::GetTitle() const {
		return SDL_GetWindowTitle(m_window);
	}

	void Window::SetTitle(std::string_view title) const {
		SDL_SetWindowTitle(m_window, title.data());
	}

	void Window::SetIcon(std::string_view path) const {
		SDL_Surface* icon = IMG_Load(path.data());
		SDL_SetWindowIcon(m_window, icon);
	}

	Vec2 Window::GetPosition() const {
		int x, y;
		SDL_GetWindowPosition(m_window, &x, &y);
		return Vec2(x, y);
	}

	void Window::SetPosition(int x, int y) const {
		SDL_SetWindowPosition(m_window, x, y);
	}

	Vec2 Window::GetSize() const {
		int w, h;
		SDL_GetWindowSize(m_window, &w, &h);
		return Vec2(w, h);
	}

	void Window::SetSize(int w, int h) const {
		SDL_SetWindowSize(m_window, w, h);
	}

	bool Window::IsResizable() const {
		return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_RESIZABLE) != 0;
	}

	void Window::SetResizable(bool resizable) const {
		SDL_SetWindowResizable(m_window, resizable);
	}

	bool Window::IsAllwaysOnTop() const {
		return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_ALWAYS_ON_TOP) != 0;
	}

	void Window::SetAllwaysOnTop(bool top) const {
		SDL_SetWindowAlwaysOnTop(m_window, top);
	}

	bool Window::IsFullScreen() const {
		return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) != 0;
	}

	void Window::SetFullScreen(bool full) const {
		SDL_SetWindowFullscreen(m_window, full);
	}

	bool Window::IsMaximized() const {
		return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MAXIMIZED) != 0;
	}

	void Window::ToggleMaximize() const {
		SDL_MaximizeWindow(m_window);
	}

	bool Window::IsMinimized() const {
		return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MINIMIZED) != 0;
	}

	void Window::ToggleMinimize() const {
		SDL_MinimizeWindow(m_window);
	}

	WindowState Window::GetState() const {
		if ((SDL_GetWindowFlags(m_window) & SDL_WINDOW_MINIMIZED) != 0) return WindowState::Minimized;
		else if ((SDL_GetWindowFlags(m_window) & SDL_WINDOW_MAXIMIZED) != 0) return WindowState::Maximized;
		else if ((SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) != 0) return WindowState::FullScreen;
		else return WindowState::Normal;
	}

	void Window::SetState(WindowState state) {
		switch (state) {
		case SimpleGui::WindowState::Normal:
			SDL_RestoreWindow(m_window);
			break;
		case SimpleGui::WindowState::Minimized:
			ToggleMinimize();
			break;
		case SimpleGui::WindowState::Maximized:
			ToggleMaximize();
			break;
		case SimpleGui::WindowState::FullScreen:
			SetFullScreen(true);
			break;
		}
	}

	WindowID Window::GetID() const {
		return SDL_GetWindowID(m_window);
	}

	bool Window::IsEnabledVsync() const {
		int vsync = SDL_RENDERER_VSYNC_DISABLED;
		SDL_GetRenderVSync(&m_renderer->GetSDLRenderer(), &vsync);
		return vsync != SDL_RENDERER_VSYNC_DISABLED;
	}

	bool Window::EnableVsync(bool enable) const {
		auto vsync = enable ? 1 : SDL_RENDERER_VSYNC_DISABLED;
		return SDL_SetRenderVSync(&m_renderer->GetSDLRenderer(), vsync);;
	}

	Font& Window::GetFont() const {
		return *m_font;
	}

	void Window::SetFont(std::string_view path, float ptsize) {
		m_font = std::make_unique<Font>(path, ptsize);
	}

	std::string Window::GetCurrentStyleName() const {
		return m_styleManager->GetCurrentStyleName();
	}

	std::vector<std::string> Window::GetStyleNames() const {
		return m_styleManager->GetStyleNames();
	}

	Style* Window::GetCurrentStyle() const {
		return m_styleManager->GetCurrentStyle();
	}

	Style* Window::GetStyle(const std::string& name) const {
		return m_styleManager->GetStyle(name);
	}

	Style * Window::CreateBlankStyle(const std::string &name) const {
		return m_styleManager->CreateBlankStyle(name);
	}

	Style * Window::CopyStyle(const std::string &name, const std::string &target) {
		return m_styleManager->CopyStyle(name, target);
	}

	bool Window::RemoveStyle(const std::string &name) const {
		return m_styleManager->RemoveStyle(name);
	}

	bool Window::SwitchStyle(const std::string& name) const {
		return m_styleManager->SwitchStyle(name);
	}

	void Window::SetStyleFollowSystem() const {
		m_styleManager->SetStyleFollowSystem();
	}

	void Window::HandleEvent(Event* event) const {
		m_rootCmp->HandleEvent(event);
	}

	void Window::UpdateAndRender() const {
		m_rootCmp->Update();
		//m_renderer->Clear();
		m_rootCmp->Render(*m_renderer);
		m_renderer->Render();
		//m_renderer->Present();
	}
}