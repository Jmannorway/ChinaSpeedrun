#pragma once

#include "Mathf.h"
#include <b2/b2_polygon_shape.h>

// ooga booga custom class ooga booga
// this may have consequences in the future
class b2BoxShape : public b2PolygonShape
{
public:
	b2BoxShape();
	b2Vec2 GetExtents() const;
	void SetExtents(b2Vec2 newExtents);
protected:
	b2Vec2 extents;
};

namespace cs
{
	struct CollisionShapeDefinition
	{
		enum class Type : unsigned { Circle, Rectangle } type;
		Vector2 extents;
		float radius;
		CollisionShapeDefinition();
	};
}

