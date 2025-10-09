#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <variant>
#include <queue>
#include "math.hpp"
#include "texture.hpp"
#include "font.hpp"


namespace SimpleGui {
    struct RenderLineCommandData final {
        SDL_FPoint start;
        SDL_FPoint end;
    };

    struct RenderLinesCommandData final {
        std::vector<SDL_FPoint> points;
    };

    struct RenderRectCommandData final {
        SDL_FRect rect;
        bool fill;
    };

    struct RenderRectsCommandData final {
        std::vector<SDL_FRect> rects;
        bool fill;
    };

    struct RenderTriangleCommandData final {
        SDL_FPoint p1;
        SDL_FPoint p2;
        SDL_FPoint p3;
        bool fill;
    };

    //struct RenderGeometryCommandData {

    //};

    struct RenderCircleCommandData final {
        SDL_FPoint center;
        float radius;
        bool fill;
    };

    struct RenderTextureCommandData final {
        SDL_Texture* texture;
        SDL_FRect srcRect;
        SDL_FRect dstRect;
        float angle;
        SDL_FPoint center;
        SDL_FlipMode mode;
    };

    struct RenderTextCommandData final {
        TTF_Text* text;
        SDL_FPoint pos;
    };

    struct RenderClipCommandData final {
        SDL_Rect rect;
    };

    struct RenderCommand final {
        std::variant<RenderLineCommandData,
            RenderLinesCommandData,
            RenderRectCommandData,
            RenderRectsCommandData,
            RenderTriangleCommandData,
            //RenderGeometryCommandData,
            RenderCircleCommandData,
            RenderTextureCommandData,
            RenderTextCommandData,
            RenderClipCommandData> data;
        SDL_Color color;
    };


    class Renderer final {
    public:
        Renderer(SDL_Window* window);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        void SetClearColor(const Color& color);
        //void Clear();
        //void Present();

        void SetClipRect(const Rect& rect) const;
        void ClearClipRect() const;

        //void AddRenderClipCommand(const Rect& rect);
        //void AddRenderLineCommand()

        void SetRenderClipRect(const SDL_Rect& rect);
        void ClearRenderClipRect();

        void RenderLine(const SDL_FPoint& p1, const SDL_FPoint& p2, const SDL_Color& color);
        void RenderLines();
        void RenderRect(const SDL_FRect& rect, const SDL_Color& color, bool fill);
        void RenderRects();
        void RenderTriangle(const SDL_FPoint& p1, const SDL_FPoint& p2, const SDL_FPoint& p3, const SDL_Color& color, bool fill);
        void RenderCircle(const SDL_FPoint& center, float radius, const SDL_Color& color, bool fill);
        void RenderTexture(SDL_Texture* texture, const SDL_FRect& srcRect, const SDL_FRect& dstRect, float angle, const SDL_FPoint center, SDL_FlipMode mode);
        void RenderText(TTF_Text* text, const SDL_FPoint& pos, const SDL_Color& color);

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

        inline bool IsTopRender() const { return m_topRender; }
        inline void SetTopRender(bool top) { m_topRender = top; }

        void Render();

    private:
        SDL_Renderer* m_renderer;
        TTF_TextEngine* m_textEngine;
        Color m_clearColor;
        bool m_topRender;

        std::queue<RenderCommand> m_renderQueue;
        std::queue<RenderCommand> m_topRenderQueue;
       
        void SetRenderColor(const Color& color) const;
        void AddRenderCommand(RenderCommand&& cmd);
        void ExecuteRenderQueue(std::queue<RenderCommand>& queue);
    };
}