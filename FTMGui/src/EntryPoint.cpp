#include <iostream>

#include "FTMGui.h"

int main()
{
	FTMGui::AppDescriptor Desc;
	Desc.appName = "app";
	Desc.platform = FTMGui::Platform::Windows;
	Desc.windowWidth = 600;
	Desc.windowHeight = 600;

	FTMGui::Init(Desc);

	FTMGui::Run();

	FTMGui::Shutdown();
}