#pragma once
#include <SDL3/SDL.h>
#include <string_view>
#include <memory>
#include "deleter.hpp"
#include "renderer.hpp"
#include "style.hpp"
#include "component/root_component.hpp"
//#include "component/base_component.hpp"


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

        SDL_Window& GetWindow() const;
        Renderer& GetRenderer() const;
        TTF_TextEngine& GetTextEngine() const;
        TTF_Font& GetDefaultFont() const;
        RootComponent& GetRootComponent() const;

        void SetDefaultFont(std::string_view path, float size);
        void SetDefaultFontSize(float size);

        Style& GetCurrentStyle();
        std::optional<std::reference_wrapper<Style>> GetStyle(const std::string& name);
        void SwitchStyle(const std::string& name);
        bool RegisterStyle(const std::string& name, std::unique_ptr<Style> style);
        bool UnregisterStyle(const std::string& name);
        void SetStyleFollowSystem();

        void HandleEvent(const SDL_Event&);
        void Update();
        void Render();

    private:
        static std::unique_ptr<GuiManager> s_guiManager;

        SDL_Window* m_window;
        UniqueTextEnginePtr m_textEngine;
        UniqueFontPtr m_defaultFont;
        std::unique_ptr<Renderer> m_renderer;
        //std::unique_ptr<BaseComponent> m_rootCmp;
        std::unique_ptr<RootComponent> m_rootCmp;
        std::unique_ptr<StyleManager> m_styleManager;

        GuiManager(SDL_Window* window, SDL_Renderer* renderer, std::string_view fontPath);
        void SetRootComponentSizeToFillWindow();
    };
}