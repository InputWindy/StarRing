#pragma once
#include "Core.h"
#include "RendererCommand.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "OrthographicCameraController.h"
#include "Light.h"
#include "RenderState.h"
#include "SkyBox.h"
#include "Uniform.h"
#include "Sampler.h"
#include "Material.h"
#include "EditorCamera.h"
#include "ResourceManager.h"
namespace StarRing {

	namespace Utils {
		static void PrintMat4(mat4& mat) {
			cout << endl;
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					cout << mat[i][j] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}

		static string Int2String(int num) {
			stringstream ss;
			ss << num;
			return ss.str();
		}
	}

	static inline bool ms_IsSkyBoxRendered = false;

	//Renderer需要维护一大堆更新RenderCtx的接口，供MeshRendererComponent调用
	class SR_API Renderer
	{
		friend class EditorLayer;
		public:
			~Renderer();
			//初始化RenderCtx
			static void Init();
			static void OnWindowResize(uint32_t width,uint32_t height);

			//清空Buffer,渲染天空盒
			static void BeginRendering();
			//清空光源
			static void EndRendering();


			static void SetThisDrawCallRenderToScreen();
			static void SetThisDrawCallRenderToFrameBuffer();
			//绘制一个Mesh
			static void DrawCall();

			
			//是一个身份信息
			static inline RendererAPI::API GetRenderAPI() { return RendererAPI::GetAPI() ; }
		public:	
			static void SetCameraPosition(const vec3 cameraPos);
			static void SetViewMatrix(const mat4 vMat);
			static void SetProjectionMatrix(const mat4 pMat);
			static void SetModelMatrix(const mat4 mMat);
			static void SetMaterial(MaterialRef& material);
			static void SetVertexArray(VertexArrayRef& vertexArray);
			static void SetSkyBox(SkyBoxRef& skybox);
			static void SetFrameBuffer(FrameBufferRef& fbo);
			static FrameBufferRef& GetFrameBuffer();

			static void AddDirectionalLight(DirectionalLight directional);
			static void AddPointLight(PointLight point);
			static void AddSpotLight(SpotLight spot);
			static void AddAreaLight(AreaLight area);

		private:
			static void ClearLight();
			static void AnalyzeRenderState();
			static void RenderSkyBox();
			//传递MVP矩阵,相机位置,光源数组
			static void DeliverUniforms();

			//渲染上下文里的所有东西都不能修改，都是只读的
			struct RenderCtx {
				//CameraComponent
				vec3 m_CameraPosition = vec3(0.0f, 0.0f, 0.0f);
				mat4 _ViewMatrix = mat4(1.0f);
				mat4 _ProjectionMatrix = mat4(1.0f);

				//LightComponent(每种类型光源都最多支持4个)
				vector<DirectionalLight> m_DirectionalLights;
				vector<PointLight> m_PointLights;
				vector<SpotLight> m_SpotLights;
				vector<AreaLight> m_AreaLights;
				
				//MeshRendererComponent
				MaterialRef m_Material = nullptr;

				//MeshComponent
				VertexArrayRef m_VertexArray = nullptr;
				mat4 _ModelMatrix = mat4(1.0f);

				//其他渲染资源
				SkyBoxRef m_SkyBox = nullptr;

				//编辑器Scene所使用的FBO，用于渲染屏幕矩形，是一个固定的FBO,不能修改
				bool m_bRenderScreen = true;
				FrameBufferRef m_SceneFbo = nullptr; 
				
				//这个是多Pass渲染所使用的自定义FBO
				FrameBufferRef m_CustomFrameBuffer = nullptr;
				
			};

			static RenderCtx* ms_RenderCtx;
	};

}

