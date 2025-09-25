#include "gui_manager.hpp"
#include "SDL3/SDL_dialog.h"


namespace SimpleGui {

	std::unique_ptr<GuiManager> GuiManager::s_guiManager;


	void CommandArgs::Setup(int argc, char** argv) {
		for (int i = 0; i < argc; ++i) {
			m_args.emplace_back(argv[i]);
		}
	}


	GuiManager::~GuiManager() {
		m_fpsController.reset();
		SDL_Log("delete fps controller");
		m_eventManager.reset();
		SDL_Log("delete event manager");
		m_window.reset();
		SDL_Log("delete window");
		m_timerManager.reset();
		SDL_Log("delete tiemr manager");
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
		s_guiManager->m_window = std::make_unique<Window>("simple gui", 640, 480);
		s_guiManager->m_window->SetFont(s_guiManager->m_defaultResource.font->GetPath(), s_guiManager->m_defaultResource.font->GetSize());
		s_guiManager->m_window->SwitchStyle(StyleManager::DarkStyle);
		s_guiManager->m_eventManager = std::make_unique<EventManager>(s_guiManager->m_window.get());
		s_guiManager->m_fpsController = std::make_unique<FrameRateController>(s_guiManager->m_window.get());
		s_guiManager->m_timerManager = std::make_unique<TimerManager>();

		SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "composition");
	}

	void GuiManager::Quit() {
		s_guiManager.reset();

		TTF_Quit();
		SDL_Quit();
	}

	GuiManager& GuiManager::GetInstance() {
		return *s_guiManager;
	}

	Window& GuiManager::GetWindow(std::string_view title, int w, int h) {
		m_window->SetTitle(title);
		m_window->SetSize(w, h);
		return *m_window;
	}

	Vec2 GuiManager::GetMousePosition() const {
		return m_window->GetRenderer().GetRenderPositionFromMouse();
	}

	void GuiManager::Run() {
		Event* event = nullptr;
		bool running = true;

		while (running) {
			// control framerate
			m_fpsController->Update();

			// update tiemrs
			m_timerManager->Update();

			// handle event
			while (event = m_eventManager->PollEvent()) {
				if (event->IsApplicationQuitEvent()) {
					running = false;
				}

				else if (event->GetWindowID() == m_window->GetID()) {
					m_window->HandleEvent(event);
				}

				m_eventManager->FreeEvent(event);
			}

			// update and render
			m_window->UpdateAndRender();
		}
	}
}

