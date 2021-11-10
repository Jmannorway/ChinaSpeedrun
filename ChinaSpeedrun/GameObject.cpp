#include "GameObject.h"

#include "Component.h"
#include "ChinaEngine.h"
#include "Mathf.h"

#include "MeshRenderer.h"
#include "CameraComponent.h"
#include "Transform.h"
#include "AudioComponent.h"
#include "PhysicsComponent.h"

#include "SceneManager.h"

cs::GameObject::GameObject() :
	name{ "Object" }, active{ true }, entity{ SceneManager::GetRegistry().create() }, parent{ nullptr }
{}

void cs::GameObject::EditorDrawComponents()
{
	for (auto* component : components)
		component->ImGuiDrawComponent();

	/*if (HasComponent<TransformComponent>())
		GetComponent<TransformComponent>().ImGuiDrawComponent();
	if (HasComponent<MeshRendererComponent>())
		GetComponent<MeshRendererComponent>().ImGuiDrawComponent();
	if (HasComponent<CameraComponent>())
		GetComponent<CameraComponent>().ImGuiDrawComponent();
	if (HasComponent<AudioComponent>())
		GetComponent<AudioComponent>().ImGuiDrawComponent();

	// Physics
	if (HasComponent<PhysicsComponent>())
		GetComponent<PhysicsComponent>().ImGuiDrawComponent();
	/*if (HasComponent<StaticBodyComponent>())
		GetComponent<StaticBodyComponent>().ImGuiDrawComponent();
	if (HasComponent<RigidbodyComponent>())
		GetComponent<RigidbodyComponent>().ImGuiDrawComponent();
	if (HasComponent<SphereColliderComponent>())
		GetComponent<SphereColliderComponent>().ImGuiDrawComponent();
	if (HasComponent<PolygonColliderComponent>())
		GetComponent<PolygonColliderComponent>().ImGuiDrawComponent();*/
}

void cs::GameObject::ExitTree()
{
	RemoveAllComponents();

	scene->registry.destroy(entity);
}

void cs::GameObject::QueueFree()
{

}

cs::Component* cs::GameObject::AddComponentType(Component::Type type)
{
	Component* _component;

	switch (type)
	{
	case Component::AUDIO_COMPONENT_TYPE:			_component = &scene->registry.emplace<AudioComponent>(entity);			break;
	case Component::CAMERA_COMPONENT_TYPE:			_component = &scene->registry.emplace<CameraComponent>(entity);			break;
	case Component::MESH_RENDERER_COMPONENT_TYPE:	_component = &scene->registry.emplace<MeshRendererComponent>(entity);	break;
	case Component::PHYSICS_COMPONENT_TYPE:			_component = &scene->registry.emplace<PhysicsComponent>(entity);		break;
	case Component::TRANSFORM_COMPONENT_TYPE:		_component = &scene->registry.emplace<TransformComponent>(entity);		break;

	default:
		Debug::LogWarning("AddComponentType: Trying to emplace unregistered or non-existent component");
		return nullptr;
	}

	components.push_back(_component);
	_component->gameObject = this;
	_component->Init();
}

cs::GameObject::~GameObject()
{

}

std::vector<cs::Component*> cs::GameObject::GetAllComponents()
{
	return components;
}

void cs::GameObject::RemoveAllComponents()
{
	scene->registry.destroy(entity);
	entity = scene->registry.create();
}

cs::Scene* cs::GameObject::GetScene() const
{
	return scene;
}
