#pragma once
#include "Shader.h"
namespace StarRing {

	enum class ShaderDataType
	{
		None = 0,
		Float,Float2,Float3,Float4,
		Mat3,Mat4,
		Int,Int2,Int3,Int4,
		Bool
	};

	static uint32_t GetShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
		case StarRing::ShaderDataType::None:	 return 0;
			break;								 
		case StarRing::ShaderDataType::Float:	 return 4;
			break;								 
		case StarRing::ShaderDataType::Float2:	 return 4*2;
			break;								 
		case StarRing::ShaderDataType::Float3:	 return 4*3;
			break;								 
		case StarRing::ShaderDataType::Float4:	 return 4*4;
			break;								 
		case StarRing::ShaderDataType::Mat3:	 return 4*3*3;
			break;								 
		case StarRing::ShaderDataType::Mat4:	 return 4*4*4;
			break;								 
		case StarRing::ShaderDataType::Int:		 return 4;
			break;								 
		case StarRing::ShaderDataType::Int2:	 return 4*2;
			break;								
		case StarRing::ShaderDataType::Int3:	 return 4*3;
			break;								 
		case StarRing::ShaderDataType::Int4:	 return 4*4;
			break;								 
		case StarRing::ShaderDataType::Bool:	 return 1;
			break;
		default:
			break;
		}
		return 0;
	}

	struct SR_API BufferElement
	{	
		string Name;

		ShaderDataType Type;

		uint32_t Offset;
		uint32_t Size;
		bool Normalized;
		BufferElement() {};
		BufferElement(ShaderDataType type,const string& _name,bool normalized = false) {
			Name = _name;
			Type = type;
			Offset = 0;
			Size = GetShaderDataTypeSize(type);
			Normalized = normalized;
		}

		uint32_t GetElementCount()const {
			switch (Type)
			{
			case StarRing::ShaderDataType::Float:	return 1;
			case StarRing::ShaderDataType::Float2:	return 2;
			case StarRing::ShaderDataType::Float3:	return 3;
			case StarRing::ShaderDataType::Float4:	return 4;
			case StarRing::ShaderDataType::Mat3:	return 3*3;
			case StarRing::ShaderDataType::Mat4:	return 4*4;
			case StarRing::ShaderDataType::Int:		return 1;
			case StarRing::ShaderDataType::Int2:	return 2;
			case StarRing::ShaderDataType::Int3:	return 3;
			case StarRing::ShaderDataType::Int4:	return 4;
			case StarRing::ShaderDataType::Bool:	return 1;
			default:
				break;
			}
		}
	};

	class SR_API BufferLayout
	{
	public:
		BufferLayout() {};
		BufferLayout(const std::initializer_list<BufferElement>& elements) :
			m_Elements(elements) {
			CalculateOffsetAndStride();
		};

		inline const vector<BufferElement>& GetElements() const { return m_Elements; }
	
		vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		vector<BufferElement>::iterator end() { return m_Elements.end(); }

		vector<BufferElement>::const_iterator begin()const { return m_Elements.begin(); }
		vector<BufferElement>::const_iterator end()const { return m_Elements.end(); }

		inline uint32_t GetStride()const {
			return m_Stride;
		}
	private:
		void CalculateOffsetAndStride() {
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	DECLARE_REF(VertexBuffer)
	DECLARE_SCOPE(VertexBuffer)
	class SR_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetBufferLayout()const = 0;

		static VertexBufferRef Create(float* vertices,uint32_t size);
		static VertexBufferRef GetSkyBoxVBO();
	private:

	};
	
	DECLARE_REF(IndexBuffer)
	DECLARE_SCOPE(IndexBuffer)
	class SR_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetBufferLayout()const = 0;

		virtual uint32_t GetCount()const = 0;
		static IndexBufferRef Create(uint32_t* indices, uint32_t count);
	private:

	};
	
}

