#version 330 core
//����
const float PI = 3.1415926535;
const int MAX_POINT_LIGHT_NUM = 4;
const int MAX_SPOT_LIGHT_NUM = 4;
const int MAX_AREA_LIGHT_NUM = 4;

out vec4 FragColor;

struct DirectionalLight{
    vec3 Color;
    vec3 Direction;
};

struct PointLight{
    vec3 Color;
    vec3 Position;
    float CutOff;
};

struct SpotLight{
    vec3 Color;
    vec3 Position;
    vec3 Direction;
    float Angle;
    float CutOff;
};

struct AreaLight{
    vec3 Color;
    vec3 Position;
    vec3 Normal;
    float width;
    float height;
    float CutOff;
};

struct Camera{
    vec3 Position;
};

in VS_OUT{
    vec3 vs_position;//����ռ�����
    
    vec2 uv;         //uv
    
    mat3 TBN;        //TBN����
}fs_in;

//��ʱʹ�õ����ñ�����֮����Ҫ�滻�ɹ�Դ�����
uniform vec3 lightPos;
uniform vec3 lightColor;

//���ñ��������һ��̫�������4�����ݣ����4���ֵ�Ͳ�����4������ƽ��,һ̨�������
uniform DirectionalLight sunLight;
uniform int PointLightNum;//�ƹ�����
uniform PointLight pointLights[MAX_POINT_LIGHT_NUM];
uniform int SpotLightNum;//�ֵ�Ͳ����
uniform SpotLight spotLights[MAX_SPOT_LIGHT_NUM];
uniform int AreaLightNum;//�ֵ�Ͳ����
uniform AreaLight AreaLights[MAX_AREA_LIGHT_NUM];
uniform Camera camera;

uniform samplerCube environment_map;//�������գ��ɷ���̽�����պ��ṩ��

//�ֶ�����Ĳ�����PBR����
uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D ao_map;
uniform sampler2D roughness_map;

//H:���������roughness���ֲڶȡ�N:���㷨�ߡ���NDF����
float D_GGX_TR(vec3 N, vec3 H, float roughness)
{
    float a2     = roughness*roughness;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

//���κ���G������΢����ģ�����ڵ�����   k�Ǵֲڶȵ���ӳ��   N������   V:����  L:����
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

//��������
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

//�����Դǿ��˥����ƽ�����ȣ�
vec3 CalRadiance(vec3 light_color,float _distance)
{
    float attenuation = 1.0 / (_distance * _distance);
    return light_color*attenuation;
}

//���¼���ȫ��������ռ���
void main()
{   
    //����
    vec3 normal = texture(normal_map, fs_in.uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(fs_in.TBN * normal);
    //������ǿ��
    vec3 irradiance = texture(environment_map,normal).rgb;
    //������(sRGB)
    vec3  albedo =pow(texture(diffuse_map, fs_in.uv).rgb,vec3(2.2));
    //�����ȣ����Կռ䣩
    float metallic = texture(metallic_map, fs_in.uv).r;
    //�ֲڶȣ����Կռ䣩
    float roughness = texture(roughness_map, fs_in.uv).r;
    //ao�����Կռ䣩
    float ao = texture(ao_map, fs_in.uv).r;
    // ����ⷽ��ָ���Դ��
    vec3 lightDir = normalize(lightPos - fs_in.vs_position);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);//��Ӱbias����ʱ���ã�
    //���߷���
    vec3 viewDir = normalize(camera.Position - fs_in.vs_position);
    //���䷽��
    vec3 reflectDir = reflect(-lightDir, normal);
    //�������
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    //====================���½���pbr���ռ���===============//
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 Lo = vec3(0.0);

    vec3 F  = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);
    float NDF = D_GGX_TR(normal, halfwayDir, roughness);       
    float G   = GeometrySmith(normal, viewDir, lightDir, roughness);  
    vec3  nominator    = NDF * G * F;
    float denominator  = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001; 
    vec3  specular     = nominator / denominator; 

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic; 

    float NdotL = max(dot(normal, lightDir), 0.0);  

    vec3 fr = ( kD*albedo/PI + kS*specular);
    //Loֻ�����й�Դ������ֱ�ӹ��ա�
    //��ӹ���Ҫ��irradiance�����ؿ������
    //
    Lo += fr * lightColor * NdotL;
    
    FragColor   = vec4( Lo*5 ,1.0);
    //====================���½���gammaУ��===============//
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/2.2));//��ɫ�޸�ΪsRGB�ռ�
}