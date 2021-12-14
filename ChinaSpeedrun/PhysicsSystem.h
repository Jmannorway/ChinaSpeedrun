#pragma once
#include <vector>

#include "PhysicsListener.h"

class b2World;

namespace cs {
	class TransformComponent;
	class PhysicsComponent;
	class PhysicsListener;

	class PhysicsSystem
	{
	public:
		/**
		 * Instance of the physics world that simulates all physics bodies
		 */
		b2World* world;
		/**
		 * 
		 */
		PhysicsListener* listener;
		/**
		 * Handles queued body creations or recreations and updates
		 */
		void UpdateComponents();
		/**
		 * Updated the physics world at a fixed rate
		 */
		void UpdateWorld();
		/**
		 * Updates physics entity's transforms 
		 */
		void UpdatePositions(PhysicsComponent& pc, TransformComponent& tc);

		PhysicsSystem();
		~PhysicsSystem();

		/**
		 * Queue a physics component's body to be updated
		 */
		void QueueComponentUpdate(PhysicsComponent* pc); /*Potentially obsolete*/
		/**
		 * Queue a physics component's body to be created or recreated
		 */
		void QueueComponentCreate(PhysicsComponent* pc); /*Potentially obsolete*/
		/**
		 * Update a body
		 */
		void UpdateBody(PhysicsComponent* pc);
		/**
		 * Create or recreate physics component's body
		 */
		void CreateBody(PhysicsComponent* pc);
		/**
		 * Destroy a physics component's physics body
		 */
		void DestroyBody(PhysicsComponent* pc);
	private:
		std::vector<PhysicsComponent*> componentToUpdate; /*Potentially obsolete*/
		std::vector<PhysicsComponent*> componentToCreate; /*Potentially obsolete*/
		int velocityIterations;
		int positionIterations;
		/**
		 * Update frequency of the physics world
		 */
		float frequency;
		/**
		 * Counter to ensure that physics world is being updated at the correct rate
		 */
		float frequencyCounter;
	};
}

