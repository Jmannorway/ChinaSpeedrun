#include "SceneManager.h"

#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"
#include "CameraBase.h"
#include "ResourceManager.h"

#include "ChinaEngine.h"
#include "Editor.h"

cs::CameraBase* cs::SceneManager::mainCamera;
std::vector<cs::SceneManager::SceneActionCombo> cs::SceneManager::queueActionsForScenes;
std::vector<cs::Scene*> cs::SceneManager::activeScenes;
uint32_t cs::SceneManager::currentScene;

cs::GameObject* cs::SceneManager::InstanceEmptyObject(const char* name)
{
	auto _scene{ GetCurrentActiveScene() };

	if (_scene == nullptr)
		return nullptr;

	GameObject* _newObject{ new GameObject };
	_newObject->name = name;
	_newObject->scene = activeScenes[currentScene];

	//ChinaEngine::editor.SetSelectedGameObject(_newObject);
	_scene->AddGameObject(_newObject);
	return _newObject;
}

cs::GameObject* cs::SceneManager::InstanceObject(const char* name, const Vector3 position, const Vector3 rotation, const Vector3 scale)
{
	if (activeScenes.empty() || activeScenes[currentScene] == nullptr)
		return nullptr;

	GameObject* _newObject{ new GameObject };
	_newObject->name = name;
	_newObject->scene = activeScenes[currentScene];

	TransformComponent& _transform{ _newObject->AddComponent<TransformComponent>() };
	_transform.position = position;
	_transform.rotationDegrees = rotation;
	_transform.scale = scale;

	//ChinaEngine::editor.SetSelectedGameObject(_newObject);
	activeScenes[currentScene]->AddGameObject(_newObject);
	return _newObject;
}

void cs::SceneManager::DestroyDescriptorPools()
{
	if (HasScenes())
		GetCurrentScene()->DestroyDescriptorPools();
}

void cs::SceneManager::CreateDescriptorPools()
{
	if (HasScenes())
		GetCurrentScene()->CreateDescriptorPools();
}

void cs::SceneManager::SetCurrentFocusedScene(const uint32_t newCurrentScene)
{
	// let's wait until the end of the frame before setting this
	currentScene = newCurrentScene;
}

void cs::SceneManager::SolveScene(Scene* scene, const cs::SceneManager::SceneAction action)
{
	queueActionsForScenes.push_back({ scene, action });
}

cs::Scene* cs::SceneManager::GetCurrentActiveScene()
{
	return activeScenes.size() > currentScene ? activeScenes[currentScene] : nullptr;
}

unsigned cs::SceneManager::GetCurrentActiveSceneNumber()
{
	return activeScenes.size();
}

void cs::SceneManager::Resolve()
{
	for (auto& sceneAction : queueActionsForScenes)
	{
		switch (sceneAction.action)
		{
		case SceneAction::NONE: // Basically the defualt case
			break;
		case SceneAction::INIT:
			sceneAction.sceneRef->Initialize();
			break;
		case SceneAction::START:
			sceneAction.sceneRef->Start();
			ResourceManager::Save<Scene>(
				sceneAction.sceneRef->GetResourcePath().empty() ?
					"../Resources/scenes/scene.txt" :
					sceneAction.sceneRef->GetResourcePath(),
				sceneAction.sceneRef);
			break;
		case SceneAction::EXIT:
			sceneAction.sceneRef->Exit();
			ChinaEngine::editor.SetSelectedGameObject(nullptr);
			break;
		case SceneAction::FREE:
			FreeScene(sceneAction.sceneRef);
			break;
		}
	}

	queueActionsForScenes.clear();
}

void cs::SceneManager::Update()
{
	if (HasScenes())
		GetCurrentScene()->Update();
}

bool cs::SceneManager::HasScenes()
{
	return !activeScenes.empty();
}

void cs::SceneManager::SendInput(int keycode, int scancode, int action, int mods)
{
	if (ChinaEngine::editor.GetPlaymodeState() == editor::EngineEditor::Playmode::PLAY && HasScenes())
		GetCurrentScene()->Input(keycode, scancode, action, mods);
}

entt::registry& cs::SceneManager::GetRegistry()
{
	return GetCurrentScene()->registry;
}

cs::Scene* cs::SceneManager::CreateScene(std::string name)
{
	Scene* _newScene{ new Scene };

	_newScene->name = name;
	// we set the resourcePath later (when saving)

	return _newScene;
}

bool cs::SceneManager::Save()
{
	if (Scene* _currentScene{ GetCurrentActiveScene() })
	{
		return ResourceManager::Save<Scene>(_currentScene->resourcePath, _currentScene);
	}

	return false;
}

void cs::SceneManager::Load()
{
	auto* _scene = ResourceManager::Load<Scene>();

	if (_scene == nullptr)
		return;

	Load(_scene);
}

void cs::SceneManager::Load(Scene* scene)
{
	SolveScene(scene, SceneAction::INIT);

	activeScenes.push_back(scene);
}

void cs::SceneManager::Reload(Scene* scene)
{
	auto _filename = scene->GetResourcePath();

	Unload(scene);

	Load(ResourceManager::LoadScene(_filename));
}

void cs::SceneManager::Unload(Scene* scene)
{
	// unload stuff here

	SolveScene(scene, SceneAction::FREE);
}

void cs::SceneManager::DestroyEverything()
{
	for (auto* scene : activeScenes)
		scene->Free();
}

cs::Scene* cs::SceneManager::GetCurrentScene()
{
	return activeScenes.empty() ? nullptr : activeScenes[currentScene];
}

void cs::SceneManager::FreeScene(Scene* scene)
{
	auto _it{ std::find(activeScenes.begin(), activeScenes.end(), scene) };

	if (_it == activeScenes.end())
	{
		Debug::LogWarning("Cannot unload this scene. It is not initialized.");
		return;
	}

	activeScenes.erase(_it);
	ChinaEngine::editor.SetSelectedGameObject(nullptr);

	Mathf::Clamp(currentScene, (uint32_t)0, (uint32_t)activeScenes.size() - 1);
	scene->Free();
}

void cs::SceneManager::DrawScenes()
{
	if (ImGui::Button("Create"))
	{
		// prompt the user

		//SceneManager::CreateScene("");
	}

	ImGui::SameLine();

	if (ImGui::Button("Save"))
	{
		// prompt the user
	}

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		// prompt the user
	}

	for (size_t i{ 0 }; i < activeScenes.size(); i++)
		if (activeScenes[i]->ImGuiDrawGameObjects())
			SetCurrentFocusedScene(i);
}
