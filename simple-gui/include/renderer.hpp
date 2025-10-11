#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>
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
        bool disable;
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
        SDL_Color color{};
    };


    class Renderer final {
    public:
        explicit Renderer(SDL_Window* window);
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

        void SetRenderClipRect(const Rect& rect);
        void ClearRenderClipRect();

        void RenderLine(const Vec2& p1, const Vec2& p2, const Color& color);
        void RenderLines(const std::vector<SDL_FPoint> points, const Color& color);
        void RenderRect(const Rect& rect, const Color& color, bool fill);
        void RenderRects(const std::vector<SDL_FRect> rects, const Color& color, bool fill);
        void RenderTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color, bool fill);
        void RenderCircle(const Vec2& center, float radius, const Color& color, bool fill);
        void RenderTexture(Texture* texture, const Rect& srcRect, const Rect& dstRect, float angle, const Vec2& center, SDL_FlipMode mode);
        void RenderText(TTF_Text* text, const Vec2& pos, const Color& color);

        void DrawLine(const Vec2& p1, const Vec2& p2, const Color& color) const;
        void DrawRect(const Rect& rect, const Color& color) const;
        void DrawTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color) const;
        void DrawCircle(const Vec2& center, float radius, const Color& color) const;                 //ʹ���е�Բ�㷨
        void FillRect(const Rect& rect, const Color& color) const;
        void FillRect(const Rect& rect, const GradientColor& color) const;
        void FillTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color) const;
        void FillCircle(const Vec2& center, float radius, const Color& color) const;                //ɨ�����㷨

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

        std::shared_ptr<SDL_Texture> CreateSharedSDLTexture(std::string_view path) const;
        std::shared_ptr<Texture> CreateSharedTexture(std::string_view path) const;
        SDL_Texture* CreateSDLTexture(std::string_view path) const;
        Texture* CreateTexture(std::string_view path) const;

        SDL_Renderer& GetSDLRenderer() const { return *m_renderer; }
        TTF_TextEngine& GetTTFTextEngine() const { return *m_textEngine; }

        bool IsTopRender() const { return m_topRender; }
        void SetTopRender(bool top) { m_topRender = top; }

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