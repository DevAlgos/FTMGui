#include "Log.h"

namespace FTMGui {

	Ref<Logger> Log::s_MainLogger;

	void Log::Init()
	{
		s_MainLogger = MakeRef<Logger>();
	}
}