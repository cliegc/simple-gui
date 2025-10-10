#pragma once
#include "base_component.hpp"
#include "label.hpp"
#include "common/caret.hpp"


namespace SimpleGui {
	class LineEdit final : public BaseComponent {
	public:
		explicit LineEdit(std::string_view placeholder = "");
		~LineEdit() override = default;

		bool HandleEvent(Event* event) override;
		void Update() override;
		void Render(Renderer& renderer) override;

		std::string GetText() const { return m_string; }
		std::string GetSelectedText() const { return m_selectedTextLbl->GetText(); }
		//void SetSelectedTextRange(int startIndex, int endIndex);

		size_t GetMaxInputLength() const { return m_maxLength; }
		void SetMaxInputLength(size_t length) {
			length = SDL_clamp(length, 0, MAX_LENGTH);
			m_maxLength = length;
		}

		std::string GetPlaceholder() const { return m_placeholder; }
		void SetPlaceholder(std::string_view placeholder) { m_placeholder = placeholder; }

		bool IsEditable() const { return m_editable; }
		void SetEditable(bool value) { m_editable = value; }

		bool IsSelectingEnabled() const { return m_selectingEnabled; }
		void SetSelectingEnabled(bool value) { m_selectingEnabled = value; }

		bool IsSecretEnable() const { return m_secretEnable; }
		void SetSecretEnable(bool value) { m_secretEnable = value; }

		char GetSecretChar() const { return m_secretChar; }
		void SetSecretChar(char c) { m_secretChar = c; }

		bool IsCaretBlink() const { return m_caret.IsBlink(); }
		void SetCaretBlink(bool blink) { m_caret.SetBlink(blink); }
		float GetCaretBlinkInterval() const { return m_caret.GetBlinkInterval(); }
		void SetCaretBlinkInterval(float interval) { m_caret.SetBlinkInterval(interval); }

		inline void SetFont(std::unique_ptr<Font> font) override;
		void SetFont(std::string_view path, int size) override;

	public:
		Signal<const std::string&> textChanged;
		Signal<const std::string&> textChangeRejected;

	protected:
		void EnteredComponentTree() override;

	private:
		struct SelectTextData final {
			float startCaretPosX;
			float startMousePosX;
			int startCaretIndex;
			int endCaretIndex;
			bool canSelect;
			bool selecting;
		};

		const size_t MAX_LENGTH = 512;

	private:
		struct SingleCharData final {
			size_t index{};
			int bytes{};
			size_t totalBytes{};
			Vec2 totalSize;
		};

		std::vector<SingleCharData> m_textCaches;

		void UpdateTextCaches();

	private:
		std::unique_ptr<Label> m_textLbl;
		std::unique_ptr<Label> m_selectedTextLbl;
		std::string m_string;
		std::string m_placeholder;
		UniqueCursorPtr m_cursor;
		SelectTextData m_selectTextData{};
		Caret m_caret;
		size_t m_caretIndex;
		size_t m_maxLength;
		bool m_active;
		bool m_editable;
		bool m_selectingEnabled;
		bool m_secretEnable;
		char m_secretChar;

		bool IsShowPlaceholder() const { return m_string.empty() && !m_placeholder.empty(); }
		
		void MoveCaretToLeft(int offset = 1);
		void MoveCaretToRight(int offset = 1);
		size_t GetMoveCaretToLeftOneStepOffset();
		size_t GetMoveCaretToRightOneStepOffset();
		size_t MapMousePosXToCaretIndex(float x);

		bool HandleMouseCursor(Event* event) const;
		bool HandleInputText(Event* event);
		bool HandleShortKey(Event* event);
		bool HandleSelectText(Event* event);
	};
}