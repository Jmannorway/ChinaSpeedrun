#pragma once

/**
 * A function but, now unused implementation of the locator
 * pattern for the physics system
 */
namespace cs {

	class PhysicsSystem;

	class PhysicsLocator
	{
	public:
		/**
		 * Gets a reference to the physics system
		 */
		static PhysicsSystem* GetPhysicsSystem();
		/**
		 * Supply the locator with a stable reference to
		 * the physics system
		 */
		static void Provide(PhysicsSystem* service);
	private:
		static PhysicsSystem* system;
	};
}

