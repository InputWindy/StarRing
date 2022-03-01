#pragma once
//StarRing Application所需头文件
#include "Core.h"
#include "Timer.h"
#include "Application.h"
#include "WindowsInput.h"
#include "MouseButtonCodes.h"
#include "KeyCodes.h"
#include "LayerStack.h"
#include "Log.h"
#include "EditorCamera.h"

#include "DeltaTime.h"

#include "ImguiLayer.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"

#include "Renderer.h"
#include "RendererCommand.h"

#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "SceneCamera.h"

#include "OrthographicCameraController.h"
#include "ResourceManager.h"

//入口点
//#include "EntryPoint.h"

using namespace StarRing;

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/geometric.hpp>
using namespace glm;