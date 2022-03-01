#pragma once
#include "Layer.h"
#include <vector>
namespace StarRing {

	//其实是一个双向队列
	class SR_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
		//从头入队
		void PushLayer(Layer* layer);		
		//从尾入队
		void PushOverlay(Layer* overlay);	
		//Pop头节点
		void PopLayer(Layer* layer);		
		//Pop尾节点
		void PopOverlay(Layer* overlay);	


		vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		//维护Layer的双向队列
		vector<Layer*> m_Layers;

		//始终指向队首
		vector<Layer*>::iterator m_LayerInsert;
	};
}

