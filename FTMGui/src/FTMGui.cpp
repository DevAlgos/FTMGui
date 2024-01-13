#include "FTMGui.h"

#include <memory>

#include "MainWindow/MainWindow.h"


namespace FTMGui {

	FTMGuiGlobals* s_FTMGuiGlobals = nullptr;

	void Init(const AppDescriptor& App)
	{
		if (!s_FTMGuiGlobals)
		{
			s_FTMGuiGlobals = (FTMGuiGlobals*)malloc(sizeof(FTMGuiGlobals));
			

			if (!s_FTMGuiGlobals) 
				__debugbreak();

			s_FTMGuiGlobals->windowClosed = false;

			s_FTMGuiGlobals->currentTitle = App.appName;
			s_FTMGuiGlobals->viewportWidth = App.windowWidth;
			s_FTMGuiGlobals->viewportHeight = App.windowHeight;

			s_FTMGuiGlobals->mainWindow = CreateMainWindow();
		}
	}
	void Run()
	{
		while (!s_FTMGuiGlobals->windowClosed)
		{
			Update();
		}
	}
	void Update()
	{
		UpdateWindow(s_FTMGuiGlobals->mainWindow);
	}
	void Shutdown()
	{
		if (s_FTMGuiGlobals)
		{
			DestroyWindow(s_FTMGuiGlobals->mainWindow);
			free(s_FTMGuiGlobals);
		}
	}
}