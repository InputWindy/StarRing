#pragma once
#include "Core.h"
#include <string>
#include "Texture.h"
#include "VertexArray.h"

using namespace std;
namespace StarRing {

	DECLARE_REF(SkyBox)
	DECLARE_SCOPE(SkyBox)
	class SR_API SkyBox
	{
		friend class Renderer;
	public:
		SkyBox() = default;
		SkyBox(vector<string>& filepath);
		~SkyBox() = default;

		static SkyBoxRef Create(vector<string>& filepath) {
			return make_shared<SkyBox>(filepath);
		}

		static SkyBoxRef GetDefault() {
			SkyBoxRef ret = make_shared<SkyBox>();
			ret->m_SkyBoxShader = Shader::GetSkyBox();
			ret->m_SkyBoxTexture = TextureCubic::GetDefaultSkyBoxTexture();
			ret->m_SkyBoxVAO = VertexArray::GetSkyBoxVAO();
			return ret;
		}

	private:
		TextureCubicRef m_SkyBoxTexture;
		VertexArrayRef m_SkyBoxVAO;
		ShaderRef m_SkyBoxShader;
	};
}

