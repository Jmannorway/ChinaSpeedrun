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

	// Create important resources
	Mesh* _defaultMesh = ResourceManager::Load<Mesh>("../Resources/models/default_mesh.obj");

	// Create scene and contained objects
	SceneManager::Load(SceneManager::CreateScene("Scene"));

	GameObject* _obj = SceneManager::InstanceObject(
		"Object",
		Vector3(9.7f, 4.f, 5.8f), 
		Vector3(-26.f, 45.2f, 0.f));
	CameraComponent& _cc = _obj->AddComponent<CameraComponent>();

	_obj = SceneManager::InstanceObject("Plane mesh");
	auto& _mrc = _obj->AddComponent<MeshRendererComponent>();
	_mrc.SetMesh(ResourceManager::Load<Mesh>("../Resources/models/stcol1.obj"));
	_mrc.material = ResourceManager::GetDefaultMaterial();
	SceneUtility::CreateStaticTriangleCollidersFromMesh(_mrc.mesh);

	_obj = SceneManager::InstanceObject(
		"Mesh object 3", 
		Vector3(-0.75f, 2.f, 0.3f),
		Vector3(0.0f),
		Vector3(0.4f));
	auto& _mrc3 = _obj->AddComponent<MeshRendererComponent>();
	_mrc3.SetMesh(ResourceManager::Load<Mesh>("../Resources/models/icosphere.obj"));
	_mrc3.material = ResourceManager::GetDefaultMaterial();
	auto& _jpc3 = _obj->AddComponent<JPhysicsComponent>();
	_jpc3.shape = new JCollisionSphere(0.4f);
	_jpc3.gravityScale = 0.25f;
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
