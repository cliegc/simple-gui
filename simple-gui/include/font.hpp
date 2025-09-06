#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <string_view>
#include <string>


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

		inline TTF_Font& GetTTFFont() const { return *m_font; }
		inline std::string GetPath() const { return m_path; }

		inline float GetSize() const { return TTF_GetFontSize(m_font); }
		inline void SetSize(float ptsize) { TTF_SetFontSize(m_font, ptsize); }
		
		inline int GetHeight() const { TTF_GetFontHeight(m_font); }

		inline std::string GetFamilyName() const { TTF_GetFontFamilyName(m_font); }

		inline FontStyle GetStyle() const { return static_cast<FontStyle>(TTF_GetFontStyle(m_font)); }
		inline void SetStyle(FontStyle style) { TTF_SetFontStyle(m_font,  static_cast<TTF_FontStyleFlags>(style));  }

		inline bool IsNull() const { return m_font == nullptr; }

	private:
		TTF_Font* m_font;
		std::string m_path;
	};
}