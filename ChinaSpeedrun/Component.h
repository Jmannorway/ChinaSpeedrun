#pragma once
#include <string>

namespace cs
{
	class Component
	{
	public:
		// TODO: Figure out if there is a better way to register components
		enum Type : unsigned
		{
			AUDIO_COMPONENT_TYPE,
			CAMERA_COMPONENT_TYPE,
			MESH_RENDERER_COMPONENT_TYPE,
			TRANSFORM_COMPONENT_TYPE,
			PHYSICS_COMPONENT_TYPE,
			__COMPONENT_ENUM_TYPE_MAX,
			UNDEFINED_COMPONENT_TYPE
		};

		class GameObject* gameObject{ nullptr };
		bool enabled{ true };

		virtual void Init() = 0;
		virtual void ImGuiDrawComponent() = 0;

		Type GetType() const;
		std::string GetTypeName() const;
		static std::string TypeToName(Type type);

		Component();
	protected:
		Type type;
	};
}
