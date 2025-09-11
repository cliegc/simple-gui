#include "component/root_component.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	RootComponent::RootComponent(Window* window) : BaseComponent() {
		m_window = window;
		m_padding = m_window->GetCurrentStyle().componentPadding;
		SetSizeToFillWindow();
	}

	void RootComponent::SetSizeToFillWindow() {
		SetSize(m_window->GetRenderer().GetRenderOutputSize());
	}

	bool RootComponent::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		//if (event.type == SDL_EVENT_WINDOW_RESIZED) {
		//	SetSizeToFillWindow();
		//}

		if (event->IsWindowResizedEvent()) {
			SetSizeToFillWindow();
		}

		return BaseComponent::HandleEvent(event);
	}

	void RootComponent::Update() {
		SG_CMP_UPDATE_CONDITIONS;
		BaseComponent::Update();
	}

	void RootComponent::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;
		renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::Background));
		BaseComponent::Render(renderer);
	}
}