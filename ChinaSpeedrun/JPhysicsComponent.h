#pragma once
#include "Component.h"
#include "Mathf.h"

namespace cs
{
	class JCollisionShape;

	class JPhysicsComponent : public Component
	{
	public:
		float mass;
		float gravityScale;
		Vector3 velocity, force;
		JCollisionShape* shape;

		void Init() override;
		void Enter();
		void ImGuiDrawComponent() override;
		JPhysicsComponent();
		~JPhysicsComponent();
	};
}
