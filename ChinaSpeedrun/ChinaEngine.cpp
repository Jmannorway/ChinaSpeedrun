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
#include "BSpline.h"
#include "PolygonCollider.h"

#include "SphereCollider.h"
#include "StaticBody.h"
#include "Rigidbody.h"
#include "Script.h"
#include "BulletManagerComponent.h"

#include "Delaunay.h"

#include "Editor.h"
#include "Draw.h"
#include "SceneManager.h"
//#include "EditorProfiler.h"

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

	/*Shader* _shader{ ResourceManager::Load<Shader>("../Resources/shaders/default_shader") };
	_shader->AssignShaderVertexInputAttrib("position", 0, Shader::Data::VEC3);
	_shader->AssignShaderVertexInputAttrib("color", 1, Shader::Data::VEC3);
	_shader->AssignShaderVertexInputAttrib("texCoord", 2, Shader::Data::VEC2);
	_shader->AssignShaderVertexBinding(Shader::InputRate::VERTEX);
	_shader->AssignShaderDescriptor("ubo", 0, Shader::Type::VERTEX, Shader::Data::UNIFORM);
	_shader->AssignShaderDescriptor("texSampler", 1, Shader::Type::FRAGMENT, Shader::Data::SAMPLER2D);

	Texture* _debugTexture{ ResourceManager::Load<Texture>("../Resources/textures/debug_texture.png") };
	_debugTexture->filter = Texture::Filter::NEAREST;

	Material* _material1{ ResourceManager::Load<Material>("../Resources/materials/test1.mat") };

	_material1->shader = _shader;
	_material1->shaderParams["texSampler"] = _debugTexture;*/

	/*SceneManager::Load(SceneManager::CreateScene("Scene"));
	auto _obj = SceneManager::InstanceObject("map");
	auto& _c = _obj->AddComponent<MeshRendererComponent>();
	_c.SetMesh(ResourceManager::LoadModelFromMapData("../Resources/models/map_data.txt"));
	_c.material = ResourceManager::Load<Material>("../Resources/materials/default_material.mat");*/

	// Set up scene
	{
		/*SceneManager::Load(SceneManager::CreateScene("Scene"));

		GameObject* _obj{ SceneManager::InstanceObject("Ball", Vector3(1.f)) };
		MeshRendererComponent& _sphereMeshComponent{ _obj->AddComponent<MeshRendererComponent>() };
		_sphereMeshComponent.SetMesh(_defaultMesh);
		_sphereMeshComponent.material = _defaultMaterial;
		_obj->AddComponent<PhysicsComponent>();

		GameObject* _obj2{ SceneManager::InstanceObject("Ball2", Vector3(1.f)) };
		MeshRendererComponent& _sphereMeshComponent2{ _obj2->AddComponent<MeshRendererComponent>() };
		_sphereMeshComponent2.SetMesh(_defaultMesh);
		_sphereMeshComponent2.material = _defaultMaterial;
		_obj2->AddComponent<PhysicsComponent>();

		GameObject* _camera = SceneManager::InstanceObject("Camera", Vector3(0.f, 0.f, -3.f));
		CameraComponent& _cameraComponent{ _camera->AddComponent<CameraComponent>() };*/
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
