#include <utf8.h>
#include <SDL3/SDL_clipboard.h>
#include "component/line_edit.hpp"
#include "gui_manager.hpp"


namespace SimpleGui {
	LineEdit::LineEdit(std::string_view placeholder) {
		m_textLbl = std::make_unique<Label>(placeholder);
		m_selectedTextLbl = std::make_unique<Label>("");

		m_placeholder = placeholder;
		m_caretIndex = 0;
		m_maxLength = MAX_LENGTH;
		m_active = false;
		m_editable = true;
		m_selectingEnabled = true;
		m_secretEnable = true;
		m_secretChar = '*';
		m_cursor = UniqueCursorPtr(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT));

		m_textLbl->SetPadding(0, 0, 0, 0);
		m_textLbl->SetSizeFollowTextEnabled(true);
		m_selectedTextLbl->SetPadding(0, 0, 0, 0);
		m_selectedTextLbl->SetSizeFollowTextEnabled(true);
		m_selectedTextLbl->SetTextAlignments(TextAlignment::Center, TextAlignment::Center);

		m_caret.SetBlink(true);
		m_caret.SetVisible(false);

		// debug
		//m_textLbl->CustomThemeColor(ThemeColorFlags::LabelBorder, Color::GRAY);
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

		Vec2 mousePos = SG_GuiManager.GetMousePosition();
		if (auto ev = event->Convert<MouseButtonEvent>();
			ev && ev->IsPressed(MouseButton::Left)) {
			m_active = m_visibleGRect.ContainPoint(mousePos);

			if (m_active) {
				SDL_StartTextInput(&m_window->GetSDLWindow());
				m_caret.SetVisible(true);

				// set IME composition window positon
				Vec2 pos = GetGlobalRect().BottomLeft();
				IMEUtils::SetIMECompositionWindowPosition(*m_window, pos);
			}
			else {
				SDL_StopTextInput(&m_window->GetSDLWindow());
				m_caret.SetVisible(false);
			}
		}

		if (HandleMouseCursor(event)) return true;
		// TODO these code -> class TextEdit
		if (HandleInputText(event)) return true;
		if (HandleShortKey(event)) return true;
		if (HandleSelectText(event)) return true;

		if (BaseComponent::HandleEvent(event)) return true;

		return false;
	}

	void LineEdit::Update() {
		SG_CMP_UPDATE_CONDITIONS;

		BaseComponent::Update();

		auto contentGRect = GetContentGlobalRect();

		// update text lbl
		float y = (contentGRect.size.h - m_textLbl->GetSize().h) / 2;
		m_textLbl->Update();
		m_textLbl->SetPositionY(y);

		// update caret pos
		float offset = m_caretIndex ? GetFont().GetTextSize(m_textLbl->GetText(), m_caretIndex).w : 0;
		m_caret.GetGlobalRect().position.x = offset + contentGRect.Left() + m_textLbl->GetPosition().x;
		m_caret.GetGlobalRect().size.h = GetFont().GetHeight();
		m_caret.GetGlobalRect().position.y = (contentGRect.size.h - m_caret.GetGlobalRect().size.h) / 2 + contentGRect.Top();
		// clamp caret pos
		m_caret.GetGlobalRect().position.x =
			SDL_clamp(m_caret.GetGlobalRect().position.x,
				contentGRect.Left(), contentGRect.Right());
		m_caret.Update();

		// update selected text label
		if (m_selectTextData.selecting) {
			float x = m_selectTextData.startCaretPosX;
			m_selectTextData.endCaretIndex = m_caretIndex;
			if (m_selectTextData.endCaretIndex < m_selectTextData.startCaretIndex) {
				x = m_caret.GetGlobalRect().Left() - contentGRect.Left();
			}
			m_selectedTextLbl->SetPosition(x, y);
			m_selectedTextLbl->Update();
		}
	}

	void LineEdit::Render(Renderer& renderer) {
		SG_CMP_RENDER_CONDITIONS;

		// draw bg
		renderer.RenderRect(m_visibleGRect, GetThemeColor(ThemeColorFlags::LineEditBackground), true);

		// draw text and placeholder text
		if (IsShowPlaceholder()) {
			m_textLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::LineEditPlaceholder));
		}
		else {
			m_textLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::LineEditForeground));
		}
		m_textLbl->Render(renderer);

		// draw selected text
		if (m_selectTextData.selecting) {
			m_selectedTextLbl->CustomThemeColor(ThemeColorFlags::LabelForeground, GetThemeColor(ThemeColorFlags::LineEditSelectedForeground));
			m_selectedTextLbl->CustomThemeColor(ThemeColorFlags::LabelBackgound, GetThemeColor(ThemeColorFlags::LineEditSelectedBackground));
			m_selectedTextLbl->Render(renderer);
		}

		// draw caret
		m_caret.SetColor(GetThemeColor(ThemeColorFlags::LineEditCaret));
		m_caret.Render(renderer);

		// draw border
		renderer.SetRenderClipRect(m_visibleGRect);
		Color borderColor = m_active ? GetThemeColor(ThemeColorFlags::LineEditActivatedBorder) : GetThemeColor(ThemeColorFlags::LineEditBorder);
		renderer.RenderRect(GetGlobalRect(), borderColor, false);
		renderer.ClearRenderClipRect();

		BaseComponent::Render(renderer);
	}

	inline void LineEdit::SetFont(std::unique_ptr<Font> font) {
	}

	void LineEdit::SetFont(std::string_view path, int size) {
	}

	void LineEdit::MoveCaretToLeft(int offset) {
		int index = m_caretIndex - offset;
		m_caretIndex = index > 0 ? index : 0;

		if (!m_caretIndex) return;
		float ofs = GetMoveCaretToLeftOneStepOffset();
		float w1 = GetFont().GetTextSize(m_string, m_caretIndex).w;
		float w2 = GetFont().GetTextSize(m_string, m_caretIndex - ofs).w;
		if (w1 < GetContentGlobalRect().size.w) {
			m_textLbl->SetPositionX(0);
		}
		else if (w2 + m_textLbl->GetPosition().x < 0) {
			m_textLbl->SetPositionX(m_textLbl->GetPosition().x + w1 - w2);
		}
	}

	void LineEdit::MoveCaretToRight(int offset) {
		size_t strLen = m_string.length();
		int index = m_caretIndex + offset;
		m_caretIndex = index < strLen ? index : strLen;

		if (m_caretIndex == strLen) return;
		float ofs = GetMoveCaretToRightOneStepOffset();
		float w = m_textLbl->GetSize().w;
		float w1 = GetFont().GetTextSize(m_string, m_caretIndex).w;
		float w2 = GetFont().GetTextSize(m_string, m_caretIndex + ofs).w;
		float contentW = GetContentGlobalRect().size.w;
		if (w2 > contentW && w - w2 < contentW) {
			m_textLbl->SetPositionX(contentW - w);
		}
		else if (w1 > contentW) {
			m_textLbl->SetPositionX(m_textLbl->GetPosition().x - w2 + w1);
		}
	}

	size_t LineEdit::GetMoveCaretToLeftOneStepOffset() {
		if (m_caretIndex == 0) return 0;

		size_t lastIndex = 0;
		size_t offset = 0;
		for (auto it = m_string.begin(); it != m_string.end();) {
			utf8::next(it, m_string.end());
			size_t byteIndex = it - m_string.begin();
			if (byteIndex == m_caretIndex) {
				offset = byteIndex - lastIndex;
				break;
			}
			lastIndex = byteIndex;
		}
		return offset;
	}

	size_t LineEdit::GetMoveCaretToRightOneStepOffset() {
		if (m_caretIndex == m_string.length()) return 0;

		size_t lastIndex = 0;
		size_t offset = 0;

		if (m_caretIndex == 0) {
			auto it = m_string.begin();
			utf8::next(it, m_string.end());
			offset = it - m_string.begin();
			return offset;
		}

		for (auto it = m_string.begin(); it != m_string.end();) {
			utf8::next(it, m_string.end());
			size_t byteIndex = it - m_string.begin();
			if (byteIndex == m_caretIndex) {
				lastIndex = byteIndex;
				utf8::next(it, m_string.end());
				byteIndex = it - m_string.begin();
				offset = byteIndex - lastIndex;
				break;
			}
		}
		return offset;
	}

	size_t LineEdit::MapMousePosXToCaretIndex(float x) {
		float offset = x - m_textLbl->GetGlobalPosition().x;
		float stringW = GetFont().GetTextSize(m_string).w;

		if (offset > stringW || IsEqualApprox(offset, stringW)) return m_string.length();

		size_t lastIndex = 0;
		for (auto it = m_string.begin(); it != m_string.end();) {
			utf8::next(it, m_string.end());
			size_t byteIndex = it - m_string.begin();
			float w = GetFont().GetTextSize(m_string, byteIndex).w;
			if (offset < w - GetFont().GetSize() * 0.5f) {
				return lastIndex;
			}
			lastIndex = byteIndex;
		}

		if (lastIndex == m_string.length()) return m_string.length();

		return 0;
	}

	bool LineEdit::HandleMouseCursor(Event* event) {
		if (auto ev = event->Convert<MouseMotionEvent>()) {
			if (m_visibleGRect.ContainPoint(ev->GetPosition())) {
				SDL_SetCursor(m_cursor.get());
			}
			else if (SDL_GetCursor() == m_cursor.get()) {
				SDL_SetCursor(SDL_GetDefaultCursor());
			}
		}
		return false;
	}

	bool LineEdit::HandleInputText(Event* event) {
		if (!m_active) return false;

		// input text
		if (auto ev = event->Convert<KeyBoardTextInputEvent>()) {
			auto inputText = ev->GetInputText();
			m_string.insert(m_caretIndex, inputText);
			m_caretIndex += inputText.length();
			m_caret.SetVisible(true);

			SDL_Log("length of input text = %d", inputText.length());

			//m_string.insert(m_textCaches[m_caretIndex].totalBytes, inputText);
			//size_t oldLen = m_textCaches.size();
			//UpdateTextCaches();
			//m_caretIndex += m_textCaches.size() - oldLen;
			//m_caret.SetVisible(true);


			// limit length, need to consider the byte length of characters
			size_t lastIndex = 0;
			for (auto it = m_string.begin(); it != m_string.end();) {
				utf8::next(it, m_string.end());
				size_t byteIndex = it - m_string.begin();

				if (byteIndex > m_maxLength) {
					std::string rejectedText = m_string.substr(lastIndex, m_string.length() - lastIndex);
					m_string.erase(lastIndex, m_string.length() - lastIndex);

					if (m_caretIndex > m_maxLength) {
						m_caretIndex = m_maxLength;
					}

					if (m_caretIndex >= m_maxLength) {
						m_caretIndex = lastIndex;
					}

					textChangeRejected.Emit(rejectedText);
					break;
				}
				lastIndex = byteIndex;
			}

			m_textLbl->SetText(m_string);
			textChanged.Emit(m_string);

			Rect contentGRect = GetContentGlobalRect();

			if ((m_caretIndex == m_string.length() ||
				IsEqualApprox(m_caret.GetGlobalRect().Left(), contentGRect.Right())) &&
				m_textLbl->GetSize().w > contentGRect.size.w) {
				float w = GetFont().GetTextSize(m_string, m_caretIndex).w;
				m_textLbl->SetPositionX(contentGRect.size.w - w);
			}

			if (m_selectTextData.selecting) m_selectTextData.selecting = false;
			return true;
		}

		return false;
	}

	bool LineEdit::HandleShortKey(Event* event) {
		if (!m_active) return false;

		if (auto ev = event->Convert<KeyBoardButtonEvent>();
			ev && ev->IsPressed()) {
			switch (ev->GetKeyCode()) {
			case SDLK_BACKSPACE: {
				if (m_string.empty() || !m_caretIndex) return false;

				size_t offset = GetMoveCaretToLeftOneStepOffset();
				size_t index = (m_caretIndex - offset) < 0 ? 0 : m_caretIndex - offset;
				size_t count = offset > m_string.length() ? m_string.length() : offset;
				m_string.erase(index, count);
				m_textLbl->SetText(m_string);
				MoveCaretToLeft(offset);

				if (m_string.empty()) {
					m_textLbl->SetText(m_placeholder);
				}

				return true;
			}

							   // move caret to left
			case SDLK_LEFT: {
				MoveCaretToLeft(GetMoveCaretToLeftOneStepOffset());
				m_caret.SetVisible(true);
				return true;
			}

						  // move caret to right
			case SDLK_RIGHT: {
				MoveCaretToRight(GetMoveCaretToRightOneStepOffset());
				m_caret.SetVisible(true);
				return true;
			}

						   // Home
			case SDLK_HOME: {
				m_caretIndex = 0;
				m_textLbl->SetPositionX(0);
				m_caret.SetVisible(true);
				return true;
			}

						  // End
			case SDLK_END: {
				m_caretIndex = m_string.length();
				float contentW = GetContentGlobalRect().size.w;
				if (m_textLbl->GetSize().w > contentW) {
					m_textLbl->SetPositionX(contentW - m_textLbl->GetSize().w);
				}
				m_caret.SetVisible(true);
				return true;
			}
			}

			if (m_selectTextData.selecting) m_selectTextData.selecting = false;

			if (ev->IsCtrlAnd(SDLK_C)) {
				/*SDL_Log("copy");*/
				std::string selected = m_selectedTextLbl->GetText();
				if (selected.empty()) return false;
				SDL_SetClipboardText(selected.c_str());
				return true;
			}
			else if (ev->IsCtrlAnd(SDLK_V)) {
				std::string text = SDL_GetClipboardText();
				m_string.insert(m_caretIndex, text);
				m_textLbl->SetText(m_string);
				textChanged.Emit(m_string);
				return true;
			}
			else if (ev->IsCtrlAnd(SDLK_A)) {
				m_selectTextData.selecting = true;
				m_selectTextData.startCaretPosX = 0;
				m_selectTextData.startCaretIndex = 0;
				m_selectTextData.endCaretIndex = m_string.length();
				m_selectedTextLbl->SetText(m_string);
				return true;
			}
		}

		return false;
	}

	bool LineEdit::HandleSelectText(Event* event) {
		if (!m_secretEnable || m_string.empty()) return false;

		if (auto ev = event->Convert<MouseButtonEvent>()) {
			if (!m_visibleGRect.ContainPoint(ev->GetPosition())) {
				m_selectTextData.canSelect = false;
				return false;
			}

			if (ev->IsPressed(MouseButton::Left)) {
				m_caretIndex = MapMousePosXToCaretIndex(ev->GetPosition().x);

				m_selectTextData.canSelect = true;
				m_selectTextData.startCaretIndex = m_caretIndex;
				m_selectTextData.startMousePosX = ev->GetPosition().x;
				if (m_caretIndex == 0) m_selectTextData.startCaretPosX = 0;
				else m_selectTextData.startCaretPosX = GetFont().GetTextSize(m_string, m_caretIndex).w;
				m_selectedTextLbl->SetText("");
			}
		}

		if (m_selectTextData.canSelect) {
			if (auto ev = event->Convert<MouseButtonEvent>();
				ev && ev->IsReleased(MouseButton::Left)) {
				m_selectTextData.canSelect = false;
			}

			else if (auto ev = event->Convert<MouseMotionEvent>()) {
				m_selectTextData.selecting = true;
				m_caretIndex = MapMousePosXToCaretIndex(ev->GetPosition().x);

				size_t startIndex = m_caretIndex < m_selectTextData.startCaretIndex ? m_caretIndex : m_selectTextData.startCaretIndex;
				size_t len = SDL_abs(m_selectTextData.startCaretIndex - m_caretIndex);
				std::string selectedStr = m_string.substr(startIndex, len);
				m_selectedTextLbl->SetText(selectedStr);

				// move text lbl
				//Rect contentGRect = GetContentGlobalRect();
				//if (m_textLbl->GetSize().w > contentGRect.size.w && ev->GetPosition().x > contentGRect.size.w) {
				//	float x = m_textLbl->GetPosition().x - 10;
				//}
			}
		}

		return false;
	}

	void LineEdit::UpdateTextCaches() {
		m_textCaches.clear();

		size_t charIndex = 0;
		size_t lastIndex = 0;
		for (auto it = m_string.begin(); it != m_string.end();) {
			utf8::next(it, m_string.end());
			size_t byteIndex = it - m_string.begin();
			Vec2 size = GetFont().GetTextSize(m_string, byteIndex);
			m_textCaches.emplace_back(charIndex, byteIndex - lastIndex, byteIndex, size);

			charIndex++;
			lastIndex = byteIndex;
		}
	}
}