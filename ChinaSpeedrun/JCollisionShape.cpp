#include "JCollisionShape.h"

#include <string>

#include "Color.h"
#include "imgui.h"
#include "Draw.h"
#include "Color.h"
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

void cs::JCollisionSphere::Draw()
{
	Draw::Circle(radius, Vector3(0.f), Color::magenta);
}

void cs::JCollisionSphere::ImGuiDraw()
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
cs::JCollisionPlane::JCollisionPlane(Vector3 plane)
{
	SetPlane(plane);
	type = Type::Plane;
}

void cs::JCollisionPlane::SetPlane(Vector3 plane)
{
	planeVector = plane;
	CalculateNormal();
}

Vector3 cs::JCollisionPlane::GetPlane() const
{
	return planeVector;
}

Vector3 cs::JCollisionPlane::GetNormal() const
{
	return normal;
}

void cs::JCollisionPlane::CalculateNormal()
{
	normal = normalize(planeVector);
}

void cs::JCollisionPlane::Draw()
{
}

void cs::JCollisionPlane::ImGuiDraw()
{
	if (ImGui::TreeNodeEx("Collision Plane Shape"))
	{
		Vector3 _plane = GetPlane();
		if (ImGui::DragFloat3("Plane", &_plane.x))
			SetPlane(_plane);

		ImGui::TreePop();
	}
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
	Update();
	type = Type::Triangle;
}

void cs::JCollisionTriangle::SetPoints(Vector3 p1, Vector3 p2, Vector3 p3)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	Update();
}

Vector3 cs::JCollisionTriangle::GetLine(unsigned index) const
{
	return points[(index + 1) % 3] - points[index];
}

Vector3 cs::JCollisionTriangle::GetLineTangent(unsigned index) const
{
	return lineTangents[index];
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

void cs::JCollisionTriangle::ImGuiDraw()
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

void cs::JCollisionTriangle::Draw()
{
	Draw::Line(
		{ GetLine(0), GetLine(1), GetLine(2), GetLine(0) }, 
		{ Color::magenta, Color::magenta, Color::magenta, Color::magenta },
		Draw::DrawMode::SOLID);
}

cs::JCollisionTriangle::JCollisionTriangle(Vector3 p1, Vector3 p2, Vector3 p3)
{
	type = Type::Triangle;
	SetPoints(p1, p2, p3);
}

void cs::JCollisionTriangle::Update()
{
	CalculateNormal();
	CalculateLineTangents();
}

void cs::JCollisionTriangle::CalculateNormal()
{
	normal = normalize(cross(GetLine(0), GetLine(1)));
}

void cs::JCollisionTriangle::CalculateLineTangents()
{
	for (unsigned i = 0; i < 3; i++)
		lineTangents[i] = normalize(cross(normal, GetLine(i)));
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
