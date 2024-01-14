#pragma once

/*
* TODO: Implement layer system in order to control
* order of rendering and updating so we can control which
* label/gui item is above or below others.
*/

namespace FTMGui {

	class Layer
	{
	public:
		virtual void OnAttach() {};
		virtual void OnUpdate() {};
		virtual void OnDettach() {};
	};

}