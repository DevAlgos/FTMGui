#include "MainWindow.h"

#include <malloc.h>

namespace FTMGui {
	FTMWindow* CreateMainWindow()
	{
		if (!s_FTMGuiGlobals)
			__debugbreak();

		FTMWindow* window = (FTMWindow*)malloc(sizeof(FTMWindow));

		if (!window) /*malloc failure*/
			return nullptr;

		/*
		* we may want to do this somewhere else but because this is the main window this should be fine
		*/

		if (!glfwInit()) 
			return nullptr; 

		/*
		* tells glfw to use latest opengl core version
		* this will change once we include metal where we will include the latest version of metal
		* if user is on mac os
		*/

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		

		window->windowHandle = glfwCreateWindow(s_FTMGuiGlobals->viewportWidth, 
											    s_FTMGuiGlobals->viewportHeight, 
												s_FTMGuiGlobals->currentTitle, 
											    nullptr, nullptr);

		/*window creation failure*/
		if (!window->windowHandle)
			return nullptr;

		/*
		* lets glfw access data from s_FTMGuiGlobals, this is nice to have
		* so we can access our viewport from within callback functions
		*/
		glfwSetWindowUserPointer(window->windowHandle, s_FTMGuiGlobals);
		
		glfwSetWindowCloseCallback(window->windowHandle, [](GLFWwindow* window)
			{
				FTMGuiGlobals* self = (FTMGuiGlobals*)(glfwGetWindowUserPointer(window));
				self->windowClosed = true;
			});

		glfwSetFramebufferSizeCallback(window->windowHandle, [](GLFWwindow* window, int width, int height)
			{
				FTMGuiGlobals* self = (FTMGuiGlobals*)(glfwGetWindowUserPointer(window));

				self->viewportWidth = width;
				self->viewportHeight = height;

				/*
				* TODO: will need to add a proper viewport system so this will be added later
				* this will be important for drawing to multiple windows/sections of windows
				*/
				//glViewport();
			});

		glfwMakeContextCurrent(window->windowHandle);

		return window;
	}
	void UpdateWindow(FTMWindow* window)
	{
		glfwPollEvents();
		glfwSwapBuffers(window->windowHandle);
	}
	void DestroyWindow(FTMWindow* window)
	{
		if (window)
		{
			glfwDestroyWindow(window->windowHandle);
			glfwTerminate(); //if we decide to have multiple windows we may not want to terminate glfw

			free(window);
		}
	}
}