#include "component/button.hpp"
#include "gui_manager.hpp"



namespace SimpleGui {
	Button::Button(std::string_view text) : BaseComponent() {
		m_lbl = std::make_unique<Label>(text);
	}

	void Button::EnteredComponentTree() {
		SetComponentOwner(m_lbl.get(), m_window, this);
		BaseComponent::EnteredComponentTree(m_lbl.get());
		SetSize(m_lbl->GetSize());
		SetMinSize(m_lbl->GetMinSize());
		m_lbl->SetTextAlignments(TextAlignment::Center, TextAlignment::Center);
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

	bool Button::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		if (BaseComponent::HandleEvent(event)) return true;
		if (!m_visibleGRect.ContainPoint(SG_GuiManager.GetMousePosition())) {
			m_mouseState = MouseState::Normal;
		}
		else {
			if (auto ev = event->Convert<MouseButtonEvent>()) {
				if (ev->IsPressed(MouseButton::Left)) {
					m_mouseState = MouseState::Pressed;
					return true;
				}
				else if (m_mouseState == MouseState::Pressed && ev->IsReleased(MouseButton::Left)) {
					m_mouseState = MouseState::Hovering;
					clicked();
					return true;
				}
			}

			if (m_mouseState != MouseState::Pressed) {
				m_mouseState = MouseState::Hovering;
				return false;
			}
		}

		return false;
	}

	void Button::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		BaseComponent::Update();
		m_lbl->SetGlobalPosition(GetGlobalPosition());
		m_lbl->SetSize(m_size);
		m_lbl->Update();
		CalcVisibleGlobalRect(this, m_lbl.get());
	}

	void Button::Render(Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		renderer.RenderRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::Background), true);

		Color color;
		if (m_mouseState == MouseState::Normal) {
			color = GetThemeColor(ThemeColorFlags::ButtonNormal);
		}
		else if (m_mouseState == MouseState::Hovering) {
			color = GetThemeColor(ThemeColorFlags::ButtonHovered);
		}
		else {
			color = GetThemeColor(ThemeColorFlags::ButtonPressed);
		}
		renderer.RenderRect(m_visibleGRect, color, true);

		m_lbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::ButtonForeground));
		m_lbl->Render(renderer);

		renderer.SetRenderClipRect(m_visibleGRect);
		renderer.RenderRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::ButtonBorder), false);
		renderer.ClearRenderClipRect();

		BaseComponent::Render(renderer);
	}
}