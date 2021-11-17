#include "Editor.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "ChinaEngine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Input.h"
#include "Camera.h"
#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Scene.h"

const ImGuizmo::OPERATION& cs::editor::EngineEditor::GetOperationState()
{
	return operation;
}

const ImGuizmo::MODE& cs::editor::EngineEditor::GetMode()
{
	return operationMode;
}

const cs::editor::EngineEditor::Playmode& cs::editor::EngineEditor::GetPlaymodeState()
{
	return mode;
}

void cs::editor::EngineEditor::SetPlaymode(const Playmode newPlaymode)
{
	mode = newPlaymode;

	switch (mode)
	{
	case cs::editor::EngineEditor::Playmode::EDITOR:
		SceneManager::mainCamera = editorCamera;
		break;
	case cs::editor::EngineEditor::Playmode::PLAY:
		break;
	case cs::editor::EngineEditor::Playmode::PAUSE:
		
		break;
	}
}

cs::GameObject* cs::editor::EngineEditor::GetSelectedGameObject() const
{
	return uiLayer->activeObject;
}

void cs::editor::EngineEditor::SetSelectedGameObject(GameObject* gameObject)
{
	uiLayer->activeObject = gameObject;
}

void cs::editor::EngineEditor::Start()
{
	Input::AddMapping("editor_forward", GLFW_KEY_W);
	Input::AddMapping("editor_backward", GLFW_KEY_S);
	Input::AddMapping("editor_left", GLFW_KEY_A);
	Input::AddMapping("editor_right", GLFW_KEY_D);
	Input::AddMapping("editor_up", GLFW_KEY_Q);
	Input::AddMapping("editor_down", GLFW_KEY_E);

	Input::AddMapping("editor_translate", GLFW_KEY_W);
	Input::AddMapping("editor_rotate", GLFW_KEY_E);
	Input::AddMapping("editor_scale", GLFW_KEY_R);
	Input::AddMapping("editor_mode_switch", GLFW_KEY_LEFT_CONTROL);
	Input::AddMapping("editor_snap", GLFW_KEY_LEFT_SHIFT);
	Input::AddMapping("editor_save_scene", GLFW_KEY_F10);
	Input::AddMapping("editor_load_scene", GLFW_KEY_F11);
	Input::AddMapping("editor_new_scene", GLFW_KEY_F2);
	Input::AddMapping("editor_new_entity", GLFW_KEY_F3);
	Input::AddMapping("editor_add_transform", GLFW_KEY_F4);
	Input::AddMapping("editor_add_mesh_renderer", GLFW_KEY_F5);
	Input::AddMapping("editor_add_camera", GLFW_KEY_F6);
	Input::AddMapping("editor_add_physics", GLFW_KEY_F7);
	Input::AddMapping("editor_add_audio", GLFW_KEY_F8);

	editorCamera = new EditorCamera(this);
	Camera::CalculatePerspective(*editorCamera);

	uiLayer = new ImGuiLayer(this);

	SetPlaymode(Playmode::EDITOR);
	operation = ImGuizmo::TRANSLATE;
	operationMode = ImGuizmo::LOCAL;
}

void cs::editor::EngineEditor::Update()
{
	if (Input::GetActionPressed("editor_mode_switch"))
		operationMode = operationMode == ImGuizmo::WORLD ? ImGuizmo::LOCAL : ImGuizmo::WORLD;

	// by moving the ui layer (editor ui) here, we have more control over when things are executed
	uiLayer->Begin();
	uiLayer->Step();

	// TODO: Treat loading and saving paths properly
	if (Input::GetActionPressed("editor_save_scene"))
	{
		if (auto _scene{SceneManager::GetCurrentActiveScene()})
			ResourceManager::Save<Scene>("../resources/scenes/scene.txt", _scene);
	}

	if (Input::GetActionPressed("editor_load_scene"))
	{
		if (auto _scene{ ResourceManager::Load<Scene>("../resources/scenes/scene.txt") })
			SceneManager::Load(_scene);
	}

	if (Input::GetActionPressed("editor_new_scene"))
	{
		SceneManager::Load(SceneManager::CreateScene("New scene " + std::to_string(SceneManager::GetCurrentActiveSceneNumber())));
	}
	
	if (Input::GetActionPressed("editor_new_entity"))
	{
		SceneManager::GetCurrentActiveScene()->AddGameObject();
	}

	if (Input::GetActionPressed("editor_add_transform"))
	{
		GameObject* _obj{ GetSelectedGameObject() };
		if (_obj && !_obj->HasComponent<TransformComponent>())
		{
			_obj->AddComponentType(Component::TRANSFORM_COMPONENT_TYPE);
		}
	}

	if (Input::GetActionPressed("editor_add_mesh_renderer"))
	{
		GameObject* _obj{ GetSelectedGameObject() };
		if (_obj && !_obj->HasComponent<MeshRendererComponent>())
		{
			auto _c = (MeshRendererComponent*)_obj->AddComponentType(Component::MESH_RENDERER_COMPONENT_TYPE);
			_c->material = ResourceManager::Load<Material>("../Resources/materials/default_material.mat");
			_c->SetMesh(Mesh::CreateDefaultCube());
		}
	}

	if (Input::GetActionPressed("editor_add_camera"))
	{
		GameObject* _obj{ GetSelectedGameObject() };
		if (_obj && !_obj->HasComponent<CameraComponent>())
		{
			_obj->AddComponentType(Component::CAMERA_COMPONENT_TYPE);
		}
	}

	if (Input::GetActionPressed("editor_add_physics"))
	{
		GameObject* _obj{ GetSelectedGameObject() };
		if (_obj && !_obj->HasComponent<PhysicsComponent>())
		{
			_obj->AddComponentType(Component::PHYSICS_COMPONENT_TYPE);
		}
	}

	if (Input::GetActionPressed("editor_add_audio"))
	{
		GameObject* _obj{ GetSelectedGameObject() };
		if (_obj && !_obj->HasComponent<AudioComponent>())
		{
			_obj->AddComponentType(Component::AUDIO_COMPONENT_TYPE);
		}
	}

	if (mode == Playmode::EDITOR)
	{
		editorCamera->Update();

		if (!Input::GetMouseHeld(1))
		{
			if (Input::GetActionPressed("editor_translate"))
				operation = ImGuizmo::TRANSLATE;
			else if (Input::GetActionPressed("editor_rotate"))
				operation = ImGuizmo::ROTATE;
			else if (Input::GetActionPressed("editor_scale"))
				operation = ImGuizmo::SCALE;
		}
	}

	uiLayer->End();
}

void cs::editor::EngineEditor::Exit()
{
	// might be useful to remove mappings when disableing the editor
	Input::RemoveMapping("editor_forward");
	Input::RemoveMapping("editor_backward");
	Input::RemoveMapping("editor_left");
	Input::RemoveMapping("editor_right");
	Input::RemoveMapping("editor_up");
	Input::RemoveMapping("editor_down");

	Input::RemoveMapping("editor_translate");
	Input::RemoveMapping("editor_rotate");
	Input::RemoveMapping("editor_scale");
	Input::RemoveMapping("editor_mode_switch");
	Input::RemoveMapping("editor_snap");
	Input::RemoveMapping("editor_save_scene");
	Input::RemoveMapping("editor_load_scene");
	Input::RemoveMapping("editor_new_scene");
	Input::RemoveMapping("editor_new_entity");

	delete editorCamera;
	delete uiLayer;
}
