#pragma once

#include "Mathf.h"
#include "Component.h"

namespace cs
{
	class TransformComponent : public Component
	{
	public:
		friend class Transform;

		Vector3 position{ 0.f }, rotation{ 0.f }, rotationDegrees{ 0.f }, scale{ 1.f };
		Vector3 globalPosition{ 0.f }, globalRotation{ 0.f }, globalScale{ 1.f };

		operator Matrix4x4&();
		virtual void Init() override;
		virtual void ImGuiDrawComponent() override;
		void GenerateOBBExtents(OBB& obb);

	protected:
		Matrix4x4 matrix{ Matrix4x4(1.0f) };
	};

	class Transform
	{
	public:
		static void CalculateMatrix(TransformComponent& transform);
		static Matrix4x4& GetMatrixTransform(TransformComponent& transform);
	};
}
