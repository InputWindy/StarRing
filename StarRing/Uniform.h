#pragma once
#include "Core.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
using namespace std;
using namespace glm;
namespace StarRing {
    DECLARE_REF(Shader)
    DECLARE_SCOPE(Shader)
    enum  {
        UNIFORM_INT,
        UNIFORM_FLOAT, UNIFORM_FLOAT2, UNIFORM_FLOAT3, UNIFORM_FLOAT4,
        UNIFORM_MAT3, UNIFORM_MAT4
    };

    struct SR_API UniformElement
    {
    public:
        UniformElement() = default;
        UniformElement(const string& nameineditor, const string& nameinshader, unsigned int uniformtype, void* data)
            :_nameInEditor(nameineditor), _nameInShader(nameinshader), _m_UniformType(uniformtype), _data(data)
        {};
        ~UniformElement() {
            delete _data;
            _data = nullptr;
        }
        unsigned int _m_UniformType ;

        string _nameInShader;
        string _nameInEditor;

        void* _data;

        template<typename _T>
        _T GetData() {
            return (_T)(*(((char*)_data)));
        }
    };

    class SR_API UniformLayout
    {
    public:
        UniformLayout() = default;
        ~UniformLayout() = default;

    public:
        UniformLayout(const initializer_list<UniformElement>& uniforms) :m_Uniforms(uniforms) {};
    public:
        vector<UniformElement> m_Uniforms;
    };

    //Uniform维护的是外部变量，内置变量在Renderer内部传递
    DECLARE_REF(Uniform)
    DECLARE_SCOPE(Uniform)
    class SR_API Uniform
    {
        friend class Material;
        friend class MeshRendererComponent;
        friend class Renderer;
    public:
        Uniform() = default;
        Uniform(const UniformLayout& layout) :m_Layout(layout) {};
        ~Uniform() = default;

        void SetLayout(const UniformLayout& layout) {
            m_Layout = layout;
        };
        static UniformRef Create(const UniformLayout& layout) {
            return make_shared<Uniform>(layout);
        };

        //读取默认Shader的参数
        static UniformRef GetDefault() { 
            return make_shared<Uniform>();
        };

        template<typename _T>
        void SetDataByName(const string& nameineditor,void* data) {
            for (auto& uniform : m_Layout.m_Uniforms) {
                if (uniform._nameInEditor == nameineditor) {
                    uniform._data = data;
                    return;
                }
            }
        }
    private:
        void SetUniforms(ShaderRef shader);
    private:
        UniformLayout m_Layout;
    };

}
