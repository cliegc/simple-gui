#pragma once
#include "base_component.hpp"
#include "label.hpp"


namespace SimpleGui {
	class LineEdit final : public BaseComponent {
	public:
		LineEdit(std::string_view placeholder = "");
		~LineEdit() = default;

		virtual bool HandleEvent(Event* event) override;
		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

		inline std::string GetText() const { return m_textLbl->GetText(); }

		inline std::string GetPlaceholder() const { return m_placeholder; }
		inline void SetPlaceholder(std::string_view placeholder) { m_placeholder = placeholder; }

		inline Alignment GetAlignment() const { return m_aligment; }
		void SetAlignment(Alignment alignment);

		inline bool IsEditable() const { return m_editable; }
		inline void SetEditable(bool value) { m_editable = value; }

		inline bool IsSelectingEnabled() const { return m_selectingEnabled; }
		inline void SetSelectingEnabled(bool value) { m_selectingEnabled = value; }

		inline bool IsSecretEnable() const { return m_secretEnable; }
		inline void SetSecretEnable(bool value) { m_secretEnable = value; }

		inline char GetSecretChar() const { return m_secretChar; }
		inline void SetSecretChar(char c) { m_secretChar = c; }

		inline bool IsCaretBlink() const { return m_caret.IsBlink(); }
		inline void SetCaretBlink(bool blink) { m_caret.SetBlink(blink); }
		inline float GetCaretBlinkInterval() { return m_caret.GetBlinkInterval(); }
		inline void SetCaretBlinkInterval(float interval) { m_caret.SetBlinkInterval(interval); }

	protected:
		virtual void EnteredComponentTree() override;

	private:
		std::unique_ptr<Label> m_textLbl;
		std::unique_ptr<Label> m_selectedTextLbl;
		std::string m_string;
		std::string m_placeholder;
		Alignment m_aligment;
		Caret m_caret;
		bool m_active;
		bool m_editable;
		bool m_selectingEnabled;
		bool m_secretEnable;
		char m_secretChar;

		inline bool IsShowPlaceholder() const { return m_string.empty() && !m_placeholder.empty(); }
	};
}