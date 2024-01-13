#pragma once
#include <cinttypes>

namespace FTMGui {

	enum class Platform { None = 0, Windows, Mac };

	struct FTMWindow;

	struct FTMGuiGlobals
	{
		FTMWindow* mainWindow;

		bool windowClosed = false;
		uint32_t viewportWidth = 0;
		uint32_t viewportHeight = 0;

		const char* currentTitle = "";
	};

	extern FTMGuiGlobals* s_FTMGuiGlobals;

	struct AppDescriptor
	{
		Platform platform = Platform::Windows; //by default

		uint32_t windowWidth = 0;
		uint32_t windowHeight = 0;

		const char* appName = "App";
	};

	void Init(const AppDescriptor& App);

	/*
	* For testing purposes, will be used to simulate client
	*/
	void Run();

	/*
	* Swaps buffers and re renderes every frame
	*/

	void Update();

	/*
	* Cleans up memory
	*/
	void Shutdown();

}