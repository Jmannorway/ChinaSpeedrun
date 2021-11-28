#include "JCollisionShape.h"

/*
 * Collision sphere
 */
cs::CollisionSphere::CollisionSphere(float radius) :
	radius(radius)
{
}

cs::CollisionPoints cs::CollisionSphere::TestCollision(
	const TransformComponent* tc, const CollisionSphere* cs, const TransformComponent* cstc) const
{
	return algo::FindSphereSphereCollisionPoints(this, tc, cs, cstc);
}

cs::CollisionPoints cs::CollisionSphere::TestCollision(
	const TransformComponent* tc, const CollisionPlane* cp, const TransformComponent* cptc) const
{
	return algo::FindSpherePlaneCollisionPoints(this, tc, cp, cptc);
}

cs::CollisionPoints cs::CollisionSphere::TestCollision(
	const TransformComponent* tc, const CollisionTriangle* ct, const TransformComponent* cptc) const
{
	return algo::FindSphereTriangleCollisionPoints(this, tc, ct, cptc);
}

cs::CollisionPoints cs::CollisionSphere::TestCollision(
	const TransformComponent* tc, const CollisionShape* cs, const TransformComponent* cstc) const
{
	return cs->TestCollision(tc, this, cstc);
}

/*
 * Collision plane
 */
cs::CollisionPlane::CollisionPlane(Vector3 plane, float length) :
	plane(plane), length(length)
{
}

/*
 * Collision triangle
 */
cs::CollisionTriangle::CollisionTriangle(Vector3 p1, Vector3 p2, Vector3 p3) :
	p1(p1), p2(p2), p3(p3)
{
}
