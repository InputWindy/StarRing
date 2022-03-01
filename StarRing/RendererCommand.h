#pragma once
#include "RendererAPI.h"
namespace StarRing {

	//�ײ���ȾAPI�����ĳ��󣬸�ͨ����Ⱦ���ṩ���õĽӿ�
	class SR_API RendererCommand
	{
	public:
		static inline void Init();
		static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height );
		static inline void SetClearColor(const vec4 color) {
			ms_RenderAPI->SetClearColor(color);
		};
		static inline void Clear() {
			ms_RenderAPI->Clear();
		};
		static inline void ActiveTexUnit(unsigned int slot) {
			ms_RenderAPI->ActiveTexUnit(GL_TEXTURE0 + slot);
		};
		static inline void DrawIndexed(const VertexArrayRef& vertexArray) {
			return ms_RenderAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* ms_RenderAPI;
	};
}

