#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <format>
#include <source_location>


#define SG_INFO(x, ...) SimpleGui::Logger::GetInstance().Info(x, __VA_ARGS__);
#define SG_WARN(x, ...) SimpleGui::Logger::GetInstance().Warn(x, __VA_ARGS__);
#define SG_ERROR(x, ...) SimpleGui::Logger::GetInstance().Error(std::source_location::current(), x, __VA_ARGS__);


namespace SimpleGui {
    enum class LogLevel {
        Info,
        Warn,
        Error,
        None
    };

    struct LogAEntry final {
        LogLevel level{};
        std::string message{};
    };

    class Logger final {
    public:
        ~Logger() = default;

        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;
        Logger(Logger &&) = delete;
        Logger &operator=(Logger &&) = delete;

        static Logger& GetInstance();

        void Setup(bool consoleLog, bool retainLog);
        const std::vector<LogAEntry>& GetLogEntries() const { return m_logEntries; }
        bool SaveToFile(const std::string& filePath) const;
        void Clear();

        template<typename... Args>
        void Info(const std::string &message, Args &&... args) {
            std::stringstream ss;
            ss << "SimpleGui [INFO]: "
                    << CurrentDateTime() << " - " << std::vformat(
                        message, std::make_format_args(std::forward<Args>(args)...)) << "\n";

            OutputToConsole(LogLevel::Info, ss.str());
            RetainLogs(LogLevel::Info, ss.str());
        }

        template<typename... Args>
        void Warn(const std::string &message, Args &&... args) {
            std::stringstream ss;
            ss << "SimpleGui [WARN]: "
                    << CurrentDateTime() << " - " << std::vformat(
                        message, std::make_format_args(std::forward<Args>(args)...)) << "\n";

            OutputToConsole(LogLevel::Warn, ss.str());
            RetainLogs(LogLevel::Warn, ss.str());
        }

        template<typename... Args>
        void Error(std::source_location location, const std::string &message, Args &&... args) {
            std::stringstream ss;
            ss << "SimpleGui [ERROR]: "
                    << CurrentDateTime() << " - " << std::vformat(
                        message, std::make_format_args(std::forward<Args>(args)...))
                    << "\nFUNC: " << location.function_name()
                    << "\nLINE: " << location.line() << "\n\n";

            OutputToConsole(LogLevel::Error, ss.str());
            RetainLogs(LogLevel::Error, ss.str());
        }

    private:
        std::vector<LogAEntry> m_logEntries {};
        bool m_consoleLog{true};
        bool m_retainLogs{false};

        struct LogTime final {
            std::string day;
            std::string dayNumber;
            std::string month;
            std::string year;
            std::string time;

            explicit LogTime(const std::string& date);
        };

        Logger() = default;

        std::string CurrentDateTime() const;
        void OutputToConsole(LogLevel level, const std::string& message) const;
        void RetainLogs(LogLevel level, const std::string& message);
    };
}