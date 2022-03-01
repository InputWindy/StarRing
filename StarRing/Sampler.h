#pragma once
#include "Texture.h"
namespace StarRing {
    DECLARE_REF(Shader)
    DECLARE_SCOPE(Shader)

    struct SR_API Sampler2DElement
    {
        friend class Renderer;
        friend class Sampler2D;
    public:
        Sampler2DElement() = default;
        Sampler2DElement(const string& nameineditor, const string& nameinshader, const string& filePath,unsigned int texUnit = 0)
            :_nameInEditor(nameineditor), _nameInShader(nameinshader), _filePath(filePath), _texUnit(texUnit)
        {
            _Texture = Texture2D::Create(_filePath);
        };
        ~Sampler2DElement() {

        }

        string _filePath;

        string _nameInShader;
        string _nameInEditor;

        unsigned int _texUnit ;
    private:
        Texture2DRef _Texture;

    };

    class SR_API Sampler2DLayout
    {
    public:
        Sampler2DLayout() = default;
        ~Sampler2DLayout() = default;

    public:
        Sampler2DLayout(const initializer_list<Sampler2DElement>& Sampler2Ds) :m_Sampler2Ds(Sampler2Ds) {};
        vector<Sampler2DElement> m_Sampler2Ds;
    };

	DECLARE_REF(Sampler2D)
	DECLARE_SCOPE(Sampler2D)
	/// <summary>
	/// 在着色器绑定之后，先bind纹理，再setInt()
    /// 需要维护：一张2D纹理/纹理单元
	/// </summary>
	class SR_API Sampler2D
	{
		friend class Material;
		friend class MeshRendererComponent;
		friend class Renderer;
	public:
		Sampler2D() = default;
		Sampler2D(const Sampler2DLayout& layout) :m_Layout(layout) {};
		~Sampler2D() = default;

		void SetLayout(const Sampler2DLayout& layout) {
			m_Layout = layout;
		};
		static Sampler2DRef Create(const Sampler2DLayout& layout) {
			return make_shared<Sampler2D>(layout);
		};
        //读取默认Shader的贴图
        static Sampler2DRef GetDefault() {
            Sampler2DLayout layout = { 
                {"diffuse","diffuse_map", "C:/Users/InputWindy/Desktop/StarRing/StarRing/Editor/assets/model/gun/gun/Textures/Cerberus_A.tga",0},
                {"normal","normal_map","C:/Users/InputWindy/Desktop/StarRing/StarRing/Editor/assets/model/gun/gun/Textures/Cerberus_N.tga",1},
                {"metallic","metallic_map","C:/Users/InputWindy/Desktop/StarRing/StarRing/Editor/assets/model/gun/gun/Textures/Cerberus_M.tga",2},
                {"ao","ao_map","C:/Users/InputWindy/Desktop/StarRing/StarRing/Editor/assets/model/gun/gun/Textures/Raw/Cerberus_AO.tga",3},
                {"roughness","roughness_map","C:/Users/InputWindy/Desktop/StarRing/StarRing/Editor/assets/model/gun/gun/Textures/Cerberus_R.tga",4}
            };
            return make_shared<Sampler2D>(layout);
        };
	private:
        void SetSampler2D(ShaderRef shader);
    private:
        Sampler2DLayout m_Layout;
	};
}

