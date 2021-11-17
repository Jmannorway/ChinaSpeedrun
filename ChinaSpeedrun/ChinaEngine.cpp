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

#include "SphereCollider.h"
#include "StaticBody.h"
#include "Rigidbody.h"

#include "Editor.h"
#include "SceneManager.h"
//#include "EditorProfiler.h"

#include "Time.h"

#include "lua.hpp"

cs::VulkanEngineRenderer cs::ChinaEngine::renderer;
cs::editor::EngineEditor cs::ChinaEngine::editor;

void cs::ChinaEngine::Run()
{
	LuaTest();
	Time::CycleInit();
	Mathf::InitRand();

	renderer.Create(800, 600, "China Speedrun");

	editor.Start();

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

int cs::ChinaEngine::LuaTest()
{
	std::string command = "a = 30 + 90";
	int result = 0;

	lua_State* L = luaL_newstate();

	int r = luaL_dostring(L, command.c_str());

	if (r == LUA_OK)
	{
		lua_getglobal(L, "a");

		if (lua_isnumber(L, -1))
		{
			float a_in_cpp = static_cast<float>(lua_tonumber(L, -1));
			Debug::Log(a_in_cpp);
		}
	}
	else
	{
		result = 1;
	}

	lua_close(L);
	return result;
}

void cs::ChinaEngine::FramebufferResizeCallback(GLFWwindow* window, int newWidth, int newHeight)
{
	if (newWidth * newHeight > 0) // don't recalculate the perspective if the screen size is 0
		Camera::CalculatePerspective(*SceneManager::mainCamera);
}

void cs::ChinaEngine::EngineInit()
{
	Shader* _defaultShader;
	Texture* _defaultTexture;
	Material* _defaultMaterial;
	Mesh* _defaultMesh;

	// Default resources setup
	{
		_defaultShader = ResourceManager::Load<Shader>("../Resources/shaders/default_shader");
		_defaultShader->AssignShaderVertexInputAttrib("position", 0, Shader::Data::VEC3, offsetof(Vertex, position));
		_defaultShader->AssignShaderVertexInputAttrib("color", 1, Shader::Data::VEC3, offsetof(Vertex, color));
		_defaultShader->AssignShaderVertexInputAttrib("texCoord", 2, Shader::Data::VEC2, offsetof(Vertex, texCoord));
		_defaultShader->AssignShaderDescriptor("ubo", 0, Shader::Type::VERTEX, Shader::Data::UNIFORM);
		_defaultShader->AssignShaderDescriptor("texSampler", 1, Shader::Type::FRAGMENT, Shader::Data::SAMPLER2D);

		_defaultTexture = ResourceManager::Load<Texture>("../Resources/textures/default_texture.png");

		_defaultMaterial = ResourceManager::Load<Material>("../Resources/materials/default_material.mat");
		_defaultMaterial->shader = _defaultShader;
		_defaultMaterial->shaderParams["texSampler"] = _defaultTexture;

		_defaultMesh = ResourceManager::Load<Mesh>("../Resources/models/icosphere.obj");
	}

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
	SceneManager::UnloadEverything();
	ResourceManager::ClearAllResourcePools();
}
