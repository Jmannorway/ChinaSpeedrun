#include "GameObject.h"
#include "Components.h"
#include "SceneManager.h"

cs::GameObject::GameObject() :
	name{ "Object" }, active{ true }, entity{ SceneManager::GetRegistry().create() }, scene{ nullptr }, parent{ nullptr }
{}

cs::GameObject::GameObject(Scene* newScene) :
	name{ "Object" }, active{ true }, entity{ newScene->registry.create() }, scene{ newScene }, parent{ nullptr }
{}

void cs::GameObject::Init()
{
	for (auto c : components)
		c->Init();
}

void cs::GameObject::EditorDrawComponents()
{
	for (auto* component : components)
		component->ImGuiDrawComponent();
}

void cs::GameObject::DestroyEntity()
{

}

void cs::GameObject::ExitTree()
{
	RemoveAllComponents();

	scene->registry.destroy(entity);
}

void cs::GameObject::QueueFree()
{
	scene->registry.destroy(entity);
}

bool cs::GameObject::HasComponentType(ComponentMeta::Type type)
{
	switch (type)
	{
	case ComponentMeta::AUDIO_COMPONENT_TYPE:			return HasComponent<AudioComponent>();
	case ComponentMeta::CAMERA_COMPONENT_TYPE:			return HasComponent<CameraComponent>();
	case ComponentMeta::MESH_RENDERER_COMPONENT_TYPE:	return HasComponent<MeshRendererComponent>();
	case ComponentMeta::PHYSICS_COMPONENT_TYPE:			return HasComponent<PhysicsComponent>();
	case ComponentMeta::TRANSFORM_COMPONENT_TYPE:		return HasComponent<TransformComponent>();

	default:
		Debug::LogWarning("HasComponentType: Cannot check for an unregistered or non-existent component");
		return false;
	}
}

cs::Component* cs::GameObject::AddComponentType(ComponentMeta::Type type)
{
	switch (type)
	{
	case ComponentMeta::AUDIO_COMPONENT_TYPE:			return &AddComponent<AudioComponent>();
	case ComponentMeta::CAMERA_COMPONENT_TYPE:			return &AddComponent<CameraComponent>();
	case ComponentMeta::MESH_RENDERER_COMPONENT_TYPE:	return &AddComponent<MeshRendererComponent>();
	case ComponentMeta::PHYSICS_COMPONENT_TYPE:			return &AddComponent<PhysicsComponent>();
	case ComponentMeta::TRANSFORM_COMPONENT_TYPE:		return &AddComponent<TransformComponent>();

	default:
		Debug::LogWarning("AddComponentType: Cannot emplace unregistered or non-existent component");
		return nullptr;
	}
}

cs::GameObject::~GameObject()
{
	DestroyEntity();
}

std::vector<cs::Component*> cs::GameObject::GetAllComponents()
{
	return components;
}

void cs::GameObject::RemoveAllComponents()
{
	DestroyEntity();
	entity = scene->registry.create();
}

cs::Scene* cs::GameObject::GetScene() const
{
	return scene;
}
