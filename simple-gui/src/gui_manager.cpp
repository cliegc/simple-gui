#include "gui_manager.hpp"
#include <SDL3/SDL_dialog.h>
#include "logger.hpp"


namespace SimpleGui {

	std::unique_ptr<GuiManager> GuiManager::s_guiManager;


	void CommandArgs::Setup(int argc, char** argv) {
		for (int i = 0; i < argc; ++i) {
			m_args.emplace_back(argv[i]);
		}
	}

	constexpr float DEFAULT_FONT_SIZE = 12.f;

	GuiManager::~GuiManager() {
		m_fpsController.reset();
		m_eventManager.reset();
		m_window.reset();
		m_timerManager.reset();
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
		s_guiManager->m_defaultResource.font = std::make_unique<Font>(fontPath, DEFAULT_FONT_SIZE);
		s_guiManager->m_defaultResource.style = StyleManager::CreateDarkStyle();
		s_guiManager->m_window = std::make_unique<Window>("simple gui", 640, 480);
		s_guiManager->m_window->SetFont(s_guiManager->m_defaultResource.font->GetPath(), s_guiManager->m_defaultResource.font->GetSize());
		s_guiManager->m_window->SwitchStyle(StyleManager::DarkStyle);
		s_guiManager->m_eventManager = std::make_unique<EventManager>(s_guiManager->m_window.get());
		s_guiManager->m_fpsController = std::make_unique<FrameRateController>(s_guiManager->m_window.get());
		s_guiManager->m_timerManager = std::make_unique<TimerManager>();

		//SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "composition");
		SG_INFO("SimpleGui: gui manager initialization successful.");
	}

	void GuiManager::Quit() {
		SG_INFO("SimpleGui: exiting gui manager.");
		s_guiManager.reset();
		TTF_Quit();
		SDL_Quit();
	}

	GuiManager& GuiManager::GetInstance() {
		return *s_guiManager;
	}

	Window& GuiManager::GetWindow(std::string_view title, int w, int h) const {
		m_window->SetTitle(title);
		m_window->SetSize(w, h);
		return *m_window;
	}

	Vec2 GuiManager::GetMousePosition() const {
		return m_window->GetRenderer().GetRenderPositionFromMouse();
	}

	void GuiManager::Run() const {
		Event* event = nullptr;
		bool running = true;

		while (running) {
			// control framerate
			m_fpsController->Update();

			// update timers
			m_timerManager->Update();

			// handle event
			while ((event = m_eventManager->PollEvent())) {
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

