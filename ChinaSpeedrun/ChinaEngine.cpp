#include "ChinaEngine.h"

#include "AudioComponent.h"
#include "ResourceManager.h"

#include "Vertex.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "MeshRenderer.h"
#include "VulkanEngineRenderer.h"

#include "ImGuiLayer.h"

#include "Input.h"
#include "Transform.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "Camera.h"
#include "PhysicsComponent.h"

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
	SceneManager::Load(SceneManager::CreateScene("Scene"));

	Shader* _shader{ ResourceManager::Load<Shader>("../Resources/shaders/default_shader") };
	{
		_shader->AssignShaderVertexInputAttrib("position", 0, Shader::Data::VEC3, offsetof(Vertex, position));
		_shader->AssignShaderVertexInputAttrib("color", 1, Shader::Data::VEC3, offsetof(Vertex, color));
		_shader->AssignShaderVertexInputAttrib("texCoord", 2, Shader::Data::VEC2, offsetof(Vertex, texCoord));
		_shader->AssignShaderDescriptor("ubo", 0, Shader::Type::VERTEX, Shader::Data::UNIFORM);
		_shader->AssignShaderDescriptor("texSampler", 1, Shader::Type::FRAGMENT, Shader::Data::SAMPLER2D);
	}

	Texture* _junkoGyate{ ResourceManager::Load<Texture>("../Resources/textures/junko_gyate.png") };

	Material* _material1{ ResourceManager::Load<Material>("../Resources/materials/test1.mat") };

	_material1->shader = _shader;
	_material1->shaderParams["texSampler"] = _junkoGyate;

	GameObject* _obj{ SceneManager::InstanceObject("Ball", Vector3(1.f)) };
	MeshRendererComponent& _sphereMeshComponent{ _obj->AddComponent<MeshRendererComponent>() };
	_sphereMeshComponent.SetMesh(ResourceManager::Load<Mesh>("../resources/models/icosphere.obj"));
	_sphereMeshComponent.material = _material1;
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
