#include "gui_manager.hpp"


namespace SimpleGui {

	std::unique_ptr<GuiManager> GuiManager::s_guiManager;


	GuiManager::GuiManager(SDL_Window* window, SDL_Renderer* renderer, std::string_view fontPath) {
		m_window = window;
		m_renderer = std::make_unique<Renderer>(renderer);

		auto engine = TTF_CreateRendererTextEngine(renderer);
		if (!engine) {
			SDL_Log("%s\n", SDL_GetError());
			exit(-1);
		}
		m_textEngine = std::unique_ptr<TTF_TextEngine, TextEgineDeleter>(engine);

		auto font = TTF_OpenFont(fontPath.data(), 14);
		if (!font) {
			SDL_Log("%s\n", SDL_GetError());
			exit(-1);
		}
		m_defaultFont = std::unique_ptr<TTF_Font, FontDeleter>(font);

		m_rootCmp = std::make_unique<BaseComponent>();
		SetRootComponentSizeToFillWindow();
	}

	GuiManager::~GuiManager() {
		m_rootCmp.reset();
		m_defaultFont.reset();
		m_textEngine.reset();
	}

	void GuiManager::Init(SDL_Window* window, SDL_Renderer* renderer, std::string_view fontPath) {
		if (!s_guiManager) {
			s_guiManager = std::unique_ptr<GuiManager>(new GuiManager(window, renderer, fontPath));
		}
	}

	void GuiManager::Quit() {
		s_guiManager.reset();
	}

	GuiManager& GuiManager::GetInstance() {
		return *s_guiManager;
	}

	TTF_TextEngine& GuiManager::GetTextEngine() const {
		return *m_textEngine;
	}

	TTF_Font& GuiManager::GetDefaultFont() const {
		return *m_defaultFont;
	}

	void GuiManager::SetDefaultFont(std::string_view path, float size) {
		auto font = TTF_OpenFont(path.data(), size);
		m_defaultFont.reset(font);
	}

	void GuiManager::SetDefaultFontSize(float size) {
		TTF_SetFontSize(m_defaultFont.get(), size);
	}

	void GuiManager::HandleEvent(const SDL_Event& event) {
		if (event.type == SDL_EVENT_WINDOW_RESIZED) {
			SetRootComponentSizeToFillWindow();
		}

		m_rootCmp->HandleEvent(event);
	}

	void GuiManager::Update() {
		m_rootCmp->Update();
	}

	void GuiManager::Render() {
		m_rootCmp->Render(*m_renderer);
	}

	void GuiManager::FrameStart() {
	}

	void GuiManager::FrameEnd() {
		//m_rootCmp->HandleNeedRemoveRequest();
		//m_rootCmp->HandleNeedAddRequest();
	}

	void GuiManager::SetRootComponentSizeToFillWindow() {
		int w, h;
		SDL_GetWindowSizeInPixels(m_window, &w, &h);
		m_rootCmp->SetSize(w, h);
	}
}

