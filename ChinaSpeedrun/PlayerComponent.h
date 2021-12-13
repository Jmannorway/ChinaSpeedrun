#pragma once

#include "Component.h"

namespace cs {
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

		CameraComponent* camera;
	private:
		PhysicsComponent* pc;
		AudioComponent* ac;
	};
}

