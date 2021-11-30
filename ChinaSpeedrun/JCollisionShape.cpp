#include "JCollisionShape.h"

#include <string>

#include "imgui.h"
#include "JPhysicsSystem.h"

/*
 * Collision sphere
 */

cs::JCollisionSphere::JCollisionSphere(float radius) :
	radius(radius)
{
	type = Type::Sphere;
}

cs::JCollisionShape::Type cs::JCollisionShape::GetType() const
{
	return type;
}

std::string cs::JCollisionShape::TypeToString(Type type)
{
	switch (type)
	{
	case Type::Sphere:		return "Sphere";
	case Type::Plane:		return "Plane";
	case Type::Triangle:	return "Triangle";
	default:				return "Invalid";
	}
}

void cs::JCollisionSphere::ImGuiDrawCollisionShape()
{
	if (ImGui::TreeNodeEx("Sphere Collision Shape"))
	{
		ImGui::DragFloat("Radius", &radius);
		ImGui::TreePop();
	}
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
	type = Type::Plane;
}

void cs::JCollisionPlane::ImGuiDrawCollisionShape()
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
void cs::JCollisionTriangle::SetPoint(unsigned index, Vector3 point)
{
	points[index] = point;
	CalculateNormal();
	type = Type::Triangle;
}

void cs::JCollisionTriangle::SetPoints(Vector3 p1, Vector3 p2, Vector3 p3)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	CalculateNormal();
}

Vector3 cs::JCollisionTriangle::GetLine(unsigned index) const
{
	return points[(index + 1) % 3] - points[index];
}

Vector3 cs::JCollisionTriangle::GetPoint(unsigned index) const
{
	return points[index];
}

Vector3 cs::JCollisionTriangle::GetCenter() const
{
	return (points[0] + points[1] + points[2]) / 3.f;
}

Vector3 cs::JCollisionTriangle::GetNormal() const
{
	return normal;
}

void cs::JCollisionTriangle::ImGuiDrawCollisionShape()
{
	if (ImGui::TreeNodeEx("Triangle Collision Shape"))
	{
		bool _changed = false;
		Vector3 _p[3] = { GetPoint(0), GetPoint(1), GetPoint(2) };
		for (unsigned i = 0; i < 3; i++)
			_changed |= ImGui::DragFloat3(("Point " + std::to_string(i)).c_str(), &_p[i].x);
		if (_changed)
			SetPoints(_p[0], _p[1], _p[2]);

		ImGui::TreePop();
	}
}

cs::JCollisionTriangle::JCollisionTriangle(Vector3 p1, Vector3 p2, Vector3 p3)
{
	type = Type::Triangle;
	SetPoints(p1, p2, p3);
}

void cs::JCollisionTriangle::CalculateNormal()
{
	normal = normalize(cross(GetLine(0), GetLine(1)));
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
