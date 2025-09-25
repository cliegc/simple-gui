#include "component/line_edit.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	LineEdit::LineEdit(std::string_view placeholder) {
		m_textLbl = std::make_unique<Label>(placeholder);
		m_selectedTextLbl = std::make_unique<Label>("");

		m_placeholder = placeholder;
		m_aligment = Alignment::Begin;
		m_active = false;
		m_editable = true;
		m_selectingEnabled = true;
		m_secretEnable = false;
		m_secretChar = '*';
		SetAlignment(m_aligment);

		m_textLbl->SetPadding(0, 0, 0, 0);
		m_selectedTextLbl->SetPadding(0, 0, 0, 0);
		m_selectedTextLbl->SetTextAlignments(TextAlignment::Center, TextAlignment::Center);
	}

	void LineEdit::EnteredComponentTree() {
		BaseComponent::EnteredComponentTree();
		SetComponentOwner(m_textLbl.get(), m_window, this);
		SetComponentOwner(m_selectedTextLbl.get(), m_window, this);
		BaseComponent::EnteredComponentTree(m_textLbl.get());
		BaseComponent::EnteredComponentTree(m_selectedTextLbl.get());
		
		m_padding = m_window->GetCurrentStyle().componentPadding;
		m_textLbl->SetPadding(0, 0, 0, 0);
		m_selectedTextLbl->SetPadding(0, 0, 0, 0);
		SetMinSize(GetFont().GetSize() + m_padding.left + m_padding.right,
			m_textLbl->GetSize().h + m_padding.top + m_padding.bottom);
		SetWidth(150);
	}

	bool LineEdit::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		if (BaseComponent::HandleEvent(event)) return true;

		Vec2 mousePos = SG_GuiManager.GetMousePosition();
		if (auto ev = event->Convert<MouseButtonEvent>();
			ev && ev->IsPressed(MouseButton::Left)) {
			m_active = m_visibleGRect.ContainPoint(mousePos);

			if (m_active) SDL_StartTextInput(&m_window->GetSDLWindow());
			else SDL_StopTextInput(&m_window->GetSDLWindow());
		}

		if (!m_active) return false;

		if (auto ev = event->Convert<KeyBoardTextInputEvent>()) {
			m_string += ev->GetInputText();
			m_textLbl->SetText(m_string);
			SDL_Log("input text: %s\n", m_string.c_str());
			return true;
		}
		else if (auto ev = event->Convert<KeyBoardTextEditingEvent>()) {
			SDL_Log("editing text: %s\n", ev->GetEditingText().c_str());
			m_textLbl->SetText(m_string + ev->GetEditingText());
			return true;
		}
		else if (auto ev = event->Convert<KeyBoardButtonEvent>();
			ev && ev->IsPressed()) {
			SDL_Log("KeyBoardButtonEvent and pressed");
			if (ev->GetKeyCode() == SDLK_BACKSPACE) {
				SDL_Log("KeyBoardButtonEvent and backspace pressed");
				if (!m_string.empty()) {
					m_string.pop_back();
					m_textLbl->SetText(m_string);
				}
			}
		}

		return false;
	}

	void LineEdit::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		BaseComponent::Update();

		auto contentGRect = GetContentGlobalRect();

		// update lbl
		m_textLbl->Update();
		// update textLbl pos

		// update caret pos
		float offset = IsShowPlaceholder() ? 0.f : GetFont().GetTextSize(m_textLbl->GetText()).w;
		m_caret.GetGlobalRect().position.x = offset + contentGRect.Left();
		m_caret.GetGlobalRect().position.y = contentGRect.Top();
		m_caret.GetGlobalRect().size.h = contentGRect.size.h;
		// clamp caret pos
		m_caret.GetGlobalRect().position.x =
			SDL_clamp(m_caret.GetGlobalRect().position.x,
				contentGRect.Left(), contentGRect.Right());
		m_caret.Update();
	}

	void LineEdit::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;
		
		// draw bg
		renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::LineEditBackground));

		// draw text and placeholder text
		if (IsShowPlaceholder()) {
			m_textLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::LineEditPlaceholder));
		}
		else {
			m_textLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::LineEditForeground));
		}
		m_textLbl->Render(renderer);

		// draw selected text

		// draw caret
		m_caret.SetColor(GetThemeColor(ThemeColorFlags::LineEditCaret));
		m_caret.Render(renderer);

		// draw borer
		renderer.SetClipRect(m_visibleGRect);
		Color borderColor = m_active ? GetThemeColor(ThemeColorFlags::LineEditActivatedBorder) : GetThemeColor(ThemeColorFlags::LineEditBorder);
		renderer.DrawRect(GetGlobalRect(), borderColor);
		renderer.ClearClipRect();

		BaseComponent::Render(renderer);
	}

	void LineEdit::SetAlignment(Alignment alignment) {
		TextAlignment aln =
			alignment == Alignment::Begin ? TextAlignment::Left :
			(alignment == Alignment::Center ? TextAlignment::Center : TextAlignment::Right);
		m_textLbl->SetTextAlignments(aln, TextAlignment::Center);
		m_aligment = alignment;
	}
}