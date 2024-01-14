#pragma once

#include "Event.h"

#include <functional>

namespace FTMGui {
	struct EventListener
	{
		std::function<void(Event&)> OnEvent;
	};
}