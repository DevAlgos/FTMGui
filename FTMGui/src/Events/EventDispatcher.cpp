#include "EventDispatcher.h"


namespace FTMGui {

	void EventDispatcher::AddListener(const EventListener& listener)
	{
		m_Listeners.push_back(listener);
	}
	void EventDispatcher::AddEvent(const Event& event)
	{
		m_EventQueue.push(event);
	}
	void EventDispatcher::Dispatch()
	{
		if (m_EventQueue.empty())
			return;

		for (const EventListener& listner : m_Listeners)
		{
			if (m_EventQueue.empty())
				return;

			Event& e = m_EventQueue.front();
			listner.OnEvent(e);
			if (e.handled == true)
				m_EventQueue.pop();
		}

		/*
		* Events that did not get handled
		*/

		while (!m_EventQueue.empty())
		{
			m_EventQueue.pop();
		}
	}
}
