#include "JCollisionShape.h"

#include "JPhysicsSystem.h"

/*
 * Collision sphere
 */
cs::JCollisionSphere::JCollisionSphere(float radius) :
	radius(radius)
{
}

cs::JCollisionPoints cs::JCollisionSphere::TestCollision(
	const TransformComponent* tc, const JCollisionSphere* cs, const TransformComponent* cstc) const
{
	return algo::FindSphereSphereCollisionPoints(this, tc, cs, cstc);
}

cs::JCollisionPoints cs::JCollisionSphere::TestCollision(
	const TransformComponent* tc, const JCollisionPlane* cp, const TransformComponent* cptc) const
{
	return algo::FindSpherePlaneCollisionPoints(this, tc, cp, cptc);
}

cs::JCollisionPoints cs::JCollisionSphere::TestCollision(
	const TransformComponent* tc, const JCollisionTriangle* ct, const TransformComponent* cptc) const
{
	return algo::FindSphereTriangleCollisionPoints(this, tc, ct, cptc);
}

cs::JCollisionPoints cs::JCollisionSphere::TestCollision(
	const TransformComponent* tc, const JCollisionShape* cs, const TransformComponent* cstc) const
{
	return cs->TestCollision(tc, this, cstc);
}

/*
 * Collision plane
 */
cs::JCollisionPlane::JCollisionPlane(Vector3 plane, float length) :
	plane(plane), length(length)
{
}

/*
 * Collision triangle
 */
cs::JCollisionTriangle::JCollisionTriangle(Vector3 p1, Vector3 p2, Vector3 p3) :
	p1(p1), p2(p2), p3(p3)
{
}
