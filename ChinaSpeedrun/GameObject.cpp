#include "GameObject.h"

#include "Component.h"
#include "ChinaEngine.h"
#include "World.h"

#include "MeshRenderer.h"
#include "CameraComponent.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "AudioComponent.h"
#include "SphereCollider.h"
#include "PolygonCollider.h"
#include "StaticBody.h"
#include "Rigidbody.h"
#include "Mesh.h"

#include "Debug.h"
#include "PhysicsComponent.h"

cs::GameObject::GameObject() :
	name { "Object" }, active{ true }
{}

void cs::GameObject::EditorDrawComponents()
{
	if (HasComponent<TransformComponent>())
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

void cs::GameObject::GenerateOBBExtents()
{
	obb = { Vector3(-1.0f), Vector3(1.0f) };
	return;

	// If we have a mesh, then we use that.
	if (HasComponent<MeshRendererComponent>())
	{
		Mesh* _mesh{ GetComponent<MeshRendererComponent>().mesh };

		if (_mesh == nullptr || _mesh->GetVertices().empty())
			return;

		Vector3 _maxExtent{ Vector3(0.0f) }, _minExtent{ Vector3(0.0f) };
		for (auto& vertex : _mesh->GetVertices())
		{
			_maxExtent.x = max(_maxExtent.x, vertex.position.x);
			_maxExtent.y = max(_maxExtent.y, vertex.position.y);
			_maxExtent.z = max(_maxExtent.z, vertex.position.z);

			_minExtent.x = min(_minExtent.x, vertex.position.x);
			_minExtent.y = min(_minExtent.y, vertex.position.y);
			_minExtent.z = min(_minExtent.z, vertex.position.z);
		}

		obb = { _minExtent, _maxExtent };
		return;
	}
	
	// if we only have a transform, resort to that
	if (HasComponent<TransformComponent>())
	{
		Debug::LogWarning("Cannot create OBB on ", name, " because it has no mesh. Creating default extents.");
		obb = { Vector3(-0.5f), Vector3(0.5f) };
		return;
	}
	
	// if we have none of these components, the obb is null (extents == 0.0f)
	Debug::LogIssue("Cannot generate OBB; ", name, " has no transform or mesh.");
	obb = { Vector3(0.0f), Vector3(0.0f) };
}

void cs::GameObject::ExitTree()
{
	RemoveAllComponents();

	ChinaEngine::world.registry.destroy(entity);
}

void cs::GameObject::QueueFree()
{

}

cs::GameObject::~GameObject()
{
	
}

void cs::GameObject::RemoveAllComponents()
{
	
}
