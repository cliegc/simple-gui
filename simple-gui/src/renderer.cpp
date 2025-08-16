#include "renderer.hpp"


namespace SimpleGui {
	Renderer::Renderer(SDL_Renderer* renderer) : m_renderer(renderer) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}

	void Renderer::SetClipRect(const Rect& rect) const {
		auto rt = rect.ToSDLRect();
		SDL_SetRenderClipRect(m_renderer, &rt);
	}

	void Renderer::ClearClipRect() const {
		SDL_SetRenderClipRect(m_renderer, NULL);
	}

	void Renderer::DrawLine(const Vec2& p1, const Vec2& p2, const Color& color) const {
		SetRenderColor(color);
		SDL_RenderLine(m_renderer, p1.x, p1.y, p2.x, p2.y);
	}

	void Renderer::DrawRect(const Rect& rect, const Color& color) const {
		auto rt = rect.ToSDLFRect();
		SetRenderColor(color);
		SDL_RenderRect(m_renderer, &rt);
	}

	void Renderer::FillRect(const Rect& rect, const Color& color) const {
		auto rt = rect.ToSDLFRect();
		SetRenderColor(color);
		SDL_RenderFillRect(m_renderer, &rt);
	}

	void Renderer::DrawTexture(SDL_Texture* texture, const Rect& srcRect, const Rect& dstRect, float angle, const Vec2 center, SDL_FlipMode mode) const {
		auto sRt = srcRect.ToSDLFRect();
		auto dRt = dstRect.ToSDLFRect();
		auto cPoint = center.ToSDLFPoint();
		SDL_RenderTextureRotated(m_renderer, texture, &sRt, &dRt, angle, &cPoint, mode);
	}

	void Renderer::DrawText(TTF_Text* text, const Vec2& pos, const Color& color) const {
		TTF_SetTextColor(text, color.r, color.g, color.b, color.a);
		TTF_DrawRendererText(text, pos.x, pos.y);
	}

	void Renderer::DrawText(TTF_Font* font, std::string_view text, const Vec2& pos, const Color& color, int wrap_width) const {
		SDL_Color clr = color.ToSDLColor();
		SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.data(), text.length(), clr, wrap_width);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
		SDL_FRect rect = { pos.x, pos.y, static_cast<float>(textTexture->w), static_cast<float>(textTexture->h) };
		SDL_RenderTexture(m_renderer, textTexture, NULL, &rect);
		SDL_DestroyTexture(textTexture);
		SDL_DestroySurface(textSurface);
	}

	void Renderer::SetRenderColor(const Color& color) const {
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	}
}
