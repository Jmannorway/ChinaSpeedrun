#pragma once

#include "Component.h"

namespace cs {
	class TransformComponent;
	class CameraComponent;
	class PhysicsComponent;
	class AudioComponent;

	class PlayerComponent : public Component
	{
	public:
		void Init() override;
		void Step();
		void ImGuiDrawComponent() override;
		PlayerComponent();

		float sensitivity;
		CameraComponent* camera;
	private:
		PhysicsComponent* pc;
		AudioComponent* ac;
		TransformComponent* tc;
	};
}

