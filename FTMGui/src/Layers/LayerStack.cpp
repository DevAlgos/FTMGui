#include "LayerStack.h"

#include "Utils/Log.h"

namespace FTMGui {
	
	void LayerStack::Push(std::shared_ptr<Layer> layer)
	{
		m_LayerStack.push_back(layer);
		m_LayerStack.back()->OnAttach();
	}
	void LayerStack::Pop(size_t index)
	{	
		FTMGUI_ASSERT(index < m_LayerStack.size(), "index out of range");

		m_LayerStack[index]->OnDettach();
		m_LayerStack.erase(m_LayerStack.begin() + index);
	}

	void LayerStack::Pop()
	{
		FTMGUI_ASSERT(m_LayerStack.size() != 0, "layer stack is empty");

		m_LayerStack[m_LayerStack.size() - 1]->OnDettach();
		m_LayerStack.pop_back();
	}
	void LayerStack::UpdateLayers()
	{
		for (auto& layer : m_LayerStack)
			layer->OnUpdate();
	}
}