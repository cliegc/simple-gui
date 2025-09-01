#include "renderer.hpp"
#include "deleter.hpp"


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

	void Renderer::DrawTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color) const {
		SetRenderColor(color);
		SDL_FPoint points[4] = {
			p1.ToSDLFPoint(), p2.ToSDLFPoint(), p3.ToSDLFPoint(), p1.ToSDLFPoint()
		};

		SDL_RenderLines(m_renderer, points, 4);
	}

	void Renderer::DrawCircle(const Vec2& center, float radius, const Color& color) const {
		float x = radius;
		float y = 0;
		float err = 0;

		SetRenderColor(color);

		while (x >= y) {
			SDL_RenderPoint(m_renderer, center.x + x, center.y + y);
			SDL_RenderPoint(m_renderer, center.x + y, center.y + x);
			SDL_RenderPoint(m_renderer, center.x - y, center.y + x);
			SDL_RenderPoint(m_renderer, center.x - x, center.y + y);
			SDL_RenderPoint(m_renderer, center.x - x, center.y - y);
			SDL_RenderPoint(m_renderer, center.x - y, center.y - x);
			SDL_RenderPoint(m_renderer, center.x + y, center.y - x);
			SDL_RenderPoint(m_renderer, center.x + x, center.y - y);

			y += 1;
			err += 1 + 2 * y;
			if (2 * (err - x) + 1 > 0) {
				x -= 1;
				err += 1 - 2 * x;
			}
		}
	}

	void Renderer::FillRect(const Rect& rect, const Color& color) const {
		auto rt = rect.ToSDLFRect();
		SetRenderColor(color);
		SDL_RenderFillRect(m_renderer, &rt);
	}

	void Renderer::FillRect(const Rect& rect, const GradientColor& color) const {
		SDL_FColor topLeft = color.start.ToSDLFColor();
		SDL_FColor topRight = color.end.ToSDLFColor();
		SDL_FColor bottomRight = color.end.ToSDLFColor();
		SDL_FColor bottomLeft = color.start.ToSDLFColor();

		if (color.type == GradientColor::Type::Vertival) {
			topRight = color.start.ToSDLFColor();
			bottomLeft = color.end.ToSDLFColor();
		}
		else if (color.type == GradientColor::Type::MainDiagonal) {
			SDL_FColor color = {
				(topLeft.r + bottomRight.r) / 2.0f,
				(topLeft.g + bottomRight.g) / 2.0f,
				(topLeft.b + bottomRight.b) / 2.0f,
				(topLeft.a + bottomRight.a) / 2.0f
			};

			topRight = color;
			bottomLeft = color;
		}
		else if (color.type == GradientColor::Type::SecondaryDiagonal) {
			SDL_FColor color = {
				(topLeft.r + bottomRight.r) / 2.0f,
				(topLeft.g + bottomRight.g) / 2.0f,
				(topLeft.b + bottomRight.b) / 2.0f,
				(topLeft.a + bottomRight.a) / 2.0f
			};

			topRight = topLeft;
			bottomLeft = bottomRight;
			topLeft = color;
			bottomRight = color;
		}

		SDL_Vertex vertices[6] = {
			// 第一个三角形 (左上、右上、左下)
			{ rect.position.ToSDLFPoint(), topLeft, {0}},
			{ rect.TopRight().ToSDLFPoint(), topRight, {0}},
			{ rect.BottomLeft().ToSDLFPoint(), bottomLeft, {0}},

			// 第二个三角形 (右上、右下、左下)
			{ rect.TopRight().ToSDLFPoint(), topRight, { 0 } },
			{ rect.BottomRight().ToSDLFPoint(), bottomRight, { 0 } },
			{ rect.BottomLeft().ToSDLFPoint(), bottomLeft, {0}}
		};
		SDL_RenderGeometry(m_renderer, nullptr, vertices, 6, nullptr, 0);
	}

	void Renderer::FillTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color) const {
		SDL_FColor fc = color.ToSDLFColor();
		SDL_Vertex vertices[3] = {
			// 第一个三角形 (左上、右上、左下)
			{ p1.ToSDLFPoint(), fc, {0}},
			{ p2.ToSDLFPoint(), fc, {0}},
			{ p3.ToSDLFPoint(), fc, {0}},
		};
		SDL_RenderGeometry(m_renderer, nullptr, vertices, 3, nullptr, 0);
	}

	void Renderer::FillCircle(const Vec2& center, float radius, const Color& color) const {
		SetRenderColor(color);
		for (float y = -radius; y <= radius; y++) {
			float x = (int)sqrt(radius * radius - y * y);
			SDL_RenderLine(m_renderer, center.x - x, center.y + y, center.x + x, center.y + y);
		}
	}

	void Renderer::DrawTexture(SDL_Texture* texture, const Rect& srcRect, const Rect& dstRect, float angle, const Vec2 center, SDL_FlipMode mode) const {
		auto sRt = srcRect.ToSDLFRect();
		auto dRt = dstRect.ToSDLFRect();
		auto cPoint = center.ToSDLFPoint();
		SDL_RenderTextureRotated(m_renderer, texture, &sRt, &dRt, angle, &cPoint, mode);
	}

	void Renderer::DrawTexture(const Texture& texture, const Rect& srcRect, const Rect& dstRect, float angle, const Vec2 center, SDL_FlipMode mode) const {
		DrawTexture(texture.GetSDLTexture(), srcRect, dstRect, angle, center, mode);
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

	Vec2 Renderer::GetRenderPosition(const Vec2& mousePos) const {
		Vec2 pos;
		SDL_RenderCoordinatesFromWindow(m_renderer, mousePos.x, mousePos.y, &pos.x, &pos.y);
		return pos;
	}

	Vec2 Renderer::GetRenderPositionFromMouse() const {
		Vec2 mousePos, pos;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		SDL_RenderCoordinatesFromWindow(m_renderer, mousePos.x, mousePos.y, &pos.x, &pos.y);
		return pos;
	}

	Vec2 Renderer::GetRenderOutputSize() const {
		int w, h;
		SDL_GetRenderOutputSize(m_renderer, &w, &h);
		return Vec2(w, h);
	}

	std::shared_ptr<SDL_Texture> Renderer::CreateSharedSDLTexture(std::string_view path) {
		SDL_Texture* tt = IMG_LoadTexture(m_renderer, path.data());
		return std::move(std::shared_ptr<SDL_Texture>(tt, TextureDeleter()));
	}

	std::shared_ptr<Texture> Renderer::CreateSharedTexture(std::string_view path) {
		return std::move(std::make_shared<Texture>(path));
	}

	void Renderer::SetRenderColor(const Color& color) const {
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	}
}
