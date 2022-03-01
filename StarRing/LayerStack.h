#pragma once
#include "Layer.h"
#include <vector>
namespace StarRing {

	//��ʵ��һ��˫�����
	class SR_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
		//��ͷ���
		void PushLayer(Layer* layer);		
		//��β���
		void PushOverlay(Layer* overlay);	
		//Popͷ�ڵ�
		void PopLayer(Layer* layer);		
		//Popβ�ڵ�
		void PopOverlay(Layer* overlay);	


		vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		//ά��Layer��˫�����
		vector<Layer*> m_Layers;

		//ʼ��ָ�����
		vector<Layer*>::iterator m_LayerInsert;
	};
}

