#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "math.hpp"
#include "texture.hpp"
#include "font.hpp"


namespace SimpleGui {
    class Renderer final {
    public:
        Renderer(SDL_Window* window);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        void SetClearColor(const Color& color);
        void Clear();
        void Present();

        void SetClipRect(const Rect& rect) const;
        void ClearClipRect() const;

        void DrawLine(const Vec2& p1, const Vec2& p2, const Color& color) const;
        void DrawRect(const Rect& rect, const Color& color) const;
        void DrawTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color) const;
        void DrawCircle(const Vec2& center, float radius, const Color& color) const;                 //使用中点圆算法
        void FillRect(const Rect& rect, const Color& color) const;
        void FillRect(const Rect& rect, const GradientColor& color) const;
        void FillTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color) const;
        void FillCircle(const Vec2& center, float radius, const Color& color) const;                //扫描线算法

        //void DrawRoundRect();
        //void FillRoundRect();

        void DrawTexture(SDL_Texture* texture, const Rect& srcRect, const Rect& dstRect, float angle, const Vec2 center, SDL_FlipMode mode) const;
        void DrawTexture(const Texture& texture, const Rect& srcRect, const Rect& dstRect, float angle, const Vec2 center, SDL_FlipMode mode) const;
        
        void DrawText(TTF_Text* text, const Vec2& pos, const Color& color) const;
        void DrawText(TTF_Font* font, std::string_view text, const Vec2& pos, const Color& color, int wrap_width) const;

        TTF_Text* CreateText(std::string_view text, const Font& font) const;

        Vec2 GetRenderPosition(const Vec2& mousePos) const;
        Vec2 GetRenderPositionFromMouse() const;
        Vec2 GetRenderOutputSize() const;

        std::shared_ptr<SDL_Texture> CreateSharedSDLTexture(std::string_view path);
        std::shared_ptr<Texture> CreateSharedTexture(std::string_view path);
        SDL_Texture* CreateSDLTexture(std::string_view path);
        Texture* CreateTexture(std::string_view path);

        inline SDL_Renderer& GetSDLRenderer() const { return *m_renderer; }
        inline TTF_TextEngine& GetTTFTextEngine() const { return *m_textEngine; }

    private:
        SDL_Renderer* m_renderer;
        TTF_TextEngine* m_textEngine;
        Color m_clearColor;
       
        void SetRenderColor(const Color& color) const;
    };
}