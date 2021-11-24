#pragma once
#include <string>

#include "ComponentMeta.h"

namespace cs
{
	class Component
	{
	public:
		// TODO: Figure out if there is a better way to register components
		

		class GameObject* gameObject{ nullptr };
		bool enabled{ true };

		virtual void Init() = 0;
		virtual void ImGuiDrawComponent() = 0;

		ComponentMeta::Type GetType() const;
		std::string GetTypeName() const;

		Component();
	protected:
		ComponentMeta::Type type;
	};
}
