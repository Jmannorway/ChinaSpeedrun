#pragma once
#include "Component.h"

namespace cs
{
	class BSODComponent : public Component
	{
	public:
		BSODComponent();

		/**
		 * Throws a fatal error using the windows API
		 */
		void Die();

		void Init() override {}
		void ImGuiDrawComponent() override;
	};
}
