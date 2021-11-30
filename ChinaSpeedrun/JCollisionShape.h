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

		virtual void ImGuiDrawCollisionShape() = 0;

		enum class Type : unsigned { Invalid, Sphere, Plane, Triangle, Max };

		Type GetType() const;
		static std::string TypeToString(Type type);
	protected:
		Type type = Type::Invalid;
	};

	struct JCollisionSphere : JCollisionShape
	{
		float radius;
		void ImGuiDrawCollisionShape() override;
		DEFINE_TEST_COLLISION_OVERRIDES()
		JCollisionSphere(float radius = 1.f);
	};

	struct JCollisionPlane : JCollisionShape
	{
		Vector3 plane;
		float length;
		void ImGuiDrawCollisionShape() override;
		DEFINE_TEST_COLLISION_OVERRIDES()
		JCollisionPlane(Vector3 plane = Vector3(0.f, 0.f, 1.f), float length = 0.f);
	};

	struct JCollisionTriangle : JCollisionShape
	{
		void SetPoint(unsigned index, Vector3 point);
		void SetPoints(Vector3 p1, Vector3 p2, Vector3 p3);
		Vector3 GetLine(unsigned index) const;
		Vector3 GetPoint(unsigned index) const;
		Vector3 GetCenter() const;
		Vector3 GetNormal() const;
		void ImGuiDrawCollisionShape() override;
		DEFINE_TEST_COLLISION_OVERRIDES()
			JCollisionTriangle(
				Vector3 p1 = Vector3(0.f), Vector3 p2 = { 0.f, 1.f, 0.f }, Vector3 p3 = {0.5f, 0.5f, 0.f});
	private:
		void CalculateNormal();
		Vector3 points[3];
		Vector3 normal;
	};
}
