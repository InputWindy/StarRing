#pragma once
#include "Texture.h"
namespace StarRing {
	class OpenGLTexture2D:public Texture2D
	{
	public:
		OpenGLTexture2D(string path);
		virtual~OpenGLTexture2D();

		virtual uint32_t GetHandle()const override;

		virtual uint32_t GetWidth()const override { return m_Width; };
		virtual uint32_t GetHeight()const override { return m_Height; };

		virtual void Bind(uint32_t tex_unit = 0)const override;		
		virtual void UnBind(uint32_t tex_unit = 0)const override;

	protected:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		string m_Path;
	};

	class OpenGLTextureCubic :public TextureCubic
	{
	public:
		OpenGLTextureCubic() = default;
		OpenGLTextureCubic(const vector<string>& path);
		virtual~OpenGLTextureCubic();

		virtual uint32_t GetHandle()const override;

		virtual uint32_t GetWidth()const override { return m_Width; };
		virtual uint32_t GetHeight()const override { return m_Height; };

		virtual void Bind(uint32_t tex_unit = 0)const override;
		virtual void UnBind(uint32_t tex_unit = 0)const override;


	protected:
		uint32_t m_RendererID;
		int m_Width, m_Height;
		vector<string> m_Path;
	};
}

