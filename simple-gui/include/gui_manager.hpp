#pragma once
#include <string_view>
#include <memory>
#include "window.hpp"
#include "event.hpp"
#include "framerate.hpp"
#include "timer.hpp"
#include "font.hpp"


#define SG_GuiManager SimpleGui::GuiManager::GetInstance()


namespace SimpleGui {
    class CommandArgs final {
    public:
        ~CommandArgs() = default;

        size_t count() const { return m_args.size(); }
        const std::string& operator[](const size_t index) const { return m_args[index]; }
        
    private:
        friend class GuiManager;

        std::vector<std::string> m_args;

        CommandArgs() = default;
        void Setup(int argc, char** argv);
    };

    class DefaultResource final {
    public:
        ~DefaultResource() = default;

    private:
        friend class GuiManager;

        std::unique_ptr<Font> font;
        std::unique_ptr<Style> style;

        DefaultResource() = default;
    };


    class GuiManager final {
    public:
        ~GuiManager();

        GuiManager(const GuiManager&) = delete;
        GuiManager& operator=(const GuiManager&) = delete;
        GuiManager(GuiManager&&) = delete;
        GuiManager& operator=(GuiManager&&) = delete;

        static void Init(int argc, char** argv, std::string_view fontPath);
        static void Quit();

        static GuiManager& GetInstance();

        Window& GetWindow() const { return *m_window; }
        Window& GetWindow(std::string_view title, int w, int h) const;
        void Run() const;

        const CommandArgs& GetCommandArgs() const { return m_cmdArgs; }
        const Font& GetDefaultFont() const { return *m_defaultResource.font; }
        const Style& GetDefaultStyle() const { return *m_defaultResource.style; }

        void SetUnlimitedFrameRate(bool value) const { m_fpsController->SetUnlimitedFrameRate(value); }
        void SetTargetFrameRate(uint32_t fps) const { m_fpsController->SetTargetFrameRate(fps); }
        double GetDelta() const { return m_fpsController->GetDelta(); }
        double GetRealFrameRate() const { return m_fpsController->GetRealFrameRate(); }
        uint32_t GetTargetFrameRate() const { return m_fpsController->GetTargetFrameRate(); }

        Timer* GetTimer(float interval) const { return m_timerManager->GetTimer(interval); }
        void KillTimer(Timer* timer) const { m_timerManager->KillTimer(timer); }

        Vec2 GetMousePosition() const;

    private:
        static std::unique_ptr<GuiManager> s_guiManager;

        CommandArgs m_cmdArgs;
        DefaultResource m_defaultResource;
        std::unique_ptr<Window> m_window;
        std::unique_ptr<EventManager> m_eventManager;
        std::unique_ptr<FrameRateController> m_fpsController;
        std::unique_ptr<TimerManager> m_timerManager;

        GuiManager() = default;
    };
}