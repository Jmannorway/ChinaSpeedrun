#pragma once

#include "Component.h"
#include "CameraBase.h"

namespace cs
{
	class CameraComponent : public Component, public CameraBase
	{
	public:
		friend class Camera;

		CameraComponent();
		virtual void Init() override;
		virtual void ImGuiDrawComponent() override;
	};
}
