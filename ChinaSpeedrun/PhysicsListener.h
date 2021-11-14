#pragma once

#include <b2/b2_world_callbacks.h>

namespace cs
{
	class PhysicsListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;
	};
}