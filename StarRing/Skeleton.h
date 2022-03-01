#pragma once
#include "Bone.h"
namespace StarRing {
	//骨骼维护的是vector<Bone>,map<ID,Bone_Name>对照表
	class SR_API Skeleton
	{
	public:
		Skeleton();
		~Skeleton();

		vector<Bone> m_Bones;
	private:
		map<uint, string> m_BoneNameToID;
	};
}