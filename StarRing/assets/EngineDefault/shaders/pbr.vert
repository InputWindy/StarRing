#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBiTangent;
layout (location = 4) in vec2 aTexCoord;
	
uniform mat4 model;       //ģ�Ϳռ䵽����ռ�
uniform mat4 view;        //����ռ䵽����ռ�
uniform mat4 projection;  //gl_Positionר��

out VS_OUT{
    vec3 vs_position;//����ռ�����
    
    vec2 uv;         //uv
    
    mat3 TBN;        //TBN����(���߿ռ䵽ģ�Ϳռ�)
}vs_out;

void main()
{
    vs_out.uv = aTexCoord;

    vec4 worldTangent = model * vec4(aTangent, 1.0);
    vec3 _worldTangent = vec3(worldTangent.xyz);
    vec4 worldBiTangent = model * vec4(aBiTangent, 1.0);
    vec3 _worldBiTangent = vec3(worldBiTangent.xyz);
    vec4 worldNormal = model * vec4(aNormal, 1.0);
    vec3 _worldNormal = vec3(worldNormal.xyz);
    
    vec4 worldPos = model * vec4(aPosition, 1.0);
    vs_out.vs_position =vec3(worldPos.xyz); //����ռ�����

    vs_out.TBN = mat3(_worldTangent,_worldBiTangent , _worldNormal);

    gl_Position = projection*view *model * vec4(aPosition, 1.0);
}