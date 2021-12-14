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

#include "AudioSystem.h"
#include "JCollisionShape.h"
#include "PlayerComponent.h"
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

	Scene* _scene = SceneManager::CreateScene("Level");

	SceneManager::Load(_scene);

	_scene->GetAudioSystem()->Load("../Resources/sounds/koto.wav");
	_scene->GetAudioSystem()->Load("../Resources/sounds/kazeoto.wav");
	_scene->GetAudioSystem()->Load("../Resources/sounds/pon1.wav");

	GameObject* _obj;

	// camera
	_obj = SceneManager::InstanceObject("Camera", Vector3(0.f, 0.f, 18.f));
	auto& _sceneCamera = _obj->AddComponent<CameraComponent>();

	{ // player
		_obj = SceneManager::InstanceObject("Player");

		auto& _mrc = _obj->AddComponent<MeshRendererComponent>();
		_mrc.SetMesh(ResourceManager::GetDefaultMesh());
		_mrc.material = ResourceManager::GetDefaultMaterial();

		_obj->AddComponent<AudioComponent>();

		auto& _pc = _obj->AddComponent<PhysicsComponent>();
		_pc.definition.type = b2_dynamicBody;
		_pc.definition.gravityScale = 0.5f;
		_pc.shape.SetType(CollisionShape::Type::Rectangle);

		auto& _player = _obj->AddComponent<PlayerComponent>();
		_player.SupplyCamera(&_sceneCamera);
	}
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
