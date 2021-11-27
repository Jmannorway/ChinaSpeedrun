#include "ComponentMeta.h"

std::string cs::ComponentMeta::TypeToName(Type type)
{
	switch (type)
	{
	case UNDEFINED_COMPONENT_TYPE:		return "Component";
	case AUDIO_COMPONENT_TYPE:			return "Audio";
	case CAMERA_COMPONENT_TYPE:			return "Camera";
	case PHYSICS_COMPONENT_TYPE:		return "Physics";
	case MESH_RENDERER_COMPONENT_TYPE:	return "MeshRenderer";
	case TRANSFORM_COMPONENT_TYPE:		return "Transform";
	case BSOD_COMPONENT_TYPE:			return "BSOD";
	default:							return "";
	}
}

std::string cs::ComponentMeta::GetAllNamesNullSep()
{
	std::string _names;
	for (unsigned i = 0; i < __COMPONENT_ENUM_TYPE_MAX; i++)
	{
		_names += TypeToName(static_cast<Type>(i)) + "\0";
	}
	return _names;
}

unsigned cs::ComponentMeta::GetComponentTypeMax()
{
	return __COMPONENT_ENUM_TYPE_MAX;
}
