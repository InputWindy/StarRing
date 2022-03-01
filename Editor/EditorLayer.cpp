#include "EditorLayer.h"
#include "ImGuizmo.h"
namespace StarRing {
extern const std::filesystem::path g_AssetPath;

string vertexFile = "C:/Users/InputWindy/Desktop/StarRing/StarRing/Editor/assets/shaders/Texture.vert";
string fragmentFile = "C:/Users/InputWindy/Desktop/StarRing/StarRing/Editor/assets/shaders/Texture.frag";


float CubeVertex[] = {
	// X轴0.5处的平面
	0.5,  -0.5,    0.5f, 1,  0,  0, 0, 0,   // VertexA
	0.5,  -0.5f,  -0.5f, 1,  0,  0, 0, 1,   // VertexB
	0.5,  0.5f,   -0.5f, 1,  0,  0, 1, 1,   // VertexC
	0.5,  0.5,    -0.5f, 1,  0,  0, 1, 1,   // VertexC
	0.5,  0.5f,    0.5f, 1,  0,  0, 1, 0,   // VertexD
	0.5,  -0.5f,   0.5f, 1,  0,  0, 0, 0,   // VertexA
	// X轴-0.5处的平面
	-0.5,  -0.5,    0.5f, -1,  0,  0, 0, 0, // VertexE
	-0.5,  -0.5f,  -0.5f, -1,  0,  0, 0, 1, // VertexF
	-0.5,  0.5f,   -0.5f, -1,  0,  0, 1, 1, // VertexG
	-0.5,  0.5,    -0.5f, -1,  0,  0, 1, 1, // VertexG
	-0.5,  0.5f,    0.5f, -1,  0,  0, 1, 0, // VertexH
	-0.5,  -0.5f,   0.5f, -1,  0,  0, 0, 0, // VertexE

	-0.5,  0.5,  0.5f, 0,  1,  0, 0, 0,     // VertexH
	-0.5f, 0.5, -0.5f, 0,  1,  0, 0, 1,     // VertexG
	0.5f, 0.5,  -0.5f, 0,  1,  0, 1, 1,     // VertexC
	0.5,  0.5,  -0.5f, 0,  1,  0, 1, 1,     // VertexC
	0.5f, 0.5,   0.5f, 0,  1,  0, 1, 0,     // VertexD
	-0.5f, 0.5,  0.5f, 0,  1,  0, 0, 0,     // VertexH
	-0.5, -0.5,   0.5f, 0,  -1,  0, 0, 0,   // VertexE
	-0.5f, -0.5, -0.5f, 0,  -1,  0, 0, 1,   // VertexF
	0.5f, -0.5,  -0.5f, 0,  -1,  0, 1, 1,   // VertexB
	0.5,  -0.5,  -0.5f, 0,  -1,  0, 1, 1,   // VertexB
	0.5f, -0.5,   0.5f, 0,  -1,  0, 1, 0,   // VertexA
	-0.5f, -0.5,  0.5f, 0,  -1,  0, 0, 0,   // VertexE

	-0.5,   0.5f,  0.5,   0,  0,  1, 0, 0,  // VertexH
	-0.5f,  -0.5f,  0.5,  0,  0,  1, 0, 1,  // VertexE
	0.5f,   -0.5f,  0.5,  0,  0,  1, 1, 1,  // VertexA
	0.5,    -0.5f, 0.5,   0,  0,  1, 1, 1,  // VertexA
	0.5f,  0.5f,  0.5,    0,  0,  1, 1, 0,  // VertexD
	-0.5f,   0.5f,  0.5,  0,  0,  1, 0, 0,  // VertexH
	-0.5,   0.5f,  -0.5,   0,  0,  -1, 0, 0,    // VertexG
	-0.5f,  -0.5f,  -0.5,  0,  0,  -1, 0, 1,    // VertexF
	0.5f,   -0.5f,  -0.5,  0,  0,  -1, 1, 1,    // VertexB
	0.5,    -0.5f, -0.5,   0,  0,  -1, 1, 1,    // VertexB
	0.5f,  0.5f,  -0.5,    0,  0,  -1, 1, 0,    // VertexC
	-0.5f,   0.5f,  -0.5,  0,  0,  -1, 0, 0,    // VertexG
};

unsigned int CubeIndices[] = {
		0,      // VertexA
		1,      // VertexB
		2,      // VertexC
		2,      // VertexC
		3,      // VertexD
		0,      // VertexA

		4,      // VertexE
		5,      // VertexF
		6,      // VertexG
		6,      // VertexG
		7,      // VertexH
		4,      // VertexE

		7,      // VertexH
		6,      // VertexG
		2,      // VertexC
		2,      // VertexC
		3,      // VertexD
		7,      // VertexH
		4,      // VertexE
		5,      // VertexF
		1,      // VertexB
		1,      // VertexB
		0,      // VertexA
		4,      // VertexE

		7,      // VertexH
		4,      // VertexE
		0,      // VertexA
		0,      // VertexA
		3,      // VertexD
		7,      // VertexH
		6,      // VertexG
		5,      // VertexF
		1,      // VertexB
		1,      // VertexB
		2,      // VertexC
		6,      // VertexG
};

EditorLayer::EditorLayer()
	: Layer("EditorLayer")
{
	OnAwake();
}

void EditorLayer::OnAwake()
{
	//ResourceManager::LoadModel("C:/Users/InputWindy/Desktop/MWDRenderer/objModel/lucia/f.fbx");
	#pragma region 测试用渲染器
	/*m_VertexArray = VertexArray::Create();

	VertexBufferRef m_VertexBuffer = VertexBuffer::Create(CubeVertex, sizeof(CubeVertex));
	m_VertexBuffer->Bind();

	BufferLayout layout = {
		{ShaderDataType::Float3,"a_Position"},
	};
	m_VertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	IndexBufferRef m_IndexBuffer = IndexBuffer::Create(CubeIndices, sizeof(CubeIndices)/sizeof(unsigned int));
	m_IndexBuffer->Bind();

	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	m_Shader = Shader::Create(vertexFile, fragmentFile);

	Renderer::SetVertexArray(m_VertexArray);*/
	#pragma endregion

	m_ActiveScene = Scene::Create("MainScene", m_viewPortPanelSize.x, m_viewPortPanelSize.y);
	//ResourceManager::LogResource();
	m_ActiveScene->AddPrefab("Lucia", "C:/Users/InputWindy/Desktop/StarRing/StarRing/Editor/assets/model/lucia/f.fbx");
	m_ActiveScene->AddPrefab("Gun", "C:/Users/InputWindy/Desktop/StarRing/StarRing/Editor/assets/model/gun/gun/Cerberus_LP.FBX");
	//ResourceManager::LogResource();
	//m_ActiveScene->LogScene();
	m_ActiveScene->AddSunLight();
	m_SceneHierarchy.SetScene(m_ActiveScene);
	m_ContentBrowser.SetScene(m_ActiveScene);
	m_Console.SetScene(m_ActiveScene);

}

void EditorLayer::OnImGuiRender()
{
	ImGui_Docking_Begin("Editor")

	#pragma region 主菜单
	if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) {}
					//NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O")) {}
					//OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
					//SaveSceneAs();

				if (ImGui::MenuItem("Exit")) Application::GetApplication().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	#pragma endregion

	#pragma region Scene
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Scene");
	
	Application::GetApplication().GetImGuiLayer()->SetBlockEvents(!ImGui::IsWindowFocused() | !ImGui::IsWindowHovered());
	ImGui::Image(((void*)(Renderer::ms_RenderCtx->m_SceneFbo->GetColorAttachmentHandle(0))), ImGui::GetContentRegionAvail(), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	
	#pragma region Gizmos
	Entity* entt = m_SceneHierarchy.GetSelectedEntity();
	if (entt) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		float window_wid = (float)ImGui::GetWindowWidth(), window_hei = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_wid, window_hei);

		//Transform
		TransformComponent& trans = entt->GetTransformComponent();
		mat4 transform = trans.GetTransform();
		ImGuizmo::Manipulate(value_ptr(m_EditorCamera.GetView()), 
			value_ptr(m_EditorCamera.GetViewProjection()), 
			ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, 
			value_ptr(transform));

		if (ImGuizmo::IsUsing()) {
			trans.Translation = vec3(transform[3]);
		}
	}
	#pragma endregion

	ImGui::End();
	ImGui::PopStyleVar();
	#pragma endregion

	#pragma region Hierarchy&Inspector
	m_SceneHierarchy.OnImGuiRender();
	#pragma endregion

	#pragma region 文件视图
	m_ContentBrowser.OnImGuiRender();
	#pragma endregion

	#pragma region Consolo控制台
	m_Console.OnImGuiRender();
	#pragma endregion

	ImGui_Docking_End
}

//执行渲染操作
void EditorLayer::OnUpdate()
{
	m_SceneHierarchy.OnUpdate();
	m_ContentBrowser.OnUpdate();
	m_Console.OnUpdate();

	if (m_bViewportFocused) {
		m_EditorCamera.OnUpdate();
	}
	m_ActiveScene->RenderScene(m_EditorCamera);
}
}