#pragma once

#include "Mathf.h"
#include "Component.h"

#include <queue>

namespace cs
{
	class TransformComponent : public Component
	{
	public:
		friend class Transform;

		Vector3 position{ 0.f }, rotation{ 0.f }, rotationDegrees{ 0.f }, scale{ 1.f };
		Vector3 globalPosition{ 0.f }, globalRotation{ 0.f }, globalScale{ 1.f };

		operator Matrix4x4&();

		void SetPosition(const Vector3 pos);
		void SetRotation(const Vector3 rot);
		void SetRotationInDegrees(const Vector3 rotDeg);
		void SetScale(const Vector3 scale);

		virtual void Init() override;
		virtual void ImGuiDrawComponent() override;
		void GenerateOBBExtents(OBB& obb);

		TransformComponent();

	protected:
		Matrix4x4 matrix{ Matrix4x4(1.0f) };

	private:
		bool isQueued;
	};

	class Transform
	{
	public:
		static std::queue<TransformComponent*> updateTransform;

		static void CalculateMatrix(TransformComponent& transform);
		static Matrix4x4& GetMatrixTransform(TransformComponent& transform);
	};
}
