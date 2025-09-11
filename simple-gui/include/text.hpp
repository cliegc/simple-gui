#pragma once
#include <SDL3_ttf/SDL_textengine.h>
#include <string>
#include <string_view>


namespace SimpleGui {
	class Text final {
	public:
		Text(std::string_view text);
		~Text();

		inline std::string GetCString() const { m_ttfText->text; }

	private:
		//std::string m_content;
		TTF_Text* m_ttfText;
	};
}