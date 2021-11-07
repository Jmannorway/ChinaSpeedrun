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
	case Type::Audio:			return "Audio";
	case Type::Camera:			return "Camera";
	case Type::Physics:			return "Physics";
	case Type::MeshRenderer:	return "MeshRenderer";
	case Type::Transform:		return "Transform";
	default:					return "";
	}
}
