#include "component/button.hpp"
#include "gui_manager.hpp"



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

	void Button::SetFont(std::unique_ptr<Font> font) {
		m_lbl->SetFont(std::move(font));
	}

	void Button::SetFont(std::string_view path, int size) {
		m_lbl->SetFont(path, size);
	}

	bool Button::HandleEvent(const SDL_Event& event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		//Vec2 renderPos = SG_GuiManager.GetRenderer().GetRenderPositionFromMouse();
		Vec2 renderPos;

		if (!m_visibleGRect.ContainPoint(renderPos)) {
			m_mouseState = MouseState::Normal;
		}
		else {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				m_mouseState = MouseState::Pressed;
				return true;
			}
			if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
				m_mouseState = MouseState::Hovering;
				clicked();
				return true;
			}

			if (m_mouseState != MouseState::Pressed) {
				m_mouseState = MouseState::Hovering;
				return true;
			}
		}
		

		return BaseComponent::HandleEvent(event);
	}

	void Button::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		BaseComponent::Update();
		m_lbl->SetGlobalPosition(GetGlobalPosition());
		m_lbl->SetSize(m_size);
		m_lbl->Update();
		CalcVisibleGlobalRect(this, m_lbl.get());
	}

	void Button::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::Background));

		if (m_mouseState == MouseState::Normal) {
			renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::ButtonNormal));
		}
		else if (m_mouseState == MouseState::Hovering) {
			renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::ButtonHovered));
		}
		else {
			renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::ButtonPressed));
		}

		m_lbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::ButtonForeground));
		m_lbl->Render(renderer);

		renderer.SetClipRect(m_visibleGRect);
		renderer.DrawRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::ButtonBorder));
		renderer.ClearClipRect();

		BaseComponent::Render(renderer);
	}
}