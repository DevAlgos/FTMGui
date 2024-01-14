#pragma once

#include "Layer.h"

#include <vector>
#include <memory>

namespace FTMGui {

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack() = default;

		void Push(std::shared_ptr<Layer> layer);

		void Pop(size_t index);
		void Pop();

		void UpdateLayers();

	private:
		std::vector<std::shared_ptr<Layer>> m_LayerStack;
	};
}