#include "text.hpp"


namespace SimpleGui {
	Text::Text(std::string_view text) {
		//m_content = text;
		//m_ttfText = TTF_Create
	}

	Text::~Text() {
		if (m_ttfText) {
			TTF_DestroyText(m_ttfText);
		}
	}
}