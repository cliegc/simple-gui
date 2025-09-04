#pragma once
#include <SDL3/SDL.h>
#include <string_view>
#include <memory>
#include "window.hpp"
#include "event.hpp"
#include "deleter.hpp"
#include "font.hpp"


#define SG_GuiManager SimpleGui::GuiManager::GetInstance()


namespace SimpleGui {
    class CommandArgs final {
    public:
        ~CommandArgs() = default;

        inline size_t count() const { return m_args.size(); }
        inline const std::string& operator[](size_t index) const { return m_args[index]; }
        
    private:
        friend class GuiManager;

        std::vector<std::string> m_args;

        CommandArgs() = default;
        void Setup(int argc, char** argv);
    };


    class GuiManager final {
    public:
        ~GuiManager() = default;

        GuiManager(const GuiManager&) = delete;
        GuiManager& operator=(const GuiManager&) = delete;
        GuiManager(GuiManager&&) = delete;
        GuiManager& operator=(GuiManager&&) = delete;

        static void Init(int argc, char** argv, std::string_view fontPath);
        static void Quit();

        static GuiManager& GetInstance();

        Window& CreateWindow(std::string_view title, int w, int h);
        void Run();

        inline const CommandArgs& GetCommandArgs() const { return m_cmdArgs; }

    private:
        static std::unique_ptr<GuiManager> s_guiManager;

        CommandArgs m_cmdArgs;
        std::unordered_map<WindowID, std::unique_ptr<Window>> m_windows;
        std::unique_ptr<EventManager> m_eventManager;
        std::string m_fontPath;

        GuiManager() = default;
        void HandleEvent();
        void Update();
        void Render();
    };
}