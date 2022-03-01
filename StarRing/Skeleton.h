#pragma once
#include "Bone.h"
namespace StarRing {
	//����ά������vector<Bone>,map<ID,Bone_Name>���ձ�
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