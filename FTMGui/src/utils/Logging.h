#pragma once

#include <unordered_map>
#include <string_view>
#include <string>

#include <iostream>


namespace FTMGui {

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
		Logger();
		~Logger();

		template<class ...Args>
		void Debug(Args&&... args);

		template<class ...Args>
		void Info(Args&&... args);

		template<class ...Args>
		void Warn(Args&&... args);

		template<class ...Args>
		void Error(Args&&... args);

		template<class ...Args>
		void Fatal(Args&&... args);

	private:
		void SetLogLevel(int level, const std::string& representation);

		std::string Format(int level);
		std::string LogLevelToString(int level) const;

		template<class ...Args>
		void Log(int level, Args&&... args);

	private:
		std::unordered_map<int, std::string> m_LogLevelMap;
		TextColor m_TextColors;

		int m_LoggingLevel = 10;
		std::string m_Name = "";
	};
	template<class ...Args>
	inline void Logger::Log(int level, Args && ...args)
	{
		std::cout << Format(level);
		(std::cout << ... << std::forward<Args>(args));
		std::cout << std::endl;
	}

	template<class ...Args>
	inline void Logger::Debug(Args&&... args)
	{
		Log(10, std::forward<Args>(args)...);
	}
	template<class ...Args>
	inline void Logger::Info(Args&&... args)
	{
		Log(20, std::forward<Args>(args)...);
	}
	template<class ...Args>
	inline void Logger::Warn(Args&&... args)
	{
		Log(30, std::forward<Args>(args)...);
	}
	template<class ...Args>
	inline void Logger::Error(Args&&... args)
	{
		Log(40, std::forward<Args>(args)...);
	}
	template<class ...Args>
	inline void Logger::Fatal(Args&&... args)
	{
		Log(50, std::forward<Args>(args)...);
	}
	
}


