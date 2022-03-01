#include "pch.h"
#include "SkyBox.h"

namespace StarRing {
    SkyBox::SkyBox(vector<string>& filepath)
    {
        m_SkyBoxShader = Shader::GetSkyBox();
        m_SkyBoxVAO = VertexArray::GetSkyBoxVAO();
        m_SkyBoxTexture = TextureCubic::Create(filepath);
    }
}