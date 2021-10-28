#pragma once

class b2Body;

namespace cs {
	class RigidBodyComponent
	{
	public:
		RigidBodyComponent();
		b2Body* body;
	};
}
