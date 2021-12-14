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
		void SupplyCamera(CameraComponent* camera) { cc = camera; };
		void ImGuiDrawComponent() override;
		PlayerComponent();

		float sensitivity;
		float zoom;
	private:
		CameraComponent* cc;
		PhysicsComponent* pc;
		AudioComponent* ac;
		TransformComponent* tc;
	};
}

