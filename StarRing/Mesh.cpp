#include "pch.h"
#include "Mesh.h"
#include "ResourceManager.h"
namespace StarRing {
	MeshRef Mesh::Create(const string& name, VertexArrayRef vao, Entity* owner)
	{
		return make_shared<Mesh>(name, vao,owner);
	}
}