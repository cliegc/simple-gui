#pragma once
#include <SDL3/SDL.h>
#include <string_view>
#include <memory>
#include "window.hpp"
#include "event.hpp"
#include "framerate.hpp"
#include "timer.hpp"
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

        inline Window& GetWindow() const { return *m_window; }
        Window& GetWindow(std::string_view title, int w, int h);
        void Run();

        inline const CommandArgs& GetCommandArgs() const { return m_cmdArgs; }
        inline Font& GetDefaultFont() const { return *m_defaultResource.font; }
        inline Style& GetDefaultStyle() const { return *m_defaultResource.style; }

        inline void SetUnlimitedFrameRate(bool value) { m_fpsController->SetUnlimitedFrameRate(value); }
        inline double GetDelta() const { return m_fpsController->GetDelta(); }
        inline double GetRealFrameRate() const { return m_fpsController->GetRealFrameRate(); }
        inline uint32_t GetTargetFrameRate() const { return m_fpsController->GetTargetFrameRate(); }
        inline void SetTargetFrameRate(uint32_t fps) { m_fpsController->SetTargetFrameRate(fps); };

        inline Timer* GetTimer(float interval) { return m_timerManager->GetTimer(interval); }
        inline void KillTimer(Timer* timer) { m_timerManager->KillTiemr(timer); }

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