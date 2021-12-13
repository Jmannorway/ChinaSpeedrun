#pragma once

#include "Component.h"

namespace cs {
	class PhysicsComponent;
	class AudioComponent;

	class PlayerComponent : public Component
	{
	public:
		void Init() override;
		void Step();
		void ImGuiDrawComponent() override;
		PlayerComponent();
	private:
		PhysicsComponent* pc;
		AudioComponent* ac;
	};
}

