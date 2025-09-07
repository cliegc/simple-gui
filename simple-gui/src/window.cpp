#include "window.hpp"
#include "SDL3/SDL_dialog.h"
#include "SDL3_image/SDL_image.h"


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

	void Window::SetTitle(std::string_view title) {
		SDL_SetWindowTitle(m_window, title.data());
	}

	void Window::SetIcon(std::string_view path) {
		SDL_Surface* icon = IMG_Load(path.data());
		SDL_SetWindowIcon(m_window, icon);
	}

	Vec2 Window::GetPosition() const {
		int x, y;
		SDL_GetWindowPosition(m_window, &x, &y);
		return Vec2(x, y);
	}

	void Window::SetPosition(int x, int y) {
		SDL_SetWindowPosition(m_window, x, y);
	}

	Vec2 Window::GetSize() const {
		int w, h;
		SDL_GetWindowSize(m_window, &w, &h);
		return Vec2(w, h);
	}

	void Window::SetSize(int w, int h) {
		SDL_SetWindowSize(m_window, w, h);
	}

	bool Window::IsResizable() const {
		return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_RESIZABLE) != 0;
	}

	void Window::SetResizable(bool resizable) {
		SDL_SetWindowResizable(m_window, resizable);
	}

	bool Window::IsAllwaysOnTop() const {
		return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_ALWAYS_ON_TOP) != 0;
	}

	void Window::SetAllwaysOnTop(bool top) {
		SDL_SetWindowAlwaysOnTop(m_window, top);
	}

	bool Window::IsFullScreen() const {
		return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) != 0;
	}

	void Window::SetFullScreen(bool full) {
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

	WindowID Window::GetID() const {
		return SDL_GetWindowID(m_window);
	}

	bool Window::IsEnabledVsync() const {
		int vsync = SDL_RENDERER_VSYNC_DISABLED;
		SDL_GetRenderVSync(&m_renderer->GetSDLRenderer(), &vsync);
		return vsync != SDL_RENDERER_VSYNC_DISABLED;
	}

	bool Window::EnableVsync(bool enable) {
		auto vsync = enable ? 1 : SDL_RENDERER_VSYNC_DISABLED;
		return SDL_SetRenderVSync(&m_renderer->GetSDLRenderer(), vsync);;
	}

	Font& Window::GetFont() const {
		return *m_font;
	}

	void Window::SetFont(std::string_view path, float ptsize) {
		m_font.reset(new Font(path, ptsize));
	}

	Style& Window::GetCurrentStyle() const {
		return m_styleManager->GetCurrentStyle();
	}

	std::optional<std::reference_wrapper<Style>> Window::GetStyle(const std::string& name) {
		return m_styleManager->GetStyle(name);
	}

	bool Window::RegisterStyle(const std::string& name, std::unique_ptr<Style> style) {
		return m_styleManager->RegisterStyle(name, std::move(style));
	}

	bool Window::UnregisterStyle(const std::string& name) {
		return m_styleManager->UnregisterStyle(name);
	}

	void Window::SwitchStyle(const std::string& name) {
		m_styleManager->SwitchStyle(name);
	}

	void Window::SetStyleFollowSystem() {
		m_styleManager->SetStyleFollowSystem();
	}

	void Window::HandleEvent(Event* event) {
	}

	void Window::UpdateAndRender() {
		m_rootCmp->Update();
		m_renderer->Clear();
		m_rootCmp->Render(*m_renderer);
		m_renderer->Present();
	}
}