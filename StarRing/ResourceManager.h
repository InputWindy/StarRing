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
/// ����ģ�͵���ȷ����(Prefab)��
///		���Root�ڵ����־ͽ�rootNode������ģ�͸��ڵ㣬ģ�͸��ڵ���rootNode����ĵ�һ��node
///		��������Scene�������node��һ��Node����һ��Entity����ÿ���ڵ㶼����һ��Entity.
///		��Mesh��node����Ҫ����node�������Mesh���ϲ���һ��Mesh�����־���node�����֡�
///		ÿ��node��ά��һ��Transform����Entity���ӽṹ�γ��˹���ģ�ͣ�����ÿһ��Entity��Ҫά��һ��Transform����
///		
/// </summary>
	
namespace StarRing {
	/// <summary>
	/// ��map��ά�����е����浼����Դ��file_path##name�Ǽ�
	/// ResourceManagerʵ������������Դ�ľ�̬����ӿڣ�
	/// �����ȡ�ⲿ��Դʱ��ͳһ��ResourceManager��ȡ�������Դδ���أ��Ϳ����첽Load
	/// </summary>
	class SR_API ResourceManager
	{
		friend class VertexArray;
		friend class EditorLayer;
	public:
		ResourceManager() = default;
		~ResourceManager() = default;

		//��ҪCloneһ���µ�Prefab�����ܰ���Դ���������ֱ���ͳ�ȥ
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
		//ResourceManager������нӿڶ���static��

		//Ԥ�����Key��·��
		static inline map<string, EntityRef>			m_PrefabResource;

		//Mesh��Key������
		static inline map<string, MeshRef>				m_MeshResource;

		//Material��Key��vShader·��
		static inline map<string, MaterialRef>			m_MaterialResource;

	};
}

