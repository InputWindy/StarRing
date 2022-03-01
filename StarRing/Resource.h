#pragma once
#include <string>
#include "Core.h"
using namespace std;
namespace StarRing {
	enum class ResourceType
	{
		Mesh = 0,Material = 1,Texture = 2,Scene = 3
	};

	//Mesh/Material/Texture/C#�����ⲿ��Դ��������ָ��ά�����������ڴ�������Ķ��������ά��һ����������
	//Entity�ǳ����ڵ�ʵ�����壬Component����Դ��Entity֮��Ľӿڣ��ǹ�����Դ��Slot
	//MeshComponentά��һ��VertexArray��Ref
	//MeshRendererComponentά��һ��Draw()��������һ��Material
	//SkeletonComponentά��һ��Skeleton��Ref
	//AnimatorComponentά��һ��vector<Animation>��Ref
	class SR_API Resource
	{
	public:
		Resource() = default;
		~Resource() = default;

		virtual const string& GetFilePath()const;
		virtual const string& GetResourceName()const;
		virtual void SetResourceName(const string& name);
		virtual void SetFilePath(const string& filepath);
	protected:
		string m_Name;
		string file_path;
	};
}

