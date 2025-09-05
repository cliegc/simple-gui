#include "gui_manager.hpp"
#include "SDL3/SDL_dialog.h"


namespace SimpleGui {

	std::unique_ptr<GuiManager> GuiManager::s_guiManager;


	void CommandArgs::Setup(int argc, char** argv) {
		for (int i = 0; i < argc; ++i) {
			m_args.emplace_back(argv[i]);
		}
	}


	void GuiManager::Init(int argc, char** argv, std::string_view fontPath) {
		if (s_guiManager) return;

		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "error", SDL_GetError(), nullptr);
			exit(-1);
		}

		if (!TTF_Init()) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "error", SDL_GetError(), nullptr);
			SDL_Quit();
			exit(-1);
		}

		s_guiManager = std::unique_ptr<GuiManager>(new GuiManager());
		s_guiManager->m_cmdArgs.Setup(argc, argv);
		s_guiManager->m_defaultResource.font = std::make_unique<Font>(fontPath, 16.f);
		s_guiManager->m_defaultResource.style = StyleManager::CreateDarkStyle();
	}

	void GuiManager::Quit() {
		s_guiManager.reset();

		TTF_Quit();
		SDL_Quit();
	}

	GuiManager& GuiManager::GetInstance() {
		return *s_guiManager;
	}

	Window& GuiManager::CreateWindow(std::string_view title, int w, int h) {
		auto win = std::make_unique<Window>(title, w, h);
		auto id = win->GetID();
		win->SetFont(m_defaultResource.font->GetPath(), m_defaultResource.font->GetSize());
		m_windows.emplace(id, std::move(win));
		return *m_windows[id];
	}

	void GuiManager::Run() {
		bool running = true;

		while (running) {
			// control framerate

			// handle event
			HandleEvent();
			
			// update and render
			for (auto& result : m_windows) {
				result.second->Update();
				result.second->Render();
			}
		}
	}

	void GuiManager::HandleEvent() {
	}

	void GuiManager::Update() {
	}

	void GuiManager::Render() {
	}
}

