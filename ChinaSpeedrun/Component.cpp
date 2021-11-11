#include "Component.h"

cs::Component::Type cs::Component::GetType() const
{
	return type;
}

std::string cs::Component::GetTypeName() const
{
	return TypeToName(type);
}

std::string cs::Component::TypeToName(Type type)
{
	switch(type)
	{
	case UNDEFINED_COMPONENT_TYPE:			return "Component";
	case AUDIO_COMPONENT_TYPE:			return "Audio";
	case CAMERA_COMPONENT_TYPE:			return "Camera";
	case PHYSICS_COMPONENT_TYPE:			return "Physics";
	case MESH_RENDERER_COMPONENT_TYPE:		return "MeshRenderer";
	case TRANSFORM_COMPONENT_TYPE:		return "Transform";
	default:							return "";
	}
}

cs::Component::Component() : type(UNDEFINED_COMPONENT_TYPE)
{

}
