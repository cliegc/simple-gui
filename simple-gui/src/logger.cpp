#include "logger.hpp"
#include <chrono>
#include <ctime>


#ifdef _WIN32
#include <windows.h>
constexpr WORD GREEN = FOREGROUND_GREEN;
constexpr WORD RED = FOREGROUND_RED;
constexpr WORD YELLOW = FOREGROUND_RED | FOREGROUND_GREEN;
constexpr WORD WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
#endif


namespace SimpleGui {
    Logger & Logger::GetInstance() {
        static Logger instance{};
        return instance;
    }

    void Logger::Setup(bool consoleLog, bool retainLog) {
        m_consoleLog = consoleLog;
        m_retainLogs = retainLog;
    }

    Logger::LogTime::LogTime(const std::string &date)
        : day(date.substr(0, 3)), dayNumber(date.substr(8, 2)), month(date.substr(4, 3)),
          year(date.substr(20, 4)), time(date.substr(11, 8)) {}

    std::string Logger::CurrentDateTime() const {
        auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        char buffer[32];
        ctime_s(buffer, sizeof(buffer), &time);

        LogTime logTime{ std::string(buffer) };
        return std::format("{0}-{1}-{2} {3}", logTime.year, logTime.month, logTime.dayNumber, logTime.time);
    }

    void Logger::OutputToConsole(LogLevel level, const std::string &message) const {
        if (!m_consoleLog) return;

#ifdef _WIN32
        WORD color = level == LogLevel::Info ? GREEN : (level == LogLevel::Warn ? YELLOW : RED);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
        std::cout << message;
        SetConsoleTextAttribute(hConsole, WHITE);
#endif
    }

    void Logger::RetainLogs(LogLevel level, const std::string &message) {
        if (!m_retainLogs) return;
        m_logEntries.emplace_back(level, message);
    }
}
