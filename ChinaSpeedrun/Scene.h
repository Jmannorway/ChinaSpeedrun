#pragma once

#include "Resource.h"

#include "entt/entt.hpp"

#include <vector>
#include <string>

namespace cs
{
	class PhysicsSystem;
	class GameObject;
	class SceneManager;
	class VulkanEngineRenderer;
	class ResourceManager;

	// Scenes can also be treated as prefabs
	class Scene : public Resource
	{
	public:
		friend SceneManager;
		friend GameObject;
		friend VulkanEngineRenderer;
		friend ResourceManager;

		Scene();
		~Scene();

		// Initialize the components
		void Initialize() override;
		// Start the components
		void Start();
		// Update all of the components
		void Update();
		// Exits the scene and destroys it's contents.
		// Usually you don't call this yourself, for safety reasons.
		void Exit();
		void Free();

		// Adds a game object
		GameObject* AddGameObject(GameObject* newObject = nullptr);
		// Removes a selective object from the entire scene
		void RemoveGameObject();
		// Gets the number of objects in the scene
		unsigned GetObjectCount() const;
		// Clears the scene's contents
		void ClearScene();
		// Queues this scene for deletion
		void QueueExit();

		class PhysicsServer* GetPhysicsServer() const;
		class PhysicsSystem* GetPhysicsSystem() const;
		class AudioSystem* GetAudioSystem() const;
		std::string GetName() const;

		void AddToRenderQueue(class RenderComponent* renderer);
		void RemoveFromRenderQueue(RenderComponent* renderer);

		uint32_t GetUBOOffset();

	private:
		void DestroyDescriptorPools();
		void CreateDescriptorPools();
		void Input(int keycode, int scancode, int action, int mods);

		bool ImGuiDrawGameObjects();
		void UpdateEditorComponents();
		void UpdateComponents();

		PhysicsServer* physicsServer;
		AudioSystem* audioSystem;
		PhysicsSystem* physicsSystem;

		entt::registry registry;
		std::string name;
		std::vector<GameObject*> gameObjects;
		std::vector<RenderComponent*> renderableObjects;
	};
}
