#include <iostream>
#include <string>

struct TextColor
{
    const std::string RESET = "\033[0m";
    const std::string BLACK = "\033[0;30m";
    const std::string RED = "\033[0;31m";
    const std::string GREEN = "\033[0;32m";
    const std::string YELLOW = "\033[0;33m";
    const std::string BLUE = "\033[0;34m";
    const std::string MAGENTA = "\033[0;35m";
    const std::string CYAN = "\033[0;36m";
    const std::string WHITE = "\033[0;37m";
};

class Logger
{
public:
    int logging_level = 10;
    std::string name = "";
    Logger()
    {
        setLogLevel(10, "DEBUG");
        setLogLevel(20, "INFO");
        setLogLevel(30, "WARNING");
        setLogLevel(40, "ERROR");
        setLogLevel(50, "FATAL");
    }
    void setLogLevel(int level, const std::string &representation)
    {
        logLevelMap[level] = representation;
    }

    std::string logLevelToString(int level) const
    {
        std::string prefix = (name.empty() ? "" : name + ".");
        auto it = logLevelMap.find(level);
        
        if (it != logLevelMap.end())
        {
            return prefix + it->second;
        }
        else
        {
            return prefix + "log." + std::to_string(level);
        }
    }
    std::string format(int level, const std::string &message)
    {
        switch (level)
        {
        case 10:
            return text_colors.CYAN + "[" + logLevelToString(level) + "] " + text_colors.RESET + message;
        case 20:
            return text_colors.GREEN + "[" + logLevelToString(level) + "] " + text_colors.RESET + message;
        case 30:
            return text_colors.YELLOW + "[" + logLevelToString(level) + "] " + text_colors.RESET + message;
        case 40:
            return text_colors.RED + "[" + logLevelToString(level) + "] " + text_colors.RESET + message;
        case 50:
            return text_colors.RED + "[" + logLevelToString(level) + "] " + text_colors.RESET + message;
        }

        return "[" + logLevelToString(level) + "] " + message;
    }

    void log(int level, const std::string &message)
    {
        if (level >= logging_level)
        {

            std::cout << format(level, message) << std::endl;
        }
    }

    void debug(const std::string &message)
    {
        log(10, message);
    }
    void info(const std::string &message)
    {
        log(20, message);
    }
    void warn(const std::string &message)
    {
        log(30, message);
    }
    void error(const std::string &message)
    {
        log(40, message);
    }
    void fatal(const std::string &message)
    {
        log(50, message);
    }

private:
    std::unordered_map<int, std::string> logLevelMap;
    TextColor text_colors;
};

int main()
{
    Logger l;
    l.logging_level = 0;
    // l.name = "MAIN";
    l.debug("Hello");
    l.info("Hello");
    l.warn("Hello");
    l.error("Hello");
    l.fatal("Hello");
    l.setLogLevel(11, "graphic.LOG");
    l.log(11, "Hello");
    return 0;
}