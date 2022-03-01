#include "pch.h"
#include "OpenGLFrameBuffer.h"
#include "Log.h"
#include <glad/glad.h>
namespace StarRing {

	namespace Utils {

		static GLenum TextureTarget(bool multisample) {
			return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void GenTextures(GLuint* out_id_arr) {
			glGenTextures(1, out_id_arr);
		};

		static void BindTexture(bool mutisample, GLuint id) {
			glBindTexture(TextureTarget(mutisample),id);
		}

		static void AttachColorTexture(GLuint id, int samples, GLenum format, GLuint width, GLuint height, int index) {
			bool mutisample = samples > 1;
			if (mutisample) {
				glEnable(GL_MULTISAMPLE);
				glfwWindowHint(GLFW_SAMPLES, samples);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,samples,format,width,height,GL_FALSE);
				/*glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
			}
			else {
				glBindTexture(GL_TEXTURE_2D, id);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA,GL_UNSIGNED_BYTE ,NULL);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, TextureTarget(mutisample), id, 0);
		}
	}



	OpenGLFrameBuffer::OpenGLFrameBuffer(const FramebufferSpecification& spec):
		m_FrameBufferSpecification(spec)
	{
		for (auto specification : m_FrameBufferSpecification.Attachments.Attachments) {
			m_ColorAttachmentSpecifications.emplace_back(specification);
		}
		Invalidate();	
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		for (int i = 0; i < m_ColorAttachments.size(); ++i) {
			glDeleteTextures(1, &m_ColorAttachments[i]);
		}
		m_ColorAttachments.clear();
		glDeleteTextures(1, &m_DepthStencilAttachment);
	}
	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			for (int i = 0; i < m_ColorAttachments.size(); ++i) {
				glDeleteTextures(1, &m_ColorAttachments[i]);
			}
			m_ColorAttachments.clear();
			glDeleteTextures(1, &m_DepthStencilAttachment);
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool IsMultiSample = m_FrameBufferSpecification.Samples > 1;
		//ColorAttachments
		if (m_ColorAttachmentSpecifications.size()) {
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			for (int i = 0; i < m_ColorAttachments.size(); ++i) {
				Utils::GenTextures(&m_ColorAttachments[i]);
				Utils::BindTexture(IsMultiSample,m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8: {
					Utils::AttachColorTexture(
						m_ColorAttachments[i],
						m_FrameBufferSpecification.
						Samples,GL_RGBA,
						m_FrameBufferSpecification.Width,
						m_FrameBufferSpecification.Height,
						i); break;
				}
				default:
					break;
				}
			}
		}

		glGenRenderbuffers(1, &m_DepthStencilAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilAttachment);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_FrameBufferSpecification.Width, m_FrameBufferSpecification.Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachment);
		
		if (m_ColorAttachments.size() > 1) {
			assert(m_ColorAttachments.size() <= 5, "FBO最多支持4个颜色缓冲");
			GLenum buffers[5] = { GL_COLOR_ATTACHMENT0 ,GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 ,GL_COLOR_ATTACHMENT3,GL_DEPTH_ATTACHMENT };
			glDrawBuffers(m_ColorAttachments.size() + 1, buffers);
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			assert(0, "Frame Buffer 不完整！");
			return;
		}
	}
	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_FrameBufferSpecification.Width, m_FrameBufferSpecification.Height);
	}
	void OpenGLFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Resize(const uint32_t x, const uint32_t y)
	{
		m_FrameBufferSpecification.Width = x;
		m_FrameBufferSpecification.Height = y;
		Invalidate();
	}
	FramebufferSpecification OpenGLFrameBuffer::GetSpecifiation() const
	{
		return m_FrameBufferSpecification;
	}
	uint32_t OpenGLFrameBuffer::GetColorAttachmentHandle(int idx) const
	{
		return m_ColorAttachments[idx];
	}
	uint32_t OpenGLFrameBuffer::GetDepthStencilAttachmentHandle() const
	{
		return m_DepthStencilAttachment;
	}
}