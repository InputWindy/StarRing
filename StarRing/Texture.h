#pragma once
#include "Core.h"
#include "Resource.h"
using namespace std;
namespace StarRing {
	class SR_API Texture:public Resource
	{
	public:
		virtual ~Texture() {};

		virtual uint32_t GetWidth()const = 0;
		virtual uint32_t GetHeight()const = 0;

		virtual uint32_t GetHandle()const = 0;

		virtual void Bind(uint32_t tex_unit = 0)const  = 0;
		virtual void UnBind(uint32_t tex_unit = 0)const = 0;

	};

	DECLARE_REF(Texture2D)
	DECLARE_SCOPE(Texture2D)
	class SR_API Texture2D:public Texture
	{
	public:
		virtual ~Texture2D() {};
		static Texture2DRef Create(const string path);
	
	};

	DECLARE_REF(TextureCubic)
	DECLARE_SCOPE(TextureCubic)
	class SR_API TextureCubic :public Texture
	{
	public:
		virtual ~TextureCubic() {};
		static TextureCubicRef Create(vector<string>& path);

		static TextureCubicRef GetDefaultSkyBoxTexture();
	};
	
}

