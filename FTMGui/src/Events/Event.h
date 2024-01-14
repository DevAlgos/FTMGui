#pragma once

/*
* Need to implement event system so we can capture and respond
* to events such as key presses or mouse buttons.
*/

namespace FTMGui {

	enum class EventType
	{
		KeyPress = 0, KeyRelease, KeyDown,KeyUp,
		MouseButtonPress, MouseButtonRelease,
		MouseButtonDown, WindowClose
	} ;

	enum class EventCode 
	{
		Key0 = 0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, Key10, 
		KeySemiColon, KeyEqual, KeyA, KeyB,KeyC,KeyD,KeyE,KeyF,KeyG,KeyH,KeyI,
		KeyJ, KeyK, KeyL, KeyM, KeyN, KeyO, KeyP, KeyQ,  KeyR, KeyS, KeyT, KeyU, 
		KeyV, KeyW, KeyX, KeyY, KeyZ, MouseLeft, MouseRight
	};

	struct Event 
	{
		EventType type;
		EventCode code;

		bool handled;

		double xPos, yPos; //for mouse events, will be initalized to -1 for  key events
	};
}