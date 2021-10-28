#pragma once

class b2World;

namespace cs {
	class PhysicsSystem
	{
	public:
		int velocityIterations, positionIterations;
		b2World* world;
		PhysicsSystem(int vIter = 6, int pIter = 2);
	};
}
