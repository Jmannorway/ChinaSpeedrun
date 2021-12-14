#pragma once

#include <b2/b2_body.h>

#include "Component.h"

#include "CollisionShape.h"
#include "Mathf.h"

namespace cs
{
	/**
	 * Holding information about how much a physics component has moved
	 * in a frame
	 */
	struct PhysicsDelta
	{
		Vector2 GetPositionDifference() const { return position - previousPosition; }
		float GetAngleDifference() const { return angle - previousAngle; }
		/**
		 * Used when moving a physics entity
		 */
		void Move(const Vector2& newPosition, float newAngle);
		/**
		 * Used when teleporting a physics entity (does not affect position difference)
		 */
		void Teleport(const Vector2& newPosition, float newAngle);
		PhysicsDelta();
	private:
		Vector2 position;
		Vector2 previousPosition;
		float angle;
		float previousAngle;
	};

	class PhysicsComponent : public Component
	{
	public:
		/**
		 * Definition of a physics body before it has been created
		 * by the physics system
		 */
		b2BodyDef definition;
		/**
		 * Pointer to the physics body created by the physics system
		 * using box2d
		 */
		b2Body* body;
		/**
		 * Struct holding information about how the physics body has moved
		 * so that the transform component can be updated properly
		 */
		PhysicsDelta delta;
		/**
		 * Definition of the shape that will be created alongside the body
		 */
		CollisionShapeDefinition shapeDefinition;
		/**
		 * Shape of the physics body
		 */
		b2Shape* shape;
		/**
		 * When the body definition has been updated this can be called
		 * to tell the system to apply the changes
		 */
		void QueueForUpdate();
		/**
		 * Function to tell the physics system to create / recreate the
		 * physics body
		 */
		void QueueForCreation();

		void Init() override {}
		void EnterScene() override;
		void ExitScene() override;
		/**
		 * Adds force to the physics body's center
		 */
		void AddForce(const Vector2& impulse);
		/**
		 * Updates the body's fixture (properties like friction in shape)
		 */
		void UpdateFixtures();
		/**
		 * Destroys the body's fixture
		 */
		void DeleteFixtures();
		/**
		 * Creates a shape based on the shape definition
		 */
		void UpdateShape();

		PhysicsComponent();
		~PhysicsComponent();

		void ImGuiDrawComponent() override;

	private:
		/**
		 * Destroy's the component's body
		 */
		void DestroyBody();
		/**
		 * Creates a default shape for the body
		 */
		b2Shape* CreateDefaultShape() const;
		/**
		 * Creates a default fixture definition
		 */
		b2FixtureDef CreateDefaultFixtureDefinition() const;
	};
}
