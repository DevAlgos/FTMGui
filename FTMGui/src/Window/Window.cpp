#include "Window.h"

namespace FTMGui {
	
	Window::Window(const WindowInfo& info)
		: m_Handle(nullptr), m_WindowInfo(info)
	{

		EventListener listner;
		listner.OnEvent = [](Event& e) {printf("%i", e.code); e.handled = true; };
		m_WindowInfo.AddEventListener(listner);

		if (!glfwInit())
			__debugbreak();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Handle = glfwCreateWindow(m_WindowInfo.width, m_WindowInfo.height, m_WindowInfo.title.data(), nullptr, nullptr);

		/*window creation failure*/
		if (!m_Handle)
			__debugbreak();

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

				/*
				* TODO: will need to add a proper viewport system so this will be added later
				* this will be important for drawing to multiple windows/sections of windows
				*/
				//glViewport();
			});

		glfwSetKeyCallback(m_Handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
			{
				WindowInfo* self = (WindowInfo*)(glfwGetWindowUserPointer(window));

				Event e{};
				e.code = ConvertGLFWKeyCodeToEventCode(key);
				e.type = ConvertGLFWKeyActionToEventType(action);

				self->AddEvent(e);
			});

		glfwMakeContextCurrent(m_Handle);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Handle);
	}
	void Window::Update()
	{
		glfwPollEvents();

		m_WindowInfo.DispatchEvents();

		glfwSwapBuffers(m_Handle);
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