#pragma once
#include "Mathf.h"

namespace cs
{
	class Mesh;

	class SceneUtility
	{
	public:
		static bool CreateStaticTriangleCollidersFromMesh(const Mesh* const mesh, Vector3 offset = Vector3(0.f));
	};
}

