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
	const TransformComponent* tc, const JCollisionShape* cs, const TransformComponent* cstc) const
{
	return cs->TestCollision(tc, this, cstc);
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

/*
 * Collision plane
 */
cs::JCollisionPlane::JCollisionPlane(Vector3 plane, float length) :
	plane(plane), length(length)
{
}

cs::JCollisionPoints cs::JCollisionPlane::TestCollision(
	const TransformComponent* tc, const JCollisionShape* cs, const TransformComponent* cstc) const
{
	return cs->TestCollision(tc, this, cstc);
}

cs::JCollisionPoints cs::JCollisionPlane::TestCollision(
	const TransformComponent* tc, const JCollisionSphere* cs, const TransformComponent* cstc) const
{
	return algo::FindSpherePlaneCollisionPoints(cs, tc, this, cstc);
}

cs::JCollisionPoints cs::JCollisionPlane::TestCollision(
	const TransformComponent* tc, const JCollisionPlane* cp, const TransformComponent* cptc) const
{
	return algo::FindPlanePlaneCollisionPoints(cp, tc, this, cptc);
}

cs::JCollisionPoints cs::JCollisionPlane::TestCollision(
	const TransformComponent* tc, const JCollisionTriangle* ct, const TransformComponent* cptc) const
{
	return algo::FindPlaneTriangleCollisionPoints(this, tc, ct, cptc);
}

/*
 * Collision triangle
 */
cs::JCollisionTriangle::JCollisionTriangle(Vector3 p1, Vector3 p2, Vector3 p3) :
	p1(p1), p2(p2), p3(p3)
{
}

cs::JCollisionPoints cs::JCollisionTriangle::TestCollision(
	const TransformComponent* tc, const JCollisionShape* cs, const TransformComponent* cttc) const
{
	return cs->TestCollision(cttc, this, tc);
}

cs::JCollisionPoints cs::JCollisionTriangle::TestCollision(
	const TransformComponent* tc, const JCollisionSphere* cs, const TransformComponent* cstc) const
{
	return algo::FindSphereTriangleCollisionPoints(cs, cstc, this, tc);
}

cs::JCollisionPoints cs::JCollisionTriangle::TestCollision(
	const TransformComponent* tc, const JCollisionPlane* cp, const TransformComponent* cptc) const
{
	return algo::FindPlaneTriangleCollisionPoints(cp, cptc, this, tc);
}

cs::JCollisionPoints cs::JCollisionTriangle::TestCollision(
	const TransformComponent* tc, const JCollisionTriangle* ct, const TransformComponent* cttc) const
{
	return algo::FindTriangleTriangleCollisionPoints(ct, cttc, this, tc);
}
