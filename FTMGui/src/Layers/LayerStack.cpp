#include "LayerStack.h"

#include <cassert>

namespace FTMGui {
	
	void LayerStack::Push(std::shared_ptr<Layer> layer)
	{
		layer->OnAttach();
		m_LayerStack.push_back(layer);
	}
	void LayerStack::Pop(size_t index)
	{	
		assert(index >= m_LayerStack.size());

		m_LayerStack[index]->OnDettach();
		m_LayerStack.erase(m_LayerStack.begin() + index);
	}

	void LayerStack::Pop()
	{
		assert(m_LayerStack.size() == 0);

		m_LayerStack[m_LayerStack.size() - 1]->OnDettach();
		m_LayerStack.pop_back();
	}
	void LayerStack::UpdateLayers()
	{
		for (auto& layer : m_LayerStack)
			layer->OnUpdate();
	}
}