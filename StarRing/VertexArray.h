#pragma once
#include "Core.h"
#include "Buffer.h"
namespace StarRing {
	DECLARE_REF(VertexArray)
	DECLARE_SCOPE(VertexArray)
	class SR_API VertexArray
	{
		friend class MeshRendererComponent;
	public:
		virtual ~VertexArray() {};

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;

		virtual uint32_t GetHandle()const = 0;

		virtual void AddVertexBuffer(const VertexBufferRef& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const IndexBufferRef& indexBuffer) = 0;

		virtual const vector<VertexBufferRef> GetVertexBuffers()const = 0;
		virtual const IndexBufferRef GetIndexBuffer()const = 0;

		static VertexArrayRef Create();
		static VertexArrayRef GetSkyBoxVAO();

	};
	
}
