#pragma once

#include "Logging.h"

namespace FTMGui {
	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<Logger>& GetLogger() { return s_MainLogger; };
	public:
		static std::shared_ptr<Logger> s_MainLogger;
	};


}

#define FTMGUI_LOG_DEBUG(...) FTMGui::Log::GetLogger()->Debug(__VA_ARGS__)
#define FTMGUI_LOG_INFO(...)  FTMGui::Log::GetLogger()->Info(__VA_ARGS__)
#define FTMGUI_LOG_WARN(...)  FTMGui::Log::GetLogger()->Warn(__VA_ARGS__)
#define FTMGUI_LOG_ERROR(...) FTMGui::Log::GetLogger()->Error(__VA_ARGS__)
#define FTMGUI_LOG_FATAL(...) FTMGui::Log::GetLogger()->Fatal(__VA_ARGS__)



#ifdef WIN32
	#define FTMGUI_BREAK() __debugbreak()
#else
//need to add for apple and linux etc..
	#define FTMGUI_BREAK()
#endif

#ifdef _DEBUG
	#define FTMGUI_ASSERT(condition, ...) if(!condition) {FTMGUI_LOG_FATAL(__VA_ARGS__); FTMGUI_BREAK();}
#else
	#define FTMGUI_ASSERT(condition, ...) if(!condition) {FTMGUI_LOG_FATAL(__VA_ARGS__);}

#endif