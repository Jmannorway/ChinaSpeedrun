#pragma once

#include "PhysicsBody.h"

namespace cs
{
	class StaticBody : public PhysicsBody
	{

	};

	class StaticBodyComponent : public PhysicsBodyComponent
	{
	public:
		friend StaticBody;

		StaticBodyComponent();

		virtual void ImGuiDrawComponent() override;
	};
}
