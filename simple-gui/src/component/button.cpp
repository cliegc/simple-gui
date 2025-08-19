#include "component/button.hpp"



namespace SimpleGui {
	Button::Button(std::string_view text) : BaseComponent() {
		m_lbl = std::make_unique<Label>(text);
		SetSize(m_lbl->GetSize());
		SetMinSize(m_lbl->GetMinSize());

		m_lbl->SetTextAlignments(TextAlignment::Center, TextAlignment::Center);
		m_lbl->CustomThemeColor(ThemeColorFlags::LabelBackgound, Color::TRANSPARENT);
		m_lbl->CustomThemeColor(ThemeColorFlags::LabelBorder, Color::TRANSPARENT);
	}

	std::string Button::GetText() const {
		return m_lbl->GetText();
	}

	void Button::SetText(std::string_view text) {
		m_lbl->SetText(text); 
		SetSize(m_lbl->GetSize());
	}

	bool Button::HandleEvent(const SDL_Event& event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		if (!m_visibleRect.ContainPoint(Vec2(event.motion.x, event.motion.y))) {
			m_mouseState = MouseState::Normal;
			return false;
		}

		if (m_mouseState != MouseState::Pressed) m_mouseState = MouseState::Hovering;

		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
			m_mouseState = MouseState::Pressed;
		}
		else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
			m_mouseState = MouseState::Hovering;
			clicked();
		}

		return BaseComponent::HandleEvent(event);
	}

	void Button::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		m_lbl->Update();

		BaseComponent::Update();
	}

	void Button::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.FillRect(m_visibleRect, GetThemeColor(ThemeColorFlags::Background));

		if (m_mouseState == MouseState::Normal) {
			renderer.FillRect(m_visibleRect, GetThemeColor(ThemeColorFlags::ButtonNormal));
		}
		else if (m_mouseState == MouseState::Hovering) {
			renderer.FillRect(m_visibleRect, GetThemeColor(ThemeColorFlags::ButtonHovered));
		}
		else {
			renderer.FillRect(m_visibleRect, GetThemeColor(ThemeColorFlags::ButtonPressed));
		}

		m_lbl->SetGlobalPosition(GetGlobalPosition());
		m_lbl->SetSize(m_size);

		m_lbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::ButtonForeground));
		m_lbl->Render(renderer);

		renderer.DrawRect(m_visibleRect, GetThemeColor(ThemeColorFlags::Border));

		BaseComponent::Render(renderer);
	}
}