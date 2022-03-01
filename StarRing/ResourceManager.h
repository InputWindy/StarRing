#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Entity.h"
#include <map>
#include <string>
#include <../assimp/Importer.hpp>
#include <../assimp/scene.h>
#include <../assimp/postprocess.h>
using namespace std;
/// <summary>
/// 导入模型的正确姿势(Prefab)：
///		如果Root节点名字就叫rootNode，则不是模型根节点，模型根节点是rootNode下面的第一个node
///		遍历整个Scene里的所有node，一个Node就是一个Entity，给每个节点都生成一个Entity.
///		有Mesh的node，需要解析node里的所有Mesh，合并成一个Mesh，名字就是node的名字。
///		每个node还维护一个Transform矩阵，Entity父子结构形成了骨骼模型，所以每一个Entity需要维护一个Transform矩阵
///		
/// </summary>
	
namespace StarRing {
	/// <summary>
	/// 用map来维护所有的引擎导入资源，file_path##name是键
	/// ResourceManager实现所有类型资源的静态导入接口，
	/// 引擎读取外部资源时，统一从ResourceManager读取，如果资源未加载，就开启异步Load
	/// </summary>
	class SR_API ResourceManager
	{
		friend class VertexArray;
		friend class EditorLayer;
	public:
		ResourceManager() = default;
		~ResourceManager() = default;

		//需要Clone一个新的Prefab，不能把资源管理器里的直接送出去
		static EntityRef		GetPrefab(const string& filepath);
		static MaterialRef		GetMaterial(const string& vpath, const string& fpath);

		static void				LogResource();
	private:
		static VertexArrayRef	ProcessMesh(const aiNode* node, const aiScene* scene, EntityRef& this_node);
		static void				AnalyzeNode(const aiNode* node, const aiScene* scene, EntityRef& rootNode);
		static void				LoadModel(const string& filepath);

		static void				LoadMaterial(const string& vpath, const string& fpath);

		static EntityRef		Clone(const EntityRef& entt);
		static MaterialRef		Clone(const MaterialRef& material);
	private:
		//ResourceManager里的所有接口都是static的

		//预制体的Key是路径
		static inline map<string, EntityRef>			m_PrefabResource;

		//Mesh的Key是名字
		static inline map<string, MeshRef>				m_MeshResource;

		//Material的Key是vShader路径
		static inline map<string, MaterialRef>			m_MaterialResource;

	};
}

