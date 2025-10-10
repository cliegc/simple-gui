#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <string_view>
#include <string>
#include "math.hpp"


namespace SimpleGui {
	enum class FontStyle {
		Normal = TTF_STYLE_NORMAL,
		Bold = TTF_STYLE_BOLD,
		Italic = TTF_STYLE_ITALIC,
		Underline = TTF_STYLE_UNDERLINE,
		StrikeThrough = TTF_STYLE_STRIKETHROUGH,
	};

	constexpr FontStyle operator|(FontStyle s1, FontStyle s2);

	class Font final {
	public:
		Font(std::string_view path, float ptsize);
		~Font();

		TTF_Font& GetTTFFont() const { return *m_font; }
		std::string GetPath() const { return m_path; }

		float GetSize() const { return TTF_GetFontSize(m_font); }
		void SetSize(float ptsize) const { TTF_SetFontSize(m_font, ptsize); }
		
		int GetHeight() const { return TTF_GetFontHeight(m_font); }
		Vec2 GetTextSize(std::string_view text) const {
			return GetTextSize(text, text.length());
		}

		Vec2 GetTextSize(std::string_view text, size_t length) const {
			int w, h;
			TTF_GetStringSize(m_font, text.data(), length, &w, &h);
			return Vec2(static_cast<float>(w), static_cast<float>(h));
		}

		std::string GetFamilyName() const { return TTF_GetFontFamilyName(m_font); }

		FontStyle GetStyle() const { return static_cast<FontStyle>(TTF_GetFontStyle(m_font)); }
		void SetStyle(FontStyle style) const { TTF_SetFontStyle(m_font,  static_cast<TTF_FontStyleFlags>(style));  }

		bool IsNull() const { return m_font == nullptr; }

	private:
		TTF_Font* m_font;
		std::string m_path;
	};
}