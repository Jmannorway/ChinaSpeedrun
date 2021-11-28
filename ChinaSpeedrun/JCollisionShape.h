#pragma once

#include "JCollisionAlgorithm.h"

#define DEFINE_TEST_COLLISION_OVERRIDES()\
JCollisionPoints TestCollision(const TransformComponent* tc, const JCollisionShape* cs, const TransformComponent* cptc) const override;\
JCollisionPoints TestCollision(const TransformComponent* tc, const JCollisionSphere* cs, const TransformComponent* cstc) const override;\
JCollisionPoints TestCollision(const TransformComponent* tc, const JCollisionPlane* cp, const TransformComponent* cptc) const override;\
JCollisionPoints TestCollision(const TransformComponent* tc, const JCollisionTriangle* ct, const TransformComponent* cttc) const override;

namespace cs
{
	class TransformComponent;
	class JCollisionPlane;
	class JCollisionSphere;
	class JCollisionTriangle;
	class RigidBodyComponent;

	struct JCollisionShape
	{
		// TODO: Fix inconsistencies in shape specific TestCollision implementations
		virtual JCollisionPoints TestCollision(
			const TransformComponent* tc, const JCollisionSphere* cs, const TransformComponent* cstc) const = 0;
		virtual JCollisionPoints TestCollision(
			const TransformComponent* tc, const JCollisionPlane* cp, const TransformComponent* cptc) const = 0;
		virtual JCollisionPoints TestCollision(
			const TransformComponent* tc, const JCollisionTriangle* ct, const TransformComponent* cttc) const = 0;
		virtual JCollisionPoints TestCollision(
			const TransformComponent* tc, const JCollisionShape* cs, const TransformComponent* cptc) const = 0;
	};

	struct JCollisionSphere : JCollisionShape
	{
		float radius;
		DEFINE_TEST_COLLISION_OVERRIDES()
		JCollisionSphere(float radius = 1.f);
	};

	struct JCollisionPlane : JCollisionShape
	{
		Vector3 plane;
		float length;
		DEFINE_TEST_COLLISION_OVERRIDES()
		JCollisionPlane(Vector3 plane = Vector3(0.f, 0.f, 1.f), float length = 0.f);
	};

	struct JCollisionTriangle : JCollisionShape
	{
		Vector3 p1, p2, p3;
		DEFINE_TEST_COLLISION_OVERRIDES()
			JCollisionTriangle(
				Vector3 p1 = Vector3(0.f), Vector3 p2 = Vector3(0.f), Vector3 p3 = Vector3(0.f));
	};
}