#include "Log.h"

namespace FTMGui {

	std::shared_ptr<Logger> Log::s_MainLogger;

	void Log::Init()
	{
		s_MainLogger = std::make_shared<Logger>();
	}
}