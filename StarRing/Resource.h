#pragma once
#include <string>
#include "Core.h"
using namespace std;
namespace StarRing {
	enum class ResourceType
	{
		Mesh = 0,Material = 1,Texture = 2,Scene = 3
	};

	//Mesh/Material/Texture/C#都是外部资源，用智能指针维护，保存在内存管理器的队列里，并且维护一个声明周期
	//Entity是场景内的实际物体，Component是资源与Entity之间的接口，是挂载资源的Slot
	//MeshComponent维护一个VertexArray的Ref
	//MeshRendererComponent维护一个Draw()方法，和一个Material
	//SkeletonComponent维护一个Skeleton的Ref
	//AnimatorComponent维护一个vector<Animation>的Ref
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

