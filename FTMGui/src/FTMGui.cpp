#include "FTMGui.h"


namespace FTMGui {

	FTMGuiContext::FTMGuiContext(Platform platform, const WindowInfo& info)
		: m_MainWindow(info), m_Platform(platform)
	{
	}

	FTMGuiContext::~FTMGuiContext()
	{
		glfwTerminate();
	}

	ContextType FTMGuiContext::CreateContext(Platform platform, const WindowInfo& info)
	{
		return std::make_shared<FTMGuiContext>(platform, info);
	}

	void FTMGuiContext::UpdateCtx()
	{
		m_MainWindow.Update();
	}

}