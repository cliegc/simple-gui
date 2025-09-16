#include "component/line_edit.hpp"


namespace SimpleGui {
	LineEdit::LineEdit(LineEditContentType type, std::string_view placeholder) {
		m_placeholder = placeholder;
		m_contentType = type;
	}
}