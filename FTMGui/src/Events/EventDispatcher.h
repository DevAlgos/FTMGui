#pragma once

#include "EventListener.h"

#include <queue>

namespace FTMGui {

	/*
	* buffered event dispatcher
	*/
	
	class EventDispatcher
	{
	public:
		EventDispatcher() = default;
		~EventDispatcher() = default;

		void AddListener(const EventListener& listener);
		void AddEvent(const Event& event);

		/*
		* dispatch all the events in the queues to the listeners
		*/
		void Dispatch(); 

	private:
		std::queue<Event> m_EventQueue;
		std::vector<EventListener> m_Listeners;
	};
}