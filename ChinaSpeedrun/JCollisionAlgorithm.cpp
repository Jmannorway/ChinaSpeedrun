#include "JCollisionAlgorithm.h"

#include "JCollisionShape.h"
#include "Transform.h"

cs::CollisionPoints cs::algo::FindSpherePlaneCollisionPoints(const CollisionSphere* cs, const TransformComponent* cstc,
	const CollisionPlane* cp, const TransformComponent* cptc)
{
	return CollisionPoints();
}

cs::CollisionPoints cs::algo::FindSphereTriangleCollisionPoints(const CollisionSphere* cs,
	const TransformComponent* cstc, const CollisionTriangle* ct, const TransformComponent* cttc)
{
	return CollisionPoints();
}

cs::CollisionPoints cs::algo::FindPlanePlaneCollisionPoints(
	const CollisionPlane* cp1, const TransformComponent* cptc1, const CollisionPlane* cp2, const TransformComponent* cptc2)
{
	return CollisionPoints();
}

cs::CollisionPoints cs::algo::FindPlaneTriangleCollisionPoints(
	const CollisionPlane* cp, const TransformComponent* cptc, const CollisionTriangle* ct, const TransformComponent* cttc)
{
	return CollisionPoints();
}

cs::CollisionPoints cs::algo::FindTriangleTriangleCollisionPoints(
	const CollisionTriangle* ct1, const TransformComponent* cttc1, const CollisionTriangle* ct2, const TransformComponent* cttc2)
{
	return CollisionPoints();
}

cs::CollisionPoints cs::algo::FindSphereSphereCollisionPoints(
	const CollisionSphere* cs1, const TransformComponent* cstc1, const CollisionSphere* cs2, const TransformComponent* cstc2)
{
	CollisionPoints _points;
	const float _distance(distance(cstc1->position, cstc2->position));
	_points.normal = normalize(cstc2->position - cstc1->position);
	_points.a = cstc1->position + _points.normal * cs1->radius;
	_points.b = cstc2->position - _points.normal * cs2->radius;
	_points.depth = -_distance + cs1->radius + cs2->radius;
	_points.hasCollision = _distance <= cs1->radius + cs2->radius;
	return _points;
}

cs::CollisionPoints::CollisionPoints() : a(0.f), b(0.f), normal(0.f), depth(0.f), hasCollision(false)
{
}
