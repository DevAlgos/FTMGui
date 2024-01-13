#pragma once
#include <GLFW/glfw3.h>

#include "FTMGui.h"

namespace FTMGui {

	struct FTMWindow
	{
		GLFWwindow* windowHandle = nullptr;
	};

	FTMWindow* CreateMainWindow();

	void UpdateWindow(FTMWindow* window);

	void DestroyWindow(FTMWindow* window);
}