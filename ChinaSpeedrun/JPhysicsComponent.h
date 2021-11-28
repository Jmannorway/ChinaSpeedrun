#pragma once
#include "Component.h"
#include "Mathf.h"

namespace cs
{
	class CollisionShape;

	class JPhysicsComponent : public Component
	{
	public:
		float mass;
		Vector3 velocity, force;
		CollisionShape* shape;
		void Init() override;
		void ImGuiDrawComponent() override;
		JPhysicsComponent();
		~JPhysicsComponent();
	};
}

