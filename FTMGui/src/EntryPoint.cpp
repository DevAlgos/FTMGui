#include <iostream>

#include "FTMGui.h"

#include "Utils/Log.h"

using namespace FTMGui;

int main()
{	
	/*FTMGui::AppDescriptor Desc;
	Desc.appName = "app";
	Desc.platform = FTMGui::Platform::Windows;
	Desc.windowWidth = 600;
	Desc.windowHeight = 600;

	WindowInfo info;
	info.width = 1920;
	info.height = 1080;
	info.title = "wassup";
	info.vSync = false;
	info.windowClose = false;

	ContextType ctx = FTMGuiContext::CreateContext(Platform::Windows, info);

	while (ctx->IsRunning())
	{
		ctx->UpdateCtx();
	}*/

	FTMGui::Log::Init();
	bool condition = true;
	FTMGUI_ASSERT(condition, "hello");

	FTMGUI_LOG_DEBUG("msg ", 500, " no but you", " yes");
}