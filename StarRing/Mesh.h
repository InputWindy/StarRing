#pragma once
#include "Core.h"
#include "VertexArray.h"
using namespace std;
namespace StarRing {
	class Entity;
	DECLARE_REF(Mesh)
	DECLARE_SCOPE(Mesh)
	class SR_API Mesh
	{
		friend class MeshRendererComponent;
		friend class ResourceManager;
		friend class SceneHierarchyPanel;
		friend class MeshRendererComponent;
	public:
		Mesh() = default;
		Mesh(const string& name, VertexArrayRef vao, Entity* owner) 
			:m_VertexArray(vao), m_Owner(owner),m_MeshName(name)
		{};
		~Mesh() = default;

		//创建一个Mesh
		static MeshRef Create(const string& name, VertexArrayRef vao = nullptr, Entity*owner = nullptr);
	protected:
		VertexArrayRef m_VertexArray = nullptr;
		string	m_MeshName;
		Entity* m_Owner = nullptr;
	};
}

