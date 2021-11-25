#pragma once
#include "Component.h"

namespace cs
{
	class BSODComponent : public Component
	{
	public:
		BSODComponent();
		void Init() override;
		void Die();
		void ImGuiDrawComponent() override;
	};
}
