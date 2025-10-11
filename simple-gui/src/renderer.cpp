#include "renderer.hpp"
#include <SDL3_image/SDL_image.h>
#include "deleter.hpp"


namespace SimpleGui {
	class RenderCommandDataVisitor final {
	public:
		RenderCommandDataVisitor(SDL_Renderer* renderer, const SDL_Color& color) :
			m_renderer(renderer), m_color(color) {
		}
		~RenderCommandDataVisitor() = default;

		void operator()(const RenderLineCommandData& data) {
			SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
			SDL_RenderLine(m_renderer, data.start.x, data.start.y, data.end.x, data.end.y);
		}

		void operator()(const RenderLinesCommandData& data) {
			SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
			SDL_RenderLines(m_renderer, data.points.data(), data.points.size());
		}

		void operator()(const RenderRectCommandData& data) {
			SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
			if (data.fill) SDL_RenderFillRect(m_renderer, &data.rect);
			else SDL_RenderRect(m_renderer, &data.rect);
		}

		void operator()(const RenderRectsCommandData& data) {
			SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
			if (data.fill) SDL_RenderFillRects(m_renderer, data.rects.data(), data.rects.size());
			else SDL_RenderRects(m_renderer, data.rects.data(), data.rects.size());
		}

		void operator()(const RenderTriangleCommandData& data) {
			if (data.fill) {
				SDL_FColor fc = {
					m_color.r / 255.f,
					m_color.g / 255.f,
					m_color.b / 255.f,
					m_color.a / 255.f
				};
				SDL_Vertex vertices[3] = {
					{ data.p3, fc, {0}},
					{ data.p1, fc, {0}},
					{ data.p2, fc, {0}},
				};
				SDL_RenderGeometry(m_renderer, nullptr, vertices, 3, nullptr, 0);

			}
			else {
				SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
				SDL_FPoint points[4] = { data.p1, data.p2, data.p3 };
				SDL_RenderLines(m_renderer, points, 4);
			}
		}

		void operator()(const RenderCircleCommandData& data) {
			SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
			if (data.fill) {
				std::vector<SDL_FPoint> points;
				for (float y = -data.radius; y <= data.radius; y++) {
					float x = (int)sqrt(data.radius * data.radius - y * y);
					points.emplace_back(data.center.x - x, data.center.y + y);
					points.emplace_back(data.center.x + x, data.center.y + y);
				}
				SDL_RenderLines(m_renderer, points.data(), points.size());
			}
			else {
				float x = data.radius;
				float y = 0;
				float err = 0;

				while (x >= y) {
					SDL_FPoint points[8] = {
						{data.center.x + x, data.center.y + y},
						{data.center.x + y, data.center.y + x},
						{data.center.x - y, data.center.y + x},
						{data.center.x - x, data.center.y + y},
						{data.center.x - x, data.center.y - y},
						{data.center.x - y, data.center.y - x},
						{data.center.x + y, data.center.y - x},
						{data.center.x + x, data.center.y - y}
					};

					SDL_RenderPoints(m_renderer, points, 8);

					y += 1;
					err += 1 + 2 * y;
					if (2 * (err - x) + 1 > 0) {
						x -= 1;
						err += 1 - 2 * x;
					}
				}
			}
		}

		void operator()(const RenderTextureCommandData& data) {
			SDL_RenderTextureRotated(m_renderer, data.texture, &data.srcRect, &data.dstRect, data.angle, &data.center, data.mode);
		}

		void operator()(const RenderTextCommandData& data) {
			TTF_SetTextColor(data.text, m_color.r, m_color.g, m_color.b, m_color.a);
			TTF_DrawRendererText(data.text, data.pos.x, data.pos.y);
		}

		void operator()(const RenderClipCommandData& data) {
			if (data.disable) SDL_SetRenderClipRect(m_renderer, NULL);
			else SDL_SetRenderClipRect(m_renderer, &data.rect);
		}

	private:
		SDL_Renderer* m_renderer;
		SDL_Color m_color;
	};


	Renderer::Renderer(SDL_Window* window) {
		m_renderer = SDL_CreateRenderer(window, NULL);
		if (!m_renderer) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "error", SDL_GetError(), nullptr);
			SDL_Log("%s\n", SDL_GetError());
			SDL_DestroyWindow(window);
			TTF_Quit();
			SDL_Quit();
			exit(-1);
		}

		m_textEngine = TTF_CreateRendererTextEngine(m_renderer);
		if (!m_textEngine) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "error", SDL_GetError(), nullptr);
			SDL_Log("%s\n", SDL_GetError());
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			SDL_Quit();
			exit(-1);
		}

		m_topRender = false;
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	}

	Renderer::~Renderer() {
		TTF_DestroyRendererTextEngine(m_textEngine);
		SDL_DestroyRenderer(m_renderer);
	}

	void Renderer::SetClearColor(const Color& color) {
		m_clearColor = color;
	}

	//void Renderer::Clear() {
	//	SDL_SetRenderDrawColor(m_renderer, m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	//	SDL_RenderClear(m_renderer);
	//}

	//void Renderer::Present() {
	//	SDL_RenderPresent(m_renderer);
	//}

	void Renderer::SetClipRect(const Rect& rect) const {
		auto rt = rect.ToSDLRect();
		SDL_SetRenderClipRect(m_renderer, &rt);
	}

	void Renderer::ClearClipRect() const {
		SDL_SetRenderClipRect(m_renderer, NULL);
	}

	void Renderer::SetRenderClipRect(const Rect& rect) {
		RenderCommand cmd{ .data = RenderClipCommandData{rect.ToSDLRect(), false} };
		AddRenderCommand(std::move(cmd));
	}

	void Renderer::ClearRenderClipRect() {
		RenderCommand cmd{ .data = RenderClipCommandData{{}, true} };
		AddRenderCommand(std::move(cmd));
	}

	void Renderer::RenderLine(const Vec2& p1, const Vec2& p2, const Color& color) {
		RenderCommand cmd{
			.data = RenderLineCommandData{p1.ToSDLFPoint(), p2.ToSDLFPoint()},
			.color = color.ToSDLColor() };
		AddRenderCommand(std::move(cmd));
	}

	void Renderer::RenderLines(const std::vector<SDL_FPoint> points, const Color& color) {
		RenderCommand cmd{ 
			.data = {RenderLinesCommandData{points}}, 
			.color = color.ToSDLColor() };
		AddRenderCommand(std::move(cmd));
	}

	void Renderer::RenderRect(const Rect& rect, const Color& color, bool fill) {
		RenderCommand cmd{
			.data = RenderRectCommandData{rect.ToSDLFRect(), fill},
			.color = color.ToSDLColor() };
		AddRenderCommand(std::move(cmd));
	}

	void Renderer::RenderRects(const std::vector<SDL_FRect> rects, const Color& color, bool fill) {
		RenderCommand cmd{ 
			.data = RenderRectsCommandData{rects, fill}, 
			.color = color.ToSDLColor() };
		AddRenderCommand(std::move(cmd));
	}

	void Renderer::RenderTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color, bool fill) {
		RenderCommand cmd{
			.data = RenderTriangleCommandData{p1.ToSDLFPoint(), p2.ToSDLFPoint(), p3.ToSDLFPoint(), fill},
			.color = color.ToSDLColor() };
		AddRenderCommand(std::move(cmd));
	}

	void Renderer::RenderCircle(const Vec2& center, float radius, const Color& color, bool fill) {
		RenderCommand cmd{
			.data = RenderCircleCommandData{center.ToSDLFPoint(), radius, fill},
			.color = color.ToSDLColor() };
		AddRenderCommand(std::move(cmd));
	}

	void Renderer::RenderTexture(Texture* texture, const Rect& srcRect, const Rect& dstRect, float angle, const Vec2& center, SDL_FlipMode mode) {
		RenderCommand cmd{
			.data = RenderTextureCommandData{
				&texture->GetSDLTexture(),
				srcRect.ToSDLFRect(),
				dstRect.ToSDLFRect(),
				angle,
				center.ToSDLFPoint(),
				mode} };
		AddRenderCommand(std::move(cmd));
	}

	void Renderer::RenderText(TTF_Text* text, const Vec2& pos, const Color& color) {
		RenderCommand cmd{
			.data = {RenderTextCommandData{text, pos.ToSDLFPoint()}},
			.color = color.ToSDLColor() };
		AddRenderCommand(std::move(cmd));
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

		if (color.type == GradientColor::Type::Vertical) {
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
			{ rect.position.ToSDLFPoint(), topLeft, {0}},
			{ rect.TopRight().ToSDLFPoint(), topRight, {0}},
			{ rect.BottomLeft().ToSDLFPoint(), bottomLeft, {0}},

			{ rect.TopRight().ToSDLFPoint(), topRight, { 0 } },
			{ rect.BottomRight().ToSDLFPoint(), bottomRight, { 0 } },
			{ rect.BottomLeft().ToSDLFPoint(), bottomLeft, {0}}
		};
		SDL_RenderGeometry(m_renderer, nullptr, vertices, 6, nullptr, 0);
	}

	void Renderer::FillTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color) const {
		SDL_FColor fc = color.ToSDLFColor();
		SDL_Vertex vertices[3] = {
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
		if (texture.IsNull()) return;
		DrawTexture(&texture.GetSDLTexture(), srcRect, dstRect, angle, center, mode);
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

	TTF_Text* Renderer::CreateText(std::string_view text, const Font& font) const {
		return TTF_CreateText(m_textEngine, &font.GetTTFFont(), text.data(), text.size());
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

	std::shared_ptr<SDL_Texture> Renderer::CreateSharedSDLTexture(std::string_view path) const {
		SDL_Texture* tt = IMG_LoadTexture(m_renderer, path.data());
		return {tt, TextureDeleter()};
	}

	std::shared_ptr<Texture> Renderer::CreateSharedTexture(std::string_view path) const {
		return std::shared_ptr<Texture>(new Texture(*this, path));
	}

	SDL_Texture* Renderer::CreateSDLTexture(std::string_view path) const {
		return IMG_LoadTexture(m_renderer, path.data());
	}

	Texture* Renderer::CreateTexture(std::string_view path) const {
		return new Texture(*this, path);
	}

	void Renderer::Render() {
		SDL_SetRenderDrawColor(m_renderer, m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		SDL_RenderClear(m_renderer);
		ExecuteRenderQueue(m_renderQueue);
		ExecuteRenderQueue(m_topRenderQueue);
		SDL_RenderPresent(m_renderer);
	}

	void Renderer::SetRenderColor(const Color& color) const {
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	}

	void Renderer::AddRenderCommand(RenderCommand&& cmd) {
		if (m_topRender) m_topRenderQueue.push(cmd);
		else m_renderQueue.push(cmd);
	}

	void Renderer::ExecuteRenderQueue(std::queue<RenderCommand>& queue) {
		while (!queue.empty()) {
			auto& cmd = queue.front();
			RenderCommandDataVisitor visitor(m_renderer, cmd.color);
			std::visit(visitor, cmd.data);
			queue.pop();
		}
	}
}
