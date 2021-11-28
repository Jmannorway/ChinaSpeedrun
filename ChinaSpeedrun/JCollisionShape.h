#pragma once

#include "JCollisionAlgorithm.h"

#define DEFINE_TEST_COLLISION_OVERRIDES()\
CollisionPoints TestCollision(const TransformComponent* tc, const CollisionPlane* cp, const TransformComponent* cptc) const override;\
CollisionPoints TestCollision(const TransformComponent* tc, const CollisionShape* cs, const TransformComponent* cptc) const override;\
CollisionPoints TestCollision(const TransformComponent* tc, const CollisionSphere* cs, const TransformComponent* cstc) const override;\
CollisionPoints TestCollision(const TransformComponent* tc, const CollisionTriangle* ct, const TransformComponent* cttc) const override;

namespace cs
{
	class TransformComponent;
	class CollisionPlane;
	class CollisionSphere;
	class CollisionTriangle;
	class RigidBodyComponent;

	struct CollisionShape
	{
		virtual CollisionPoints TestCollision(
			const TransformComponent* tc, const CollisionSphere* cs, const TransformComponent* cstc) const = 0;
		virtual CollisionPoints TestCollision(
			const TransformComponent* tc, const CollisionPlane* cp, const TransformComponent* cptc) const = 0;
		virtual CollisionPoints TestCollision(
			const TransformComponent* tc, const CollisionTriangle* ct, const TransformComponent* cttc) const = 0;
		virtual CollisionPoints TestCollision(
			const TransformComponent* tc, const CollisionShape* cs, const TransformComponent* cptc) const = 0;
	};

	struct CollisionSphere : CollisionShape
	{
		float radius;
		DEFINE_TEST_COLLISION_OVERRIDES()
		CollisionSphere(float radius = 1.f);
	};

	struct CollisionPlane : CollisionShape
	{
		Vector3 plane;
		float length;
		DEFINE_TEST_COLLISION_OVERRIDES()
		CollisionPlane(Vector3 plane = Vector3(0.f, 0.f, 1.f), float length = 0.f);
	};

	struct CollisionTriangle : CollisionShape
	{
		Vector3 p1, p2, p3;
		DEFINE_TEST_COLLISION_OVERRIDES()
			CollisionTriangle(
				Vector3 p1 = Vector3(0.f), Vector3 p2 = Vector3(0.f), Vector3 p3 = Vector3(0.f));
	};
}