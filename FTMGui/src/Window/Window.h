#pragma once

#include <cinttypes>

#include <string_view>

#include "Events/EventDispatcher.h"
#include "GLFWBase.h"


namespace FTMGui {
	
	struct WindowInfo
	{
	public:
		uint32_t width = 0;
		uint32_t height = 0;

		std::string_view title = "";
		bool vSync = false;

		bool windowClose = false;

		void AddEventListener(const EventListener& listener)
		{
			Dispatcher.AddListener(listener);
		}

		void AddEvent(const Event& e)
		{
			Dispatcher.AddEvent(e);
		}

		void DispatchEvents()
		{
			Dispatcher.Dispatch();
		}

	private:
		EventDispatcher Dispatcher;
	};

	EventCode ConvertGLFWKeyCodeToEventCode(int code);
	EventType ConvertGLFWKeyActionToEventType(int action);

	class Window
	{
	public:
		Window(const WindowInfo& info);
		Window() = default;
		~Window();

		void Update();

		void AddEventListener(const EventListener& listener);

		inline		 GLFWwindow*	  GetHandle()    const	 { return m_Handle; }
		inline const std::string_view GetTitle()     const   { return m_WindowInfo.title; }
		inline const uint32_t		  GetWidth()     const	 { return m_WindowInfo.width; }
		inline const uint32_t		  GetHeight()    const	 { return m_WindowInfo.height; }
		inline const bool			  WindowClosed() const   { return m_WindowInfo.windowClose; }

	private:
		GLFWwindow* m_Handle;
		WindowInfo m_WindowInfo;
	};
}