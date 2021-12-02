#pragma once

namespace cs
{
	class Mesh;

	class SceneUtility
	{
	public:
		static bool CreateStaticTriangleCollidersFromMesh(const Mesh* const mesh);
	};
}

