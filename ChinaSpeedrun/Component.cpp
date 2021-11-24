#include "Component.h"

cs::ComponentMeta::Type cs::Component::GetType() const
{
	return type;
}

std::string cs::Component::GetTypeName() const
{
	return ComponentMeta::TypeToName(type);
}

cs::Component::Component() : type(ComponentMeta::UNDEFINED_COMPONENT_TYPE)
{

}
