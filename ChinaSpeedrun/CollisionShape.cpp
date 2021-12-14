#include "CollisionShape.h"

#include <b2/b2_circle_shape.h>

cs::CollisionShapeDefinition::CollisionShapeDefinition() :
	type(Type::Rectangle), extents({ 1.f }), radius(1.f)
{
}

b2BoxShape::b2BoxShape() : extents({0.5f, 0.5f})
{
	SetAsBox(extents.x, extents.y);
}

b2Vec2 b2BoxShape::GetExtents() const
{
	return extents;
}

void b2BoxShape::SetExtents(b2Vec2 newExtents)
{
	extents = newExtents;
	SetAsBox(extents.x, extents.y);
}
