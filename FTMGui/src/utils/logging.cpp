#include "Logging.h"

#include <iostream>
#include <string>
#include <unordered_map>

namespace FTMGui {
    Logger::Logger()
    {
        std::cout.sync_with_stdio(false);

        SetLogLevel(10, "DEBUG");
        SetLogLevel(20, "INFO");
        SetLogLevel(30, "WARNING");
        SetLogLevel(40, "ERROR");
        SetLogLevel(50, "FATAL");
    }

    Logger::~Logger()
    {
    }

    void Logger::SetLogLevel(int level, const std::string& representation)
    {
        m_LogLevelMap[level] = representation;
    }


    std::string Logger::LogLevelToString(int level) const
    {
        std::string prefix = (m_Name.empty() ? "" : m_Name + ".");
        auto it = m_LogLevelMap.find(level);

        if (it != m_LogLevelMap.end())
            return prefix + it->second;
        else
            return prefix + "log." + std::to_string(level);
        
    }

    std::string Logger::Format(int level)
    {
        switch (level)
        {
        case 10:
            return m_TextColors.CYAN + "[" + LogLevelToString(level) + "] " + m_TextColors.RESET;
        case 20:
            return m_TextColors.GREEN + "[" + LogLevelToString(level) + "] " + m_TextColors.RESET;
        case 30:
            return m_TextColors.YELLOW + "[" + LogLevelToString(level) + "] " + m_TextColors.RESET;
        case 40:
            return m_TextColors.RED + "[" + LogLevelToString(level) + "] " + m_TextColors.RESET;
        case 50:
            return m_TextColors.RED + "[" + LogLevelToString(level) + "] " + m_TextColors.RESET;
        }

        return "[" + LogLevelToString(level) + "]";
    }
}