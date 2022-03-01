#version 330 core
//Mesh�������ݣ�ģ�Ϳռ�Pos,nor,uv,tan,bitan,bone_id,bone_weight��
layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBiTangent;

layout (location = 4) in ivec4 aBoneIDs;
layout (location = 5) in vec4 aBoneWeights;

layout (location = 10) in vec2 TexCoords0;
	
//�������ñ��������й����ı任����ʵʱ���㣩
const int MAX_BONE_NUM = 200;
uniform mat4 gBonesTransform[MAX_BONE_NUM];//�����ı任����T_POSE�£��þ��󲻻�Զ���λ�����޸�

uniform mat4 model;     //ģ�Ϳռ䵽����ռ�
uniform mat4 view;      //����ռ䵽����ռ�
uniform mat4 projection;//����ռ䵽�ü��ռ�(gl_Positionר��)

out VS_OUT{
    vec3 vs_position;//����ռ�����
    
    vec2 uv;         //uv
    
    mat3 TBN;        //TBN����(���߿ռ䵽ģ�Ϳռ�)
}vs_out;

void main()
{
    mat4 BoneTransform = gBonesTransform[aBoneIDs[0]] * aBoneWeights[0];
    BoneTransform += gBonesTransform[aBoneIDs[1]] * aBoneWeights[1];
    BoneTransform += gBonesTransform[aBoneIDs[2]] * aBoneWeights[2];
    BoneTransform += gBonesTransform[aBoneIDs[3]] * aBoneWeights[3];
    vs_out.uv = TexCoords0;

    vec4 worldTangent = model * vec4(aTangent, 1.0);
    vec3 _worldTangent = vec3(worldTangent.xyz);
    vec4 worldBiTangent = model * vec4(aBiTangent, 1.0);
    vec3 _worldBiTangent = vec3(worldBiTangent.xyz);
    vec4 worldNormal = model * vec4(aNormal, 1.0);
    vec3 _worldNormal = vec3(worldNormal.xyz);
    
    vec4 worldPos = model * vec4(aPos, 1.0);
    vs_out.vs_position =vec3(worldPos.xyz); //����ռ�����

    vs_out.TBN = mat3(_worldTangent,_worldBiTangent , _worldNormal);

    //�������������gl_Position = projection * view *  model *BoneTransform* vec4(aPos, 1.0);
    gl_Position = projection * view *model * vec4(aPos, 1.0);
}