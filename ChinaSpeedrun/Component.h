#pragma once
#include <string>

namespace cs
{
	class Component
	{
	public:
		enum class Type
		{
			Transform,
			Physics,
			Camera,
			Audio,
			MeshRenderer
		};

		class GameObject* gameObject{ nullptr };
		bool enabled{ true };
		virtual void ImGuiDrawComponent() = 0;

		Type GetType() const;
		std::string GetTypeName() const;
		static std::string TypeToName(Type type);
	protected:
		Type type;
	};
}
