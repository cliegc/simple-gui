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
		m_textEngine = UniqueTextEnginePtr(engine);

		m_defaultFont = std::make_unique<Font>(fontPath, 14);
		if (!m_defaultFont || m_defaultFont->IsNull()) {
			SDL_Log("%s\n", SDL_GetError());
			exit(-1);
		}

		m_styleManager = std::make_unique<StyleManager>();
	}

	GuiManager::~GuiManager() {
		m_rootCmp.reset();
		m_defaultFont.reset();
		m_textEngine.reset();
	}

	void GuiManager::Init(SDL_Window* window, SDL_Renderer* renderer, std::string_view fontPath) {
		if (!s_guiManager) {
			s_guiManager = std::unique_ptr<GuiManager>(new GuiManager(window, renderer, fontPath));
			s_guiManager->m_rootCmp = std::unique_ptr<RootComponent>(new RootComponent());
		}
	}

	void GuiManager::Quit() {
		s_guiManager.reset();
	}

	GuiManager& GuiManager::GetInstance() {
		return *s_guiManager;
	}

	SDL_Window& GuiManager::GetWindow() const {
		return *m_window;
	}

	Renderer& GuiManager::GetRenderer() const {
		return *m_renderer;
	}

	TTF_TextEngine& GuiManager::GetTextEngine() const {
		return *m_textEngine;
	}

	Font& GuiManager::GetDefaultFont() const {
		return *m_defaultFont;
	}

	RootComponent& GuiManager::GetRootComponent() const {
		return *m_rootCmp;
	}

	void GuiManager::SetDefaultFont(std::string_view path, float size) {
		m_defaultFont.reset(new Font(path, size));
	}

	Style& GuiManager::GetCurrentStyle() {
		return m_styleManager->GetCurrentStyle();
	}

	std::optional<std::reference_wrapper<Style>> GuiManager::GetStyle(const std::string& name) {
		return m_styleManager->GetStyle(name);
	}

	void GuiManager::SwitchStyle(const std::string& name) {
		m_styleManager->SwitchStyle(name);
	}

	bool GuiManager::RegisterStyle(const std::string& name, std::unique_ptr<Style> style) {
		return m_styleManager->RegisterStyle(name, std::move(style));
	}

	bool GuiManager::UnregisterStyle(const std::string& name) {
		return m_styleManager->UnregisterStyle(name);
	}

	void GuiManager::SetStyleFollowSystem() {
		m_styleManager->SetStyleFollowSystem();
	}

	void GuiManager::HandleEvent(const SDL_Event& event) {
		m_rootCmp->HandleEvent(event);
	}

	void GuiManager::Update() {
		m_rootCmp->Update();
	}

	void GuiManager::Render() {
		m_rootCmp->Render(*m_renderer);
	}

	void GuiManager::SetRootComponentSizeToFillWindow() {
		int w, h;
		SDL_GetWindowSizeInPixels(m_window, &w, &h);
		m_rootCmp->SetSize(w, h);
	}
}

