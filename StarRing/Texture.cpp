#include "pch.h"
#include "Texture.h"
#include "Renderer.h"
#include "OpenGLTexture.h"
const std::vector<string> DefaultSkyBox = {
	"C:/Users/InputWindy/Desktop/StarRing/StarRing/bin/Debug-x64/Editor/assets/EngineDefault/textures/skybox/right.jpg",
	"C:/Users/InputWindy/Desktop/StarRing/StarRing/bin/Debug-x64/Editor/assets/EngineDefault/textures/skybox/left.jpg",
	"C:/Users/InputWindy/Desktop/StarRing/StarRing/bin/Debug-x64/Editor/assets/EngineDefault/textures/skybox/top.jpg",
	"C:/Users/InputWindy/Desktop/StarRing/StarRing/bin/Debug-x64/Editor/assets/EngineDefault/textures/skybox/bottom.jpg",
	"C:/Users/InputWindy/Desktop/StarRing/StarRing/bin/Debug-x64/Editor/assets/EngineDefault/textures/skybox/front.jpg",
	"C:/Users/InputWindy/Desktop/StarRing/StarRing/bin/Debug-x64/Editor/assets/EngineDefault/textures/skybox/Back.jpg"
};
namespace StarRing {
	Texture2DRef Texture2D::Create(const string path)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:break;
		case RendererAPI::API::OpenGL: return make_shared<OpenGLTexture2D>(path);
		default:
			break;
		}
	}
	TextureCubicRef TextureCubic::Create(vector<string>& path)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:break;
		case RendererAPI::API::OpenGL: return make_shared<OpenGLTextureCubic>(path);
		default:
			break;
		}
	}
	TextureCubicRef TextureCubic::GetDefaultSkyBoxTexture()
	{
		
		switch (Renderer::GetRenderAPI())
		{
			case RendererAPI::API::None:break;
			case RendererAPI::API::OpenGL: return make_shared<OpenGLTextureCubic>(DefaultSkyBox);
			default:
				break;
		}
	}
}