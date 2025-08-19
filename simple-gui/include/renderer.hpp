#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "math.hpp"

namespace SimpleGui {
    class Renderer final {
    public:
        Renderer(SDL_Renderer* renderer);
        ~Renderer() = default;

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        void SetClipRect(const Rect& rect) const;
        void ClearClipRect() const;
        void DrawLine(const Vec2& p1, const Vec2& p2, const Color& color) const;
        void DrawRect(const Rect& rect, const Color& color) const;
        void FillRect(const Rect& rect, const Color& color) const;
        void FillRect(const Rect& rect, const GradientColor& color) const;
        void DrawTexture(SDL_Texture* texture, const Rect& srcRect, const Rect& dstRect, float angle, const Vec2 center, SDL_FlipMode mode) const;
        void DrawText(TTF_Text* text, const Vec2& pos, const Color& color) const;
        void DrawText(TTF_Font* font, std::string_view text, const Vec2& pos, const Color& color, int wrap_width) const;

        inline SDL_Renderer* GetSDLRenderer() const { return m_renderer; }

    private:
        SDL_Renderer* m_renderer;

        void SetRenderColor(const Color& color) const;
    };
}