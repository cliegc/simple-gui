#pragma once
#include <SDL3/SDL.h>
#include <string_view>
#include <memory>
#include "renderer.hpp"
#include "component/base_component.hpp"

#define SG_GuiManager SimpleGui::GuiManager::GetInstance()

namespace SimpleGui {
    class GuiManager final {
    public:
        ~GuiManager();

        GuiManager(const GuiManager&) = delete;
        GuiManager& operator=(const GuiManager&) = delete;
        GuiManager(GuiManager&&) = delete;
        GuiManager& operator=(GuiManager&&) = delete;

        static void Init(SDL_Window* window, SDL_Renderer* renderer, std::string_view fontPath);
        static void Quit();

        static GuiManager& GetInstance();

        template<typename T, typename... Args>
        T* AddComponent(Args&&... args) {
            return m_rootCmp->AddChild<T>(std::forward<Args>(args)...);
        }

        TTF_TextEngine& GetTextEngine() const;
        TTF_Font& GetDefaultFont() const;
        void SetDefaultFont(std::string_view path, float size);
        void SetDefaultFontSize(float size);

        void FrameStart();
        void FrameEnd();
        void HandleEvent(const SDL_Event&);
        void Update();
        void Render();

    private:
        struct TextEgineDeleter final {
            void operator()(TTF_TextEngine* engine) {
                if (!engine) return;
                TTF_DestroyRendererTextEngine(engine);
            }
        };

        struct FontDeleter final {
            void operator()(TTF_Font* font) {
                if (!font) return;
                TTF_CloseFont(font);
            }
        };

    private:
        static std::unique_ptr<GuiManager> s_guiManager;

        SDL_Window* m_window;
        std::unique_ptr<Renderer> m_renderer;
        std::unique_ptr<TTF_TextEngine, TextEgineDeleter> m_textEngine;
        std::unique_ptr<TTF_Font, FontDeleter> m_defaultFont;
        std::unique_ptr<BaseComponent> m_rootCmp;

        GuiManager(SDL_Window* window, SDL_Renderer* renderer, std::string_view fontPath);
        void SetRootComponentSizeToFillWindow();
    };
}