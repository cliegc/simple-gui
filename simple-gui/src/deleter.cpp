#include "deleter.hpp"


namespace SimpleGui {
	void TextEngineDeleter::operator()(TTF_TextEngine* engine) const noexcept {
		if (!engine) return;
		TTF_DestroyRendererTextEngine(engine);
	}
	
	void FontDeleter::operator()(TTF_Font* font) const noexcept {
		if (!font) return;
		TTF_CloseFont(font);
	}
	void TextDeleter::operator()(TTF_Text* text) const noexcept {
		if (!text) return;
		if (text) TTF_DestroyText(text);
	}
}