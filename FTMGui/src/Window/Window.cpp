#include "Window.h"

#include "Utils/Log.h"

namespace FTMGui {
	
	Window::Window(const WindowInfo& info)
		: m_Handle(nullptr), m_WindowInfo(info)
	{

		EventListener listner;
		listner.OnEvent = [](Event& e) {printf("%i", e.code); e.handled = true; };
		m_WindowInfo.AddEventListener(listner);

		if (!glfwInit())
			FTMGUI_HALT();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Handle = glfwCreateWindow(m_WindowInfo.width, m_WindowInfo.height, m_WindowInfo.title.data(), nullptr, nullptr);

		/*window creation failure*/
		FTMGUI_ASSERT(m_Handle, "window failed");

		/*
		* lets glfw access data from s_FTMGuiGlobals, this is nice to have
		* so we can access our viewport from within callback functions
		*/
		glfwSetWindowUserPointer(m_Handle, &m_WindowInfo);

		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* window)
			{
				WindowInfo* self = (WindowInfo*)(glfwGetWindowUserPointer(window));
				self->windowClose = true;
			});

		glfwSetFramebufferSizeCallback(m_Handle, [](GLFWwindow* window, int width, int height)
			{
				WindowInfo* self = (WindowInfo*)(glfwGetWindowUserPointer(window));

				self->width = width;
				self->height = height;
			});

		glfwSetKeyCallback(m_Handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
			{
				WindowInfo* self = (WindowInfo*)(glfwGetWindowUserPointer(window));

				Event e{};
				e.code = ConvertGLFWKeyCodeToEventCode(key);
				e.type = ConvertGLFWKeyActionToEventType(action);

				self->AddEvent(e);
			});

	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Handle);
	}
	void Window::Update()
	{
		glfwPollEvents();

		m_WindowInfo.DispatchEvents();
	}
	void Window::AddEventListener(const EventListener& listener)
	{
		m_WindowInfo.AddEventListener(listener);
	}
	EventCode ConvertGLFWKeyCodeToEventCode(int code)
	{
		return (EventCode)(code - 53);
	}
	EventType ConvertGLFWKeyActionToEventType(int action)
	{
		EventType type{};

		switch (action)
		{
		case GLFW_PRESS:				type = EventType::KeyPress;     break;
		case GLFW_RELEASE:				type = EventType::KeyRelease;   break;
		case GLFW_PRESS | GLFW_REPEAT:	type = EventType::KeyDown;		break;
		default: break;
		}

		return type;
	}
}