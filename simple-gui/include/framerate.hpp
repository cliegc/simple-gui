#pragma once
#include <SDL3/SDL_timer.h>


namespace SimpleGui {
	class FramerateController final {
	public:
		FramerateController();
		~FramerateController() = default;

	private:
		friend class GuiManager;

		Uint32 m_lastTicks;

		float Update();
	};
}
