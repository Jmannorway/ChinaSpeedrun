#pragma once

#include <entt/entt.hpp>

namespace cs
{
	class AudioSystem;
	class PhysicsSystem;

	class World
	{
	public:
		entt::registry registry;
		AudioSystem* audioSystem;
		
		World();
		
		void Step();
	};
}