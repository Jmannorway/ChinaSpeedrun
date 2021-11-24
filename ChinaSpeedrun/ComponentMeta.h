#pragma once
#include <string>

namespace cs
{
	class ComponentMeta
	{
	public:
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

		static std::string TypeToName(Type type);
		static std::string GetAllNamesNullSep();
		static unsigned GetComponentTypeMax();
	};
}
