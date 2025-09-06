#include "font.hpp"


namespace SimpleGui {
	Font::Font(std::string_view path, float ptsize) {
		m_font = TTF_OpenFont(path.data(), ptsize);
		m_path = path;

		TTF_SetFontHinting(m_font, TTF_HINTING_LIGHT_SUBPIXEL);
	}

	Font::~Font() {
		if (m_font) {
			TTF_CloseFont(m_font);
		}
	}

	constexpr FontStyle operator|(FontStyle s1, FontStyle s2) {
		return static_cast<FontStyle>(static_cast<TTF_FontStyleFlags>(s1) | static_cast<TTF_FontStyleFlags>(s2));
	}
}