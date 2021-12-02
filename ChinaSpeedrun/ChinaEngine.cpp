#include "ChinaEngine.h"

#include <vector>

#include "ResourceManager.h"

#include "Vertex.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "VulkanEngineRenderer.h"

#include "ImGuiLayer.h"

#include "Input.h"
#include "GameObject.h"
#include "Components.h"
#include "Camera.h"
#include "Script.h"
#include "BulletManagerComponent.h"

#include "Editor.h"
#include "Draw.h"
#include "SceneManager.h"
//#include "EditorProfiler.h"

#include "JCollisionShape.h"
#include "SceneUtility.h"
#include "Time.h"

cs::VulkanEngineRenderer cs::ChinaEngine::renderer;
cs::editor::EngineEditor cs::ChinaEngine::editor;

void cs::ChinaEngine::Run()
{
	Time::CycleInit();
	Mathf::InitRand();

	renderer.Create(800, 600, "China Speedrun");

	editor.Start();
	Draw::Setup();
	renderer.Resolve();
	Draw::CreateDescriptorSets();
	Draw::DebugGrid();

	EngineInit();

	renderer.Resolve();

	InitInput();

	MainLoop();
	EngineExit();
}

float cs::ChinaEngine::AspectRatio()
{
	return renderer.AspectRatio();
}

void cs::ChinaEngine::FramebufferResizeCallback(GLFWwindow* window, int newWidth, int newHeight)
{
	if (newWidth * newHeight > 0) // don't recalculate the perspective if the screen size is 0
		Camera::CalculateProjection(*SceneManager::mainCamera);
}

void cs::ChinaEngine::EngineInit()
{
	ResourceManager::CreateDefaultResources();

	GameObject* _obj;

	// Scene containing the first sphere triangle collision example
	

	{
		SceneManager::Load(SceneManager::CreateScene("Map data collision"));

		_obj = SceneManager::InstanceObject(
			"Object",
			Vector3(9.7f, 4.f, 5.8f),
			Vector3(-26.f, 45.2f, 0.f));
		_obj->AddComponent<CameraComponent>();

		_obj = SceneManager::InstanceObject("Map for collision", Vector3(-205.6f, -24.5f, -236.7f));
		auto& _mrc = _obj->AddComponent<MeshRendererComponent>();
		_mrc.SetMesh(ResourceManager::LoadModelFromMapData("../Resources/test_las.txt", 25000, 8, 8));
		_mrc.material = ResourceManager::GetDefaultMaterial();

		_obj = SceneManager::InstanceObject(
			"Rolling ball",
			Vector3(0.f, 3.5f, 0.3f),
			Vector3(0.0f),
			Vector3(0.4f));
		auto& _mrc2 = _obj->AddComponent<MeshRendererComponent>();
		_mrc2.SetMesh(ResourceManager::Load<Mesh>("../Resources/models/icosphere.obj"));
		_mrc2.material = ResourceManager::GetDefaultMaterial();
		auto& _jpc2 = _obj->AddComponent<JPhysicsComponent>();
		_jpc2.shape = new JCollisionSphere(0.4f);
		_jpc2.gravityScale = 0.25f;
	}

	SceneManager::SetCurrentFocusedScene(0);
}

void cs::ChinaEngine::InitInput()
{
	glfwSetKeyCallback(renderer.GetWindow(), Input::GlfwKeyfunCallback);
	glfwSetCursorPosCallback(renderer.GetWindow(), Input::GlfwCursorPosCallback);
	glfwSetScrollCallback(renderer.GetWindow(), Input::GlfwScrollCallback);
	glfwSetMouseButtonCallback(renderer.GetWindow(), Input::GlfwMouseButtonCallback);

	Input::AddMapping("accept", GLFW_KEY_ENTER);
	Input::AddMapping("space", GLFW_KEY_SPACE);
	Input::AddMapping("shift", GLFW_KEY_LEFT_SHIFT);
	Input::AddMapping("up", GLFW_KEY_UP);
	Input::AddMapping("down", GLFW_KEY_DOWN);
	Input::AddMapping("left", GLFW_KEY_LEFT);
	Input::AddMapping("right", GLFW_KEY_RIGHT);
}

void cs::ChinaEngine::MainLoop()
{
	while (!glfwWindowShouldClose(renderer.GetWindow()))
	{
		Time::CycleStart();
		SceneManager::Resolve();

		glfwPollEvents();

		editor.Update();
		Draw::Update();
		SceneManager::Update();

		renderer.Resolve();
		renderer.DrawFrame();
		Input::FinishFrame();

		Time::CycleEnd();
	}

	vkDeviceWaitIdle(renderer.GetDevice());
}

void cs::ChinaEngine::EngineExit()
{
	editor.Exit();
	ResourceManager::ClearAllResourcePools();
	SceneManager::DestroyEverything();
}
