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
#include "JCollisionShape.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "SceneUtility.h"

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
                // TODO: Loading meshes fucks up the scene
                /*if (SceneManager::GetCurrentScene()->GetResourcePath().empty())
                    if (!SceneManager::Save())
                        break;*/

                editorRoot->SetPlaymode(EngineEditor::Playmode::PLAY);
            }
            break;
        case EngineEditor::Playmode::PLAY:
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
        if (ImGui::Button("Load Scene 1"))
        {
            SceneManager::Load(SceneManager::CreateScene("Triangle sphere 1st - " + std::to_string(SceneManager::GetCurrentActiveSceneNumber())));
            SceneManager::SetCurrentFocusedScene(SceneManager::GetCurrentActiveSceneNumber() - 1);
            GameObject* _obj;

            _obj = SceneManager::InstanceObject(
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
                "Rolling ball",
                Vector3(-0.75f, 2.f, 0.3f),
                Vector3(0.0f),
                Vector3(0.4f));
            auto& _mrc2 = _obj->AddComponent<MeshRendererComponent>();
            _mrc2.SetMesh(ResourceManager::Load<Mesh>("../Resources/models/icosphere.obj"));
            _mrc2.material = ResourceManager::GetDefaultMaterial();
            auto& _jpc2 = _obj->AddComponent<JPhysicsComponent>();
            _jpc2.shape = new JCollisionSphere(0.4f);
            _jpc2.gravityScale = 0.25f;
        }

        ImGui::SameLine();
        if (ImGui::Button("Load Scene 2"))
        {
            SceneManager::Load(SceneManager::CreateScene("Triangulated map data - " + std::to_string(SceneManager::GetCurrentActiveSceneNumber())));
            SceneManager::SetCurrentFocusedScene(SceneManager::GetCurrentActiveSceneNumber() - 1);
            GameObject* _obj;

            _obj = SceneManager::InstanceObject(
                "Object",
                Vector3(9.7f, 4.f, 5.8f),
                Vector3(-26.f, 45.2f, 0.f));
            CameraComponent& _cc = _obj->AddComponent<CameraComponent>();

            _obj = SceneManager::InstanceObject("Triangulated map mesh", Vector3(-230.f, -47.f, -266.f));
            auto& _mrc = _obj->AddComponent<MeshRendererComponent>();
            _mrc.SetMesh(ResourceManager::LoadModelFromMapData("../Resources/test_las.txt"));
            _mrc.material = ResourceManager::GetDefaultMaterial();
        }

        ImGui::SameLine();
        if (ImGui::Button("Load Scene 3")) // Scene containing triangulated map data, triangle colliders and a sphere collider
        {
            SceneManager::Load(SceneManager::CreateScene("Map data collision - " + std::to_string(SceneManager::GetCurrentActiveSceneNumber())));
            SceneManager::SetCurrentFocusedScene(SceneManager::GetCurrentActiveSceneNumber() - 1);
            GameObject* _obj;

            _obj = SceneManager::InstanceObject(
                "Camera",
                Vector3(3.1f, 13.4f, -21.7f),
                Vector3(-25.f, -169.8f, 0.f));
            _obj->AddComponent<CameraComponent>();

            _obj = SceneManager::InstanceObject("Map for collision", Vector3(-205.6f, -24.5f, -236.7f));
            auto& _mrc = _obj->AddComponent<MeshRendererComponent>();
            _mrc.SetMesh(ResourceManager::LoadModelFromMapData("../Resources/test_las.txt", 25000, 8, 8));
            _mrc.material = ResourceManager::GetDefaultMaterial();
            SceneUtility::CreateStaticTriangleCollidersFromMesh(_mrc.mesh, _obj->GetComponent<TransformComponent>().position);

            _obj = SceneManager::InstanceObject(
                "Rolling ball",
                Vector3(6.9f, 7.6f, -11.2f),
                Vector3(0.0f),
                Vector3(0.4f));
            auto& _mrc2 = _obj->AddComponent<MeshRendererComponent>();
            _mrc2.SetMesh(ResourceManager::Load<Mesh>("../Resources/models/icosphere.obj"));
            _mrc2.material = ResourceManager::GetDefaultMaterial();
            auto& _jpc2 = _obj->AddComponent<JPhysicsComponent>();
            _jpc2.shape = new JCollisionSphere(0.4f);
            _jpc2.gravityScale = 0.25f;
        }

        ImGui::SameLine();

        // TODO: Move to inspector
        if (activeObject && ImGui::BeginCombo("", "Add Component", ImGuiComboFlags_NoArrowButton))
        {
            for (unsigned i = 0; i < ComponentMeta::GetComponentTypeMax(); i++)
            {
                const auto _type = static_cast<ComponentMeta::Type>(i);

                // TODO: Components that need default materials or other initialization outside of Init() will be left unchecked
                if (ImGui::Selectable(ComponentMeta::TypeToName(_type).c_str()) && !activeObject->HasComponentType(_type))
                {
                    auto _c = activeObject->AddComponentType(_type);

                    switch (_type)
                    {
                    case ComponentMeta::MESH_RENDERER_COMPONENT_TYPE:
	                    {
                        auto _mrc = static_cast<MeshRendererComponent*>(_c);
                        _mrc->SetMesh(ResourceManager::GetFirstMesh());
                        _mrc->material = ResourceManager::GetFirstMaterial();
	                    }
                    }
                }
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
        // TODO: Reload scene
    	//SceneManager::Reload(SceneManager::GetCurrentActiveScene());
    }
}

void cs::editor::ImGuiLayer::IsWindowHovered()
{
    isWindowActive |= ImGui::IsWindowHovered(
        ImGuiHoveredFlags_AllowWhenBlockedByActiveItem |
		ImGuiHoveredFlags_RootAndChildWindows);
}
