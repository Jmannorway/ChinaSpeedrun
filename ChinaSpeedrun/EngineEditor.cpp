#include "Editor.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "ChinaEngine.h"
#include "World.h"
#include "Input.h"
#include "Camera.h"

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
		ChinaEngine::world.mainCamera = editorCamera;
		ChinaEngine::world.Stop();
		break;
	case cs::editor::EngineEditor::Playmode::PLAY:
		ChinaEngine::world.Start();
		break;
	case cs::editor::EngineEditor::Playmode::PAUSE:
		ChinaEngine::world.Stop();
		break;
	}
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
	Input::AddMapping("editor_save_scene", GLFW_KEY_9);
	Input::AddMapping("editor_load_scene", GLFW_KEY_0);

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

	if (Input::GetActionPressed("editor_save_scene"))
		SceneManager::Save();

	if (Input::GetActionPressed("editor_load_scene"))
		SceneManager::Load("../resources/scenes/china.txt");

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

	delete editorCamera;
	delete uiLayer;
}
