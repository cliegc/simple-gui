#pragma once
#include "base_component.hpp"
#include "label.hpp"


namespace SimpleGui {
	enum class LineEditContentType {
		Text,
		Int,
		Float,
	};

	class LineEdit final : public BaseComponent {
	public:
		LineEdit(LineEditContentType type = LineEditContentType::Text, std::string_view placeholder = "");
		~LineEdit() = default;

		virtual bool HandleEvent(Event* event) override;
		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

		inline std::string GetContentText() const { return m_content; }
		inline int GetContentInt() const {};
		inline float GetContentFloat() const {};
		
		inline std::string GetPlaceholder() const { return m_placeholder; }
		inline void SetPlaceholder(std::string_view placeholder) { m_placeholder = placeholder; }

		inline LineEditContentType GetContentType() const { return m_contentType; }
		inline void SetContentType(LineEditContentType type) { m_contentType = type; }

		inline Alignment GetAlignment() const { return m_aligment; }
		inline void SetAlignment(Alignment alignment) { m_aligment = alignment; }

		inline bool IsEditable() const { return m_editable; }
		inline void SetEditable(bool value) { m_editable = value; }

		inline bool IsSelectingEnabled() const { return m_selectingEnabled; }
		inline void SetSelectingEnabled(bool value) { m_selectingEnabled = value; }

		inline bool IsSecretEnable() const { return m_secretEnable; }
		inline void SetSecretEnable(bool value) { m_secretEnable = value; }

		inline char GetSecretChar() const { return m_secretChar; }
		inline void SetSecretChar(char c) { m_secretChar = c; }

	private:
		std::string m_content;
		std::string m_placeholder;
		LineEditContentType m_contentType;
		Alignment m_aligment = Alignment::Begin;
		Caret m_caret;
		bool m_editable = true;
		bool m_selectingEnabled = true;
		bool m_secretEnable = false;
		char m_secretChar = '*';
	};
}