#include "Editor.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "ImGuizmo.h"

#include <glm/gtx/quaternion.hpp>
#include "Mathf.h"
#include "GameObject.h"
#include "Time.h"

#include "Script.h"
#include "Transform.h"
#include "VulkanEngineRenderer.h"
#include "ChinaEngine.h"
#include "SceneManager.h"
#include "Camera.h"

cs::editor::ImGuiLayer::ImGuiLayer(EngineEditor* root) :
    editorRoot{ root }, activeObject{ nullptr }, isManipulating{ false }, isWindowActive{ false }
{}

void cs::editor::ImGuiLayer::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& _io{ ImGui::GetIO() }; (void)_io;
}

void cs::editor::ImGuiLayer::Begin()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void cs::editor::ImGuiLayer::Step()
{
    bool _gizmoWindow{ true };

    isWindowActive = false;

    if (ImGui::Begin("Gizmos", &_gizmoWindow, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking))
    {
        int _width, _height;
        ChinaEngine::renderer.GetViewportSize(_width, _height);

        ImGui::SetWindowPos({ 0.0f, 0.0f });
        ImGui::SetWindowSize({ static_cast<float>(_width), static_cast<float>(_height) });

        if (activeObject != nullptr && activeObject->HasComponent<TransformComponent>())
        {
            TransformComponent& _transform{ activeObject->GetComponent<TransformComponent>() };
            Matrix4x4 _viewMatrix{ Camera::GetViewMatrix(*SceneManager::mainCamera) }, _projectionMatrix{ Camera::GetProjectionMatrix(*SceneManager::mainCamera) };
            Matrix4x4& _transformMatrix{ Transform::GetMatrixTransform(_transform) };

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(0.0f, 0.0f, static_cast<float>(_width), static_cast<float>(_height));

            _projectionMatrix[1][1] *= -1.0f;

            ImGuizmo::Manipulate(glm::value_ptr(_viewMatrix), glm::value_ptr(_projectionMatrix),
                editorRoot->GetOperationState(), editorRoot->GetMode(), glm::value_ptr(_transformMatrix));

            isManipulating = ImGuizmo::IsUsing();

            if (isManipulating)
            {
                Vector3 _position{}, _rotation{}, _scale{};
                //Mathf::DecomposeMatrix(_transformMatrix, _position, _rotation, _scale); // This doesn't work...

                ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(_transformMatrix), &_position[0], &_rotation[0], &_scale[0]);

                _transform.position = _position;
                _transform.rotationDegrees = _rotation;
                _transform.scale = _scale;
            }
        }
    }
    ImGui::End();

    if (ImGui::Begin("Hierarchy"))
    {
        SceneManager::DrawScenes();
        IsWindowHovered();
    }
    ImGui::End();

	if (ImGui::Begin("Inspector"))
	{
		if (activeObject != nullptr)
		{
            ImGui::Checkbox("", &activeObject->active);
            ImGui::SameLine();
			ImGui::Text(activeObject->name.c_str());

            activeObject->EditorDrawComponents();
		}

        IsWindowHovered();
	}
	ImGui::End();

    if (ImGui::Begin("Top Bar"))
    {
        switch (editorRoot->GetPlaymodeState())
        {
        case EngineEditor::Playmode::EDITOR:
            if (ImGui::Button("Play"))
            {

                if (SceneManager::GetCurrentScene()->GetResourcePath().empty())
                    if (!SceneManager::Save())
                        break;

                editorRoot->SetPlaymode(EngineEditor::Playmode::PLAY);
            }
            break;
        case editor::EngineEditor::Playmode::PLAY:
            if (ImGui::Button("Pause"))
            {
                editorRoot->SetPlaymode(EngineEditor::Playmode::PAUSE);
            }

            DrawStopSimulationButton();
            break;
        case EngineEditor::Playmode::PAUSE:
            if (ImGui::Button("Continue"))
            {
                editorRoot->SetPlaymode(EngineEditor::Playmode::PLAY);
            }

            DrawStopSimulationButton();
            break;
        }

        ImGui::SameLine();
        ImGui::Button("New Scene");
        if (ImGui::IsItemClicked())
        {
            SceneManager::Load(SceneManager::CreateScene(
                "New scene " + std::to_string(SceneManager::GetCurrentActiveSceneNumber())));
        }

        ImGui::SameLine();
        ImGui::Button("Save Scene");
        if (ImGui::IsItemClicked())
        {
            SceneManager::Save();
        }

        ImGui::SameLine();
        ImGui::Button("Load Scene");
        if (ImGui::IsItemClicked())
        {
            SceneManager::Load();
        }

        ImGui::SameLine();
        ImGui::Button("Add Entity");
        if (ImGui::IsItemClicked())
        {
            std::string _objectName = "New Entity " + std::to_string(SceneManager::GetCurrentActiveScene()->GetObjectCount());
            SceneManager::InstanceObject(_objectName.c_str());
        }

        // TODO: Move to inspector (there was a bug that didn't let me open the combo last time I attempted this)
        ImGui::SameLine();
        if (activeObject && ImGui::BeginCombo("", "Add Component", ImGuiComboFlags_NoArrowButton))
        {
            for (unsigned i = 0; i < ComponentMeta::GetComponentTypeMax(); i++)
            {
                auto _type = static_cast<ComponentMeta::Type>(i);

                ImGui::Selectable(
                    ComponentMeta::TypeToName(_type).c_str());

                // TODO: Components that need default materials or other initialization outside of Init() will be left unchecked
                if (ImGui::IsItemClicked() && !activeObject->HasComponentType(_type))
                    activeObject->AddComponentType(_type);
            }

            ImGui::EndCombo();
        }

        IsWindowHovered();
    }
    ImGui::End();

    if (ImGui::Begin("Profiler"))
    {
        ImGui::Text("Delta Time: %f", Time::deltaTime);

        const auto& _status{ ChinaEngine::renderer.GetStatus() };

        ImGui::Text("Index Buffer");
        ImGui::SameLine();
        ImGui::ProgressBar(_status.indexDataFractionSize);
        ImGui::Text("%f kB", (float)(*_status.indexDataSize) * 0.0001f);

        ImGui::Text("Vertex Buffer");
        ImGui::SameLine();
        ImGui::ProgressBar(_status.vertexDataFractionSize);
        ImGui::Text("%f kB", (float)(*_status.vertexDataSize) * 0.0001f);

        IsWindowHovered();
    }
	ImGui::End();

    if (activeObject != nullptr && activeObject->HasComponent<ScriptComponent>())
    {
        ScriptComponent& _script{ activeObject->GetComponent<ScriptComponent>() };

        if (ImGui::Begin("Scripting"))
        {
            char* _text{ _script.GetScript()->scriptText.data() };
            ImGui::InputTextMultiline("", _text, INT16_MAX, ImVec2(ImGui::GetWindowSize().x - 12.0f, ImGui::GetWindowSize().y - 35.0f), ImGuiInputTextFlags_AllowTabInput);
            _script.GetScript()->scriptText = _text;

            if (ImGui::IsItemDeactivatedAfterEdit())
                _script.CompileScript();

            IsWindowHovered();
        }
        ImGui::End();
    }

    if (ImGui::Begin("Debugger"))
    {
        Debug::ImGuiDrawMessages();
        IsWindowHovered();
    }
    ImGui::End();
}

void cs::editor::ImGuiLayer::End()
{
    ImGui::Render();
}

void cs::editor::ImGuiLayer::SetStyle()
{
    ImGuiStyle* _style{ &ImGui::GetStyle() };
}

const bool& cs::editor::ImGuiLayer::IsManipulating() const
{
    return isManipulating;
}

const bool& cs::editor::ImGuiLayer::IsInteractingWithWindow() const
{
    return isWindowActive;
}

void cs::editor::ImGuiLayer::DrawStopSimulationButton()
{
    ImGui::SameLine();
    ImGui::Button("Stop");
    if (ImGui::IsItemClicked())
    {
        editorRoot->SetPlaymode(EngineEditor::Playmode::EDITOR);
        SceneManager::GetCurrentScene()->Exit();
        SceneManager::Reload(SceneManager::GetCurrentActiveScene());
    }
}

void cs::editor::ImGuiLayer::IsWindowHovered()
{
    isWindowActive |= ImGui::IsWindowHovered(
        ImGuiHoveredFlags_AllowWhenBlockedByActiveItem |
		ImGuiHoveredFlags_RootAndChildWindows);
}
