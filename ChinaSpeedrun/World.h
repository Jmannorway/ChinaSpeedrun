#pragma once

#include <entt/entt.hpp>
#include "Mathf.h"

namespace cs
{
	class World
	{
	public:
		friend class GameObject;
		friend class VulkanEngineRenderer;

		class AudioSystem* audioSystem;
		//class Rednerer* rendererSystem;
		//class MeshRenderer* meshRendererSystem;
		
		World();
		
		class GameObject* InstanceObject(const char* name, const Vector3 position = Vector3(0.0f, 0.0f, 0.0f), const Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f), const Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));
		
		// Start all the components
		void Start();
		void Step();

		// Very dangrous to call yourself... Don't do it!
		void DeleteAllObjects();

		const uint64_t GetUBONextOffset() const;

		const std::vector<GameObject*>& GetObjects();

	private:
		entt::registry registry;
		std::vector<GameObject*> objects;
	};
}