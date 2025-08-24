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
		TTF_DestroyText(text);
	}

	void CursorDeleter::operator()(SDL_Cursor* cursor) const noexcept {
		if (!cursor) return;
		SDL_DestroyCursor(cursor);
	}

	void TextureDeleter::operator()(SDL_Texture* texture) const noexcept {
		if (!texture) return;
		SDL_DestroyTexture(texture);
	}
}