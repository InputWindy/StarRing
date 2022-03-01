#include "pch.h"
#include "Renderer.h"
#include "RendererCommand.h"
#include "Log.h"
namespace StarRing {

	

	Renderer::RenderCtx* Renderer::ms_RenderCtx = new Renderer::RenderCtx();

	Renderer::~Renderer()
	{
		delete ms_RenderCtx;
		ms_RenderCtx = nullptr;
	}

	void Renderer::Init()
	{
		ms_RenderCtx->m_SceneFbo = FrameBuffer::Create(FramebufferSpecification::GetDefault());
		ms_RenderCtx->m_DirectionalLights.clear();
		ms_RenderCtx->m_PointLights.clear();
		ms_RenderCtx->m_SpotLights.clear();
		ms_RenderCtx->m_AreaLights.clear();
		ms_RenderCtx->m_SkyBox = SkyBox::GetDefault();
		ms_RenderCtx->m_Material = Material::GetDefault();
		ms_RenderCtx->m_bRenderScreen = true;
		ms_RenderCtx->m_CameraPosition = vec3(0.0f, 0.0f, 0.0f);
		ms_RenderCtx->m_CustomFrameBuffer = nullptr;
		ms_RenderCtx->m_VertexArray = nullptr;
		ms_RenderCtx->_ModelMatrix = mat4(1.0f);
		ms_RenderCtx->_ViewMatrix = mat4(1.0f);
		ms_RenderCtx->_ProjectionMatrix = mat4(1.0f);
		
		RendererCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewPort(0,0,width,height);
	}

	//每帧开始时执行一次
	void Renderer::BeginRendering()
	{
		ms_RenderCtx->m_SceneFbo->Bind();
		//先清空各种buffer/然后第一个渲染天空盒
		RendererCommand::Clear();
		RenderSkyBox();
		ms_RenderCtx->m_SceneFbo->UnBind();
	}
	//每帧结束时执行一次
	void Renderer::EndRendering()
	{
		ClearLight();
	}
	//每一个Mesh执行一次
	void Renderer::DrawCall()
	{
		if (ms_RenderCtx->m_bRenderScreen) {
			ms_RenderCtx->m_SceneFbo->Bind();
		}
		else {
			ms_RenderCtx->m_CustomFrameBuffer->Bind();
		}

		//解析渲染状态
		AnalyzeRenderState();
		ms_RenderCtx->m_VertexArray->Bind(); 
		//设置Shader
		DeliverUniforms();

		//绘制
		RendererCommand::DrawIndexed(ms_RenderCtx->m_VertexArray);

		if (ms_RenderCtx->m_bRenderScreen) {
			ms_RenderCtx->m_SceneFbo->UnBind();
		}
		else {
			ms_RenderCtx->m_CustomFrameBuffer->UnBind();
		}
	}

	void Renderer::SetCameraPosition(const vec3 cameraPos)
	{
		ms_RenderCtx->m_CameraPosition = cameraPos;
	}
	void Renderer::SetViewMatrix(const mat4 vMat)
	{
		ms_RenderCtx->_ViewMatrix = vMat;
	}
	void Renderer::SetProjectionMatrix(const mat4 pMat)
	{
		ms_RenderCtx->_ProjectionMatrix = pMat;
	}
	void Renderer::SetModelMatrix(const mat4 mMat)
	{
		ms_RenderCtx->_ModelMatrix = mMat;
	}
	void Renderer::SetMaterial(MaterialRef& material)
	{
		ms_RenderCtx->m_Material = material;
	}
	void Renderer::SetVertexArray(VertexArrayRef& vertexArray)
	{
		ms_RenderCtx->m_VertexArray = vertexArray;
	}
	void Renderer::SetSkyBox(SkyBoxRef& skybox)
	{
		ms_RenderCtx->m_SkyBox = skybox;
	}
	void Renderer::SetFrameBuffer(FrameBufferRef& fbo) {
		ms_RenderCtx->m_CustomFrameBuffer = fbo;
		ms_RenderCtx->m_bRenderScreen = false;
	}
	FrameBufferRef& Renderer::GetFrameBuffer()
	{
		return ms_RenderCtx->m_CustomFrameBuffer;
	};
	void Renderer::AddDirectionalLight(DirectionalLight directional)
	{
		ms_RenderCtx->m_DirectionalLights.push_back(directional);
	}
	void Renderer::AddPointLight(PointLight point)
	{
		ms_RenderCtx->m_PointLights.push_back(point);
	}
	void Renderer::AddSpotLight(SpotLight spot)
	{
		ms_RenderCtx->m_SpotLights.push_back(spot);
	}
	void Renderer::AddAreaLight(AreaLight area)
	{
		ms_RenderCtx->m_AreaLights.push_back(area);
	}
	void Renderer::SetThisDrawCallRenderToScreen()
	{
		ms_RenderCtx->m_bRenderScreen = true;
	}
	void Renderer::SetThisDrawCallRenderToFrameBuffer()
	{
		ms_RenderCtx->m_bRenderScreen = false;
	}
	void Renderer::ClearLight()
	{
		ms_RenderCtx->m_DirectionalLights.clear();
		ms_RenderCtx->m_PointLights.clear();
		ms_RenderCtx->m_SpotLights.clear();
		ms_RenderCtx->m_AreaLights.clear();
	}
	void Renderer::AnalyzeRenderState()
	{
		#pragma region 填写渲染状态
		RenderState render_state = ms_RenderCtx->m_Material->m_RenderState;
		if (render_state.m_bEnableDepth) {
			glEnable(GL_DEPTH_TEST);
			if (render_state.m_bDepthReadOnly) {
				glDepthMask(GL_FALSE);
			}
			glDepthFunc(render_state.m_CompareMethod);
		}
		if (render_state.m_bEnableBlend) {
			glEnable(GL_BLEND);
			glBlendEquation(render_state.m_uiBlendFuncMode);
			glBlendFunc(render_state.m_uiBlendSrc, render_state.m_uiBlendDest);
		}
		if (render_state.m_bEnableStencil) {
			glEnable(GL_STENCIL);
			if (render_state.m_bStencilReadOnly) {
				glStencilFunc(render_state.m_CompareMethod, render_state.m_uiStencilRef, 0x00);
			}
			else {
				glStencilFunc(render_state.m_CompareMethod, render_state.m_uiStencilRef, 0xFF);
			}

			glStencilOp(render_state.m_uiSFailOp, render_state.m_uiSPassZFailOp, render_state.m_uiPassOp);
		}
		if (render_state.m_bEnableCull) {
			glEnable(GL_CULL_FACE);
			glCullFace(render_state.m_uiCullMode);
			glFrontFace(render_state.m_uiFrontDefinition);
		}
		#pragma endregion
	}
	void Renderer::RenderSkyBox()
	{
		if (!ms_RenderCtx->m_SkyBox) {
			return;
		}
		else {
			glDepthMask(GL_FALSE);
			mat4 _View = mat4(mat3(ms_RenderCtx->_ViewMatrix));
			mat4 _Projection = ms_RenderCtx->_ProjectionMatrix;
			ShaderRef skyboxShader = ms_RenderCtx->m_SkyBox->m_SkyBoxShader;
			VertexArrayRef skyboxVAO = ms_RenderCtx->m_SkyBox->m_SkyBoxVAO;
			TextureCubicRef skyTexture = ms_RenderCtx->m_SkyBox->m_SkyBoxTexture;
			//Utils::PrintMat4(_View);
			skyboxShader->Bind();
			skyTexture->Bind(0);
			skyboxShader->SetUniformInt("skybox", 0);
			skyboxShader->SetUniformMat4("view", _View);
			skyboxShader->SetUniformMat4("projection", _Projection);

			glBindVertexArray(skyboxVAO->GetHandle());
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			skyTexture->UnBind(0);
		}
	}
	void Renderer::DeliverUniforms()
	{
		//绑定Shader并传递参数
		ShaderRef& shader = ms_RenderCtx->m_Material->m_Shader;
		shader->Bind();

		//传递内置参数:MVP矩阵/相机位置/光源
		shader->SetUniformMat4("model",ms_RenderCtx->_ModelMatrix);
		shader->SetUniformMat4("view",ms_RenderCtx->_ViewMatrix);
		shader->SetUniformMat4("projection", ms_RenderCtx->_ProjectionMatrix);

		shader->SetUniformFloat3("cameraPos", ms_RenderCtx->m_CameraPosition.x, ms_RenderCtx->m_CameraPosition.y, ms_RenderCtx->m_CameraPosition.z);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ms_RenderCtx->m_SkyBox->m_SkyBoxTexture->GetHandle());
		shader->SetUniformInt("environment_map", 0);
		vec3 sunColor = ms_RenderCtx->m_DirectionalLights[0].dColor;
		vec3 sunDir = ms_RenderCtx->m_DirectionalLights[0].dLightDir;
		shader->SetUniformFloat3("sunLight.Color", sunColor.x, sunColor.y, sunColor.z);
		shader->SetUniformFloat3("sunLight.Direction", sunDir.x, sunDir.y, sunDir.z);
		//环境光贴图environment_map(暂时没有)

		//阳光
		/*vec3 sunColor = ms_RenderCtx->m_DirectionalLights[0].dColor;
		vec3 sunDir = ms_RenderCtx->m_DirectionalLights[0].dLightDir;
		shader->SetUniformFloat3("sunLight.Color", sunColor.x, sunColor.y, sunColor.z);
		shader->SetUniformFloat3("sunLight.Direction", sunDir.x, sunDir.y, sunDir.z);*/

		//点光源
		for (int i = 0; i < ms_RenderCtx->m_PointLights.size();++i) {
			vec3 Color = ms_RenderCtx->m_PointLights[i].pColor;
			vec3 Pos = ms_RenderCtx->m_PointLights[i].pPosition;
			float CutOff = ms_RenderCtx->m_PointLights[i].pCutOff;

			shader->SetUniformFloat3(string("pointLights[") + Utils::Int2String(i) + string("].Color"), Color.x, Color.y, Color.z);
			shader->SetUniformFloat3(string("pointLights[") + Utils::Int2String(i) + string("].Position"), Pos.x, Pos.y, Pos.z);
			shader->SetUniformFloat(string("pointLights[") + Utils::Int2String(i) + string("].Position"), CutOff);
		}

		//聚光灯
		for (int i = 0; i < ms_RenderCtx->m_SpotLights.size(); ++i) {
			vec3 Color = ms_RenderCtx->m_SpotLights[i].sColor;
			vec3 Pos = ms_RenderCtx->m_SpotLights[i].sPosition;
			vec3 Dir = ms_RenderCtx->m_SpotLights[i].sLightDir;
			float Angle = ms_RenderCtx->m_SpotLights[i].sAngle;
			float CutOff = ms_RenderCtx->m_SpotLights[i].sCutOff;

			shader->SetUniformFloat3(string("spotLights[") + Utils::Int2String(i) + string("].Color"), Color.x, Color.y, Color.z);
			shader->SetUniformFloat3(string("spotLights[") + Utils::Int2String(i) + string("].Position"), Pos.x, Pos.y, Pos.z);
			shader->SetUniformFloat3(string("spotLights[") + Utils::Int2String(i) + string("].Direction"), Dir.x, Dir.y, Dir.z);
			shader->SetUniformFloat(string("spotLights[") + Utils::Int2String(i) + string("].Angle"), Angle);
			shader->SetUniformFloat(string("spotLights[") + Utils::Int2String(i) + string("].CutOff"), CutOff);
		}

		//面光源
		for (int i = 0; i < ms_RenderCtx->m_AreaLights.size(); i++)
		{
			vec3 Color = ms_RenderCtx->m_AreaLights[i].aColor;
			vec3 Position = ms_RenderCtx->m_AreaLights[i].aPosition;
			vec3 Normal = ms_RenderCtx->m_AreaLights[i].aNormal;
			float width = ms_RenderCtx->m_AreaLights[i].awidth;
			float height = ms_RenderCtx->m_AreaLights[i].aheight;
			float CutOff = ms_RenderCtx->m_AreaLights[i].aCutOff;

			shader->SetUniformFloat3(string("AreaLights[") + Utils::Int2String(i) + string("].Color"), Color.x, Color.y, Color.z);
			shader->SetUniformFloat3(string("AreaLights[") + Utils::Int2String(i) + string("].Position"), Position.x, Position.y, Position.z);
			shader->SetUniformFloat3(string("AreaLights[") + Utils::Int2String(i) + string("].Normal"), Normal.x, Normal.y, Normal.z);
			shader->SetUniformFloat(string("AreaLights[") + Utils::Int2String(i) + string("].width"), width);
			shader->SetUniformFloat(string("AreaLights[") + Utils::Int2String(i) + string("].height"), height);
			shader->SetUniformFloat(string("AreaLights[") + Utils::Int2String(i) + string("].CutOff"), CutOff);
		}

		//传递实时参数
		ms_RenderCtx->m_Material->m_Uniforms->SetUniforms(shader);
		ms_RenderCtx->m_Material->m_Samplers->SetSampler2D(shader);
	}
}