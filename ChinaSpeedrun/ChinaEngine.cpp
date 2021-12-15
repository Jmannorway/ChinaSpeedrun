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

	Material* _playerMaterial = ResourceManager::Load<Material>("../Resources/materials/player_material.mat");
	_playerMaterial->shader = ResourceManager::GetDefaultShader();
	_playerMaterial->cullMode = Material::CullMode::NONE;
	auto _texture = ResourceManager::Load<Texture>("../Resources/textures/default_texture.png");
	_texture->filter = Texture::Filter::NEAREST;
	_playerMaterial->shaderParams["texSampler"] = _texture;

	ResourceManager::Load<Scene>("../Resources/scenes/synthas pizzeria 2");
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
