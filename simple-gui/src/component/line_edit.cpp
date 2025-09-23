#include "component/line_edit.hpp"


namespace SimpleGui {
	LineEdit::LineEdit(LineEditContentType type, std::string_view placeholder) {
		m_placeholder = placeholder;
		m_contentType = type;
	}

	bool LineEdit::HandleEvent(Event* event) {
		SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE;

		if (BaseComponent::HandleEvent(event)) return true;



		return false;
	}

	void LineEdit::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		BaseComponent::Update();

		// update caret pos
	}

	void LineEdit::Render(const Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;
		
		// draw bg
		renderer.FillRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::LineEditBackground));

		// draw placeholder text
		if (m_content.empty()) {

		}

		// draw text

		// draw caret
		m_caret.SetColor(GetThemeColor(ThemeColorFlags::LineEditCaret));
		//m_caret.Render(renderer, );

		// draw borer
		renderer.SetClipRect(m_visibleGRect);
		renderer.DrawRect(GetGlobalRect(), GetThemeColor(ThemeColorFlags::LineEditBorder));
		renderer.ClearClipRect();

		BaseComponent::Render(renderer);
	}
}