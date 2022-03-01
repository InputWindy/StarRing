#pragma once
#include "VertexArray.h"
namespace StarRing {
	class OpenGLVertexArray:public VertexArray
	{
		public:
			OpenGLVertexArray();
			virtual ~OpenGLVertexArray() ;

			virtual void Bind()const override;
			virtual void UnBind()const override;

			virtual uint32_t GetHandle()const override;

			virtual void AddVertexBuffer(const VertexBufferRef& vertexBuffer) override;
			virtual void SetIndexBuffer(const IndexBufferRef& indexBuffer) override;

			virtual const vector<VertexBufferRef> GetVertexBuffers()const override {
				return m_VertexBuffers;
			};
			virtual const IndexBufferRef GetIndexBuffer()const override {
				return m_IndexBuffer;
			};
		private:
			uint32_t m_RendererID;

			vector<VertexBufferRef> m_VertexBuffers;
			IndexBufferRef m_IndexBuffer;
	};
}
